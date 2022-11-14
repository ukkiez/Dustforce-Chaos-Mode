#include "../lib/std.cpp";

#include "../lib/Random.cpp";

#include "./QueueEvent.cpp";
#include "./events/index.cpp";

#include "../EventList.cpp";

class ActiveEvent {
  QueueEvent@ event;

  uint time = 0;
  uint duration = 0;
  bool keep_running = false;

  bool initialized = false;

  string name;
  string subtext;
  uint colour;

  ActiveEvent( QueueEvent@ qe, uint duration ) {
    @this.event = qe;
    this.duration = duration;

    QueueEventConfig config = qe.get_config();
    this.keep_running = config.keep_running;

    this.name = config.name;
    this.subtext = config.subtext;
    this.colour = config.colour;
  }
}

const array<uint> TEXT_COLOURS = {
  0x11FFFFFF,
  0x22FFFFFF,
  0x33FFFFFF,
  0x44FFFFFF,
  0x55FFFFFF,
  0x66FFFFFF,
  0x77FFFFFF,
  0x88FFFFFF,
  0x99FFFFFF,
  0xAAFFFFFF,
  0xBBFFFFFF,
};

class Queue : Random {
  // if true, gets only the specified DEBUG events from the get_queue_events()
  // in ./events/index.cpp, giving them a 100 weight, guaranteeing them to be
  // available to be picked every round (though still not twice in a row, and
  // retaining the existing limit of concurrent number of events)
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

  int last_picked_event_index = -1;

  bool checkpoint_loaded = false;

  array<QueueEvent@> @events;

  array<ActiveEvent@> @active_events = {};
  array<uint> active_event_indexes;

  array<QueueEventConfig> event_configs = {};

  // interval in seconds
  uint interval = 1;
  uint interval_min = 1;
  uint interval_max = 5;

  // min/max random event duration in frames
  uint duration_min = 180;
  uint duration_max = 720;

  array<textfield@> event_name_textfields;
  array<textfield@> event_subtext_textfields;
  uint text_display_time = 0;
  uint text_fadeout_time = 120;
  uint colours_index = 0;

  bool turbo_mode = false;

  Queue() {
    @g = get_scene();

    if ( DEBUG_MODE ) {
      puts( "--- DEBUG_MODE ON ---\n" );
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

      initialized = true;
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
        int duration = qe.get_config().duration;
        if ( duration == -1 ) {
          duration = srand_range( duration_min, duration_max );
        }

        active_events.insertLast( ActiveEvent( qe, duration ) );
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

    if ( event_name_textfields.length > 0 ) {
      if ( text_display_time == 0 ) {
        colours_index = 0;
      }

      event_name_textfields[ 0 ].colour(
        TEXT_COLOURS[ colours_index ]
      );

      if ( event_subtext_textfields.length > 0 ) {
        event_subtext_textfields[ 0 ].colour(
          TEXT_COLOURS[ colours_index ]
        );
      }

      if ( text_display_time >= text_fadeout_time ) {
        if ( colours_index >= 0 ) {
          if ( text_display_time % 2 == 0 ) {
            if ( colours_index == 0 ) {
              event_name_textfields.resize( 0 );
              event_subtext_textfields.resize( 0 );
            }
            else {
              colours_index--;
            }
          }
        }
      }
      else if ( ( colours_index < TEXT_COLOURS.length-1 ) && ( text_display_time % 2 ) == 0 ) {
        colours_index++;
      }

      text_display_time++;
    }

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
      if ( event_configs[ i ].weight >= range ) {
        pool.insertLast( events[ i ] );
      }
    }

    return pool;
  }

  void work_queue( int entities ) {
    for ( uint i = 0; i < active_events.length; i++ ) {
      ActiveEvent@ active_event = active_events[ i ];
      QueueEvent@ event = active_event.event;

      if ( !active_event.initialized ) {
        event.initialize();

        script.event_list.add_queue_element( active_event.name, active_event.subtext );
        active_event.initialized = true;
      }

      if ( active_event.keep_running ) {
        // keep running this event until it sends a signal to deactivate itself
        event.step( entities );
        continue;
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
