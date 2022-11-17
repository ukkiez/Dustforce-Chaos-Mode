#include "../lib/std.cpp";

#include "../lib/Random.cpp";

#include "./CycleEvent.cpp";

#include "./events/index.cpp";

class ActiveCycleEvent {
  CycleEvent@ event;

  bool initialized = false;

  string name;
  string subtext;
  uint colour;

  ActiveCycleEvent( CycleEvent@ qe ) {
    @this.event = qe;

    CycleEventConfig config = qe.get_config();

    this.name = config.name;
    this.subtext = config.subtext;
    this.colour = config.colour;
  }
}

class Cycle : Random {
  // if true, gets only the specified DEBUG events from the get_cycle_events()
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

  // time_since_last_pick that current events are active (in step() frames)
  uint time_since_last_pick = 0;

  bool active_event = false;

  bool checkpoint_loaded = false;

  array<CycleEvent@> @events = {};

  array<ActiveCycleEvent@> @active_events = {};

  array<CycleEventConfig> event_configs = {};

  array<uint> active_event_indexes;

  // interval in seconds
  uint interval = 1;
  uint interval_min = 5;
  uint interval_max = 12;
  uint current_num_active_events;

  array<ActiveCycleEvent@> CHECKPOINT_active_events;
  array<uint> CHECKPOINT_active_event_indexes;
  uint CHECKPOINT_iteration;

  bool turbo_mode = false;

  Cycle() {
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
        puts( "--- CYCLE DEBUG_MODE ON ---" );
      }

      @events = get_cycle_events( DEBUG_MODE );
      for ( uint i = 0; i < events.length; i++ ) {
        CycleEventConfig config = events[ i ].get_config();

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
    if ( active_event ) {
      for ( uint i = 0; i < current_num_active_events; i++ ) {
        events[ active_event_indexes[ i ] ].entity_on_add( e );
      }
    }
  }
  void entity_on_remove( entity@ e ) {
    if ( active_event ) {
      for ( uint i = 0; i < current_num_active_events; i++ ) {
        events[ active_event_indexes[ i ] ].entity_on_remove( e );
      }
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
      // just stop the cycle at this point, otherwise we might run into e.g.
      // issues with gathering the player controllable, or shenanigans during
      // loading
      return;
    }

    // activate new events every X seconds
    if ( time_since_last_pick % ( interval * 60 ) == 0 ) {
      time_since_last_pick = 0;
      iteration++;
      script.event_list.iterate_event_cycle();

      if ( !turbo_mode ) {
        // choose after how many seconds we will pick the next set of Events
        // after this one
        interval = srand_range( interval_min, interval_max );
      }
      else {
        // the interval is always 1 during turbo mode
        interval = 1;
      }

      // randomly filter out available events to be activated next, based on
      // their weight and whether they were activated the previous time
      array<CycleEvent@> pool = filter_events();

      // deactivate and remove the previously active events
      remove_active_events();

      if ( active_event_indexes.length > 0 ) {
        // empty out the active event index list
        active_event_indexes.removeRange( 0, active_event_indexes.length );
      }

      // pick a range for the number of active events that is more weighted
      // towards 3 and 4 than any other
      current_num_active_events = pick_number_of_events();

      if ( current_num_active_events > pool.length ) {
        current_num_active_events = pool.length;
      }

      if ( pool.length > 0 ) {
        for ( uint i = 0; i < current_num_active_events; i++ ) {
          // randomly pick new events to activate

          uint index = srand_range( 0, pool.length - 1 );
          while ( active_event_indexes.find( index ) >= 0 ) {
            // don't pick duplicate events
            index = srand_range( 0, pool.length - 1 );
          }

          active_event_indexes.insertLast( index );

          // get a random event from the pool
          CycleEvent@ ce = pool[ index ];
          CycleEventConfig config = ce.get_config();

          active_events.insertLast( ActiveCycleEvent( ce ) );
        }

        active_event = true;
      }
      else {
        active_event = false;
        // never stay without any Events active for longer than 5 seconds
        interval = ( interval < 5 ) ? 5 : interval;
      }
    }

    work_cycle( entities );

    // always increment the time_since_last_pick, even if no Event is active,
    // because that would just mean that last time we didn't have any Events
    // available and we're just playing vanilla for <interval> seconds
    time_since_last_pick++;
  }

  void work_cycle( int entities ) {
    if ( checkpoint_loaded && ( CHECKPOINT_iteration != iteration ) ) {
      // reinitialize and deactivate all events that were active at the last
      // saved checkpoint, if we're past the iteration from that time;
      // reinitialization is necessary since script variables may have been
      // reset/lost
      for ( uint i = 0; i < CHECKPOINT_active_events.length; i++ ) {
        CHECKPOINT_active_events[ i ].event.initialize();
        CHECKPOINT_active_events[ i ].event.deactivate();
      }
    }

    for ( uint i = 0; i < active_events.length; i++ ) {
      ActiveCycleEvent@ active_event = active_events[ i ];
      CycleEvent@ event = active_event.event;

      if ( !active_event.initialized ) {
        event.initialize();

        // add the event to the EventList
        script.event_list.add_cycle_element( active_event.name, active_event.subtext );
        active_event.initialized = true;
      }

      if ( checkpoint_loaded ) {
        // reinitialize the event after we loaded from a checkpoint, to make
        // sure the script variables aren't messed up
        event.deactivate();
        event.initialize();
      }

      // just step the event, as well as increasing the time it has been
      // active
      event.step( entities );
    }

    if ( checkpoint_loaded ) {
      checkpoint_loaded = false;
    }
  }

  array<CycleEvent@> filter_events() {
    uint range;
    if ( !turbo_mode ) {
      // generate a range to exclude events based on their weight, e.g. a rolled
      // range of 50 means all weights below 50 should not be included in the pool
      range = srand_range( 1, 100 );
    }
    else {
      // practically give all events a weight of 100 during turbo mode
      range = 1;
    }

    // get new class instances of the events
    @events = get_cycle_events( DEBUG_MODE );

    array<CycleEvent@> pool = {};
    for ( uint i = 0; i < events.length; i++ ) {
      string id = event_configs[ i ].name + event_configs[ i ].subtext;

      bool broke = false;
      for ( uint j = 0; j < active_events.length; j++ ) {
        if ( id == ( active_events[ j ].name + active_events[ j ].subtext ) ) {
          broke = true;
          break;
        }
      }
      if ( broke ) {
        continue;
      }

      if ( event_configs[ i ].weight >= range ) {
        pool.insertLast( events[ i ] );
      }
    }

    return pool;
  }

  uint pick_number_of_events() {
    // pick a range for the number of active events that is more weighted
    // towards 2 and 3

    int range = srand_range( 2, 6 );
    if ( range <= 3 ) {
      return 2;
    }
    else if ( range <= 5 ) {
      return 3;
    }
    else if ( range == 6 ) {
      return 4;
    }
    else {
      return 3;
    }
  }

  void remove_active_events() {
    for ( uint i = 0; i < active_events.length; i++ ) {
      active_events[ i ].event.deactivate();
    }

    active_events.resize( 0 );
  }

  void checkpoint_save() {
    // keep track of script state across checkpoints, so we can go back to it
    // when a checkpoint gets loaded to be in line with the game state at that
    // point; to prevent e.g. saving a checkpoint while attack is disabled, and
    // a minute later the checkpoint loads and the event will no longer be
    // active and able to be deactivated
    CHECKPOINT_active_events = active_events;
    CHECKPOINT_active_event_indexes = active_event_indexes;
    CHECKPOINT_iteration = iteration;
  }
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
