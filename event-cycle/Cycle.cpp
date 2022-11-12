#include "../lib/std.cpp";

#include "../lib/Random.cpp";

#include "./CycleEvent.cpp";

#include "./events/index.cpp";

class Cycle : Random {
  // is automatically set to true if any events are added to the below
  // DEBUG_events_override array
  bool DEBUG_MODE = false;
  // putting events in here will automatically give them a 100 weight,
  // guaranteeing them to be available to be picked every round (though still
  // not twice in a row, and retaining the existing limit of concurrent number
  // of events); these are all already imported
  array<CycleEvent@> DEBUG_events_override = {
  };

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

  array<CycleEvent@> @events = get_cycle_events();

  array<uint> active_event_indexes;

  array<CycleEventConfig> event_configs = {};
  array<textfield@> event_textfields;
  array<textfield@> event_subtextfields;

  // interval in seconds
  uint interval = 1;
  uint interval_min = 3;
  uint interval_max = 12;
  uint current_num_active_events;

  array<CycleEvent@> CHECKPOINT_events;
  array<uint> CHECKPOINT_active_event_indexes;
  uint CHECKPOINT_current_num_active_events;
  uint CHECKPOINT_iteration;

  bool turbo_mode = false;

  Cycle() {
    @g = get_scene();

    if ( DEBUG_events_override.length > 0 ) {
      // only pick DEBUG events when provided
      events = DEBUG_events_override;
      DEBUG_MODE = true;
    }

    for ( uint i = 0; i < events.length; i++ ) {
      CycleEventConfig config = events[ i ].get_config();

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

      for ( uint i = 0; i <= 6; i++ ) {
        textfield@ event_text = create_textfield();
        event_text.set_font( "Caracteres", 40 );
        event_text.align_horizontal( -1 );
        event_text.align_vertical( 0 );
        event_textfields.insertAt( i, event_text );

        textfield@ event_subtext = create_textfield();
        event_subtext.set_font( "Caracteres", 26 );
        event_subtext.align_horizontal( -1 );
        event_subtext.align_vertical( 0 );
        event_subtextfields.insertAt( i, event_subtext );
      }

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
      // just stop the cycle at this point, otherwise we might run into e.g.
      // issues with gathering the player controllable, or shenanigans during
      // loading
      return;
    }

    if ( player.filth_type() != 0 ) {
      // when character swapping to bosses we set filth type to 0, but other
      // events may have some effects that set the filth type indirectly, e.g.
      // character scaling; so just make sure the player always has 0 for now
      player.filth_type( 0 );
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
      array<uint> filtered_pool = filter_events();

      if ( active_event_indexes.length > 0 ) {
        // empty out the active event index list
        active_event_indexes.removeRange( 0, active_event_indexes.length );
      }

      // pick a range for the number of active events that is more weighted
      // towards 3 and 4 than any other
      current_num_active_events = pick_number_of_events();

      if ( current_num_active_events > filtered_pool.length ) {
        current_num_active_events = filtered_pool.length;
      }

      for ( uint i = 0; i < current_num_active_events; i++ ) {
        // randomly pick new events to activate
        uint index = filtered_pool[ srand_range( 0, filtered_pool.length - 1 ) ];

        while ( active_event_indexes.find( index ) >= 0 ) {
          // don't pick duplicate events
          index = filtered_pool[ srand_range( 0, filtered_pool.length - 1 ) ];
        }

        active_event_indexes.insertLast( index );

        // initialize the event
        events[ index ].initialize();

        CycleEventConfig config = event_configs[ index ];
        event_textfields[ i ].text( config.name );
        event_textfields[ i ].colour( config.colour );
        event_subtextfields[ i ].text( config.subtext );
        event_subtextfields[ i ].colour( config.colour );

        script.event_list.add_cycle_element( config.name, config.subtext );
      }

      if ( current_num_active_events == 0 ) {
        active_event = false;
        // never stay without any Events active for longer than 5 seconds
        interval = ( interval < 5 ) ? 5 : interval;
      }
      else {
        active_event = true;
      }
    }

    if ( checkpoint_loaded && ( CHECKPOINT_iteration != iteration ) ) {
      // reinitialize and deactivate all events that were active at the last
      // saved checkpoint, if we're past the iteration from that time;
      // reinitialization is necessary since script variables may have been
      // reset/lost
      for ( uint i = 0; i < CHECKPOINT_current_num_active_events; i++ ) {
        CHECKPOINT_events[ CHECKPOINT_active_event_indexes[ i ] ].initialize();
        CHECKPOINT_events[ CHECKPOINT_active_event_indexes[ i ] ].deactivate();
      }
    }

    if ( active_event ) {
      for ( uint i = 0; i < current_num_active_events; i++ ) {
        if ( checkpoint_loaded ) {
          // reinitialize the event after we loaded from a checkpoint, to make
          // sure the script variables aren't messed up
          events[ active_event_indexes[ i ] ].deactivate();
          events[ active_event_indexes[ i ] ].initialize();
        }

        events[ active_event_indexes[ i ] ].step( entities );
      }
    }

    if ( checkpoint_loaded ) {
      checkpoint_loaded = false;

      if ( ALL_BOSSES.find( player.character() ) >= 0 ) {
        // make sure not to spread dust as boss characters, as that often mostly
        // ruins any fun, as it means you just have to wait around and do
        // nothing until you get swapped to a different character for fear of
        // spreading dust
        player.filth_type( 0 );
      }
    }

    // always increment the time_since_last_pick, even if no Event is active,
    // because that would just mean that last time we didn't have any Events
    // available and we're just playing vanilla for <interval> seconds
    time_since_last_pick++;
  }

  array<uint> filter_events() {
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

    // TODO: get new class instances of the events each time
    // @events = get_cycle_events();

    array<uint> pool = {};
    for ( uint i = 0; i < events.length; i++ ) {
      if ( active_event_indexes.find( i ) >= 0 ) {
        // this event was already activated the previous time, so don't add it
        // to the pool; also deactivate this last event and remove it from the
        // ones marked as active
        events[ i ].deactivate();
        continue;
      }

      if ( event_configs[ i ].weight >= range ) {
        pool.insertLast( i );
      }
    }

    return pool;
  }

  uint pick_number_of_events() {
    // pick a range for the number of active events that is more weighted
    // towards 3 and 4

    int range = srand_range( 2, 6 );
    if ( range == 2 ) {
      return 2;
    }
    else if ( range <= 4 ) {
      return 3;
    }
    else if ( range <= 6 ) {
      return 4;
    }
    else {
      return 3;
    }
  }

  void checkpoint_save() {
    // keep track of script state across checkpoints, so we can go back to it
    // when a checkpoint gets loaded to be in line with the game state at that
    // point; to prevent e.g. saving a checkpoint while attack is disabled, and
    // a minute later the checkpoint loads and the event will no longer be
    // active and able to be deactivated
    CHECKPOINT_events = events;
    CHECKPOINT_active_event_indexes = active_event_indexes;
    CHECKPOINT_current_num_active_events = current_num_active_events;
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
    if ( !active_event ) {
      return;
    }

    for ( uint i = 0; i < current_num_active_events; i++ ) {
      // call the event's draw method
      events[ active_event_indexes[ i ] ].draw( sub_frame );
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
