#include "../lib/std.cpp";

#include "../lib/Random.cpp";

#include "./QueueEvent.cpp";
#include "./events/index.cpp";

#include "../EventList.cpp";

class ActiveQueueEvent {
  QueueEvent@ event;

  uint time = 0;
  uint duration = 0;

  bool initialized = false;

  string name;
  string subtext;
  uint colour;

  ActiveQueueEvent( QueueEvent@ qe, uint duration ) {
    @this.event = qe;
    this.duration = duration;

    QueueEventConfig config = qe.get_config();

    this.name = config.name;
    this.subtext = config.subtext;
    this.colour = config.colour;
  }
}

class Queue : Random {
  // if true, gets only the specified DEBUG events from the get_queue_events()
  // in ./events/index.cpp, giving them a 100 weight, guaranteeing them to be
  // available to be picked every round (though still not twice in a row, and
  // retaining the existing limit of concurrent number of events); this can be
  // altered by an annotation specified in the main script module
  bool DEBUG_MODE = false;

	script@ script;
  scene@ g;
  dustman@ player;

  bool initialized = false;

  // keep track of which iteration we're on currently, to help deal with
  // checkpoints
  uint iteration = 0;

  // keep track of the Queue's step() time between the last event pick
  uint time_since_last_pick = 0;

  bool checkpoint_loaded = false;

  array<QueueEvent@> @events;

  array<ActiveQueueEvent@> @active_events = {};

  array<QueueEventConfig> event_configs = {};

  string last_picked_event_name;

  // interval in seconds
  uint interval = 1;
  uint interval_min = 1;
  uint interval_max = 5;

  // min/max random event duration in frames
  uint duration_min = 180;
  uint duration_max = 720;

  bool turbo_mode = false;

  Queue() {
    @g = get_scene();
  }

  void init() {
    if ( !initialized ) {
      controllable@ c = controller_controllable( 0 );
      if ( @c == null ) {
        return;
      }

      @player = c.as_dustman();

      // get the main script object
      @script = cast<script@>( get_script() );

      if ( DEBUG_MODE ) {
        puts( "--- QUEUE DEBUG_MODE ON ---" );
      }

      @events = get_queue_events( DEBUG_MODE );
      for ( uint i = 0; i < events.length; i++ ) {
        QueueEventConfig config = events[ i ].get_config();

        if ( DEBUG_MODE ) {
          config.weight = 100;
        }

        // fill up the array with all event configurations up front, so they will
        // be readily available for the script
        event_configs.insertLast( config );
      }

      initialized = true;
    }
  }

  void entity_on_add( entity@ e ) {
    for ( uint i = 0; i < active_events.length; i++ ) {
      active_events[ i ].event.entity_on_add( e );
    }
  }
  void entity_on_remove( entity@ e ) {
    for ( uint i = 0; i < active_events.length; i++ ) {
      active_events[ i ].event.entity_on_add( e );
    }
  }

  void step( int entities ) {
    if ( @player == null ) {
      controllable@ c = controller_controllable( 0 );
      if ( @c == null ) {
        return;
      }

      @player = c.as_dustman();
      if ( @player == null ) {
        return;
      }
    }

    if ( player.dead() ) {
      // just stop the queue at this point, otherwise we might run into e.g.
      // issues with gathering the player controllable, or shenanigans during
      // loading
      return;
    }

    // activate new events every X seconds
    if ( time_since_last_pick % ( interval * 60 ) == 0 ) {
      time_since_last_pick = 0;
      iteration++;

      // randomly filter out available events to be activated next, based on
      // their weight
      array<QueueEvent@> pool = filter_events();
      if ( pool.length > 0 ) {
        // get a random event from the pool
        QueueEvent@ qe = pool[ srand_range( 0, pool.length-1 ) ];
        QueueEventConfig config = qe.get_config();
        int duration = config.duration;
        if ( duration == -1 ) {
          duration = srand_range( duration_min, duration_max );
        }

        active_events.insertLast( ActiveQueueEvent( qe, duration ) );

        last_picked_event_name = config.name + config.subtext;
      }
      else {
        last_picked_event_name = "";
      }

      if ( !turbo_mode ) {
        // choose after how many seconds we will pick the next random Event
        interval = srand_range( interval_min, interval_max );
      }
      else {
        // the interval is always 1 during turbo mode
        interval = 1;
      }
    }

    work_queue( entities );

    time_since_last_pick++;
  }

  array<QueueEvent@> filter_events() {
    if ( turbo_mode ) {
      // don't filter any events by weight during turbo mode
      return get_queue_events( DEBUG_MODE );
    }

    // generate a range to exclude events based on their weight, e.g. a rolled
    // range of 50 means all weights below 50 should not be included in the pool
    uint range = srand_range( 1, 100 );

    // get new class instances of the events
    @events = get_queue_events( DEBUG_MODE );

    array<QueueEvent@> pool = {};
    for ( uint i = 0; i < events.length; i++ ) {
      if ( ( event_configs[ i ].name + event_configs[ i ].subtext ) == last_picked_event_name ) {
        // don't pick the same event twice in a row
        continue;
      }

      if ( event_configs[ i ].weight >= range ) {
        pool.insertLast( events[ i ] );
      }
    }

    return pool;
  }

  void work_queue( int entities ) {
    for ( uint i = 0; i < active_events.length; i++ ) {
      ActiveQueueEvent@ active_event = active_events[ i ];
      QueueEvent@ event = active_event.event;

      if ( !active_event.initialized ) {
        event.initialize();

        script.event_list.add_queue_element( active_event.name, active_event.subtext );
        active_event.initialized = true;
      }

      if ( active_event.duration == 0 ) {
        // this event simply initializes and doesn't have to do anything else,
        // so remove it from the queue
        active_events.removeAt( i );
        continue;
      }

      if ( active_event.time < active_event.duration ) {
        // just step the event, as well as increasing the time it has been
        // active
        event.step( entities );
        active_event.time++;
      }
      else {
        // deactivate and remove the event from the queue
        event.deactivate();
        active_events.removeAt( i );
      }
    }
  }

  void checkpoint_save() {}
  void checkpoint_load() {
    controllable@ c = controller_controllable( 0 );
    if ( @c == null ) {
      return;
    }

    @player = c.as_dustman();

    checkpoint_loaded = true;
  }

  void draw( float sub_frame ) {
    // call active event draw methods
    for ( uint i = 0; i < active_events.length; i++ ) {
      // call the event's draw method itself
      active_events[ i ].event.draw( sub_frame );
    }
  }

  void activate_turbo_mode() {
    turbo_mode = true;
    interval = 1;
    time_since_last_pick = 0;
  }
  void deactivate_turbo_mode() {
    turbo_mode = false;
    time_since_last_pick = 0;
  }
}
