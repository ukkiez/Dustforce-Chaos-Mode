#include "../lib/std.cpp";

#include "../lib/Random.cpp";

#include "./CycleEvent.cpp";

#include "./events/index.cpp";

class Cycle : Random {
	script@ script;
  scene@ g;
  dustman@ player;

  bool initialized = false;

  // keep track of which iteration we're on currently, to help deal with
  // checkpoints
  uint iteration = 0;

  // duration that current events are active (in step() frames)
  uint duration = 0;

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

  // is automatically set to true if any events are added to the below
  // DEBUG_events_override array
  bool DEBUG_MODE = false;
  // putting events in here will automatically give them a 100 weight,
  // guaranteeing them to be available to be picked every round (though still
  // not twice in a row, and retaining the existing limit of concurrent number
  // of events); these are all already imported
  array<CycleEvent@> DEBUG_events_override = {
  };

  array<uint> position_history( 5 );

  Cycle() {
    // get the main script object so we can access its properties
    @script = cast<script@>( get_script() );

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
      // just stop the cycle at this point, otherwise we might run into issues
      // with e.g. gathering the player controllable, or shenanigans during
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
    if ( duration % ( interval * 60 ) == 0 ) {
      duration = 0;
      iteration++;

      // choose after how many seconds we will pick the next set of Events after
      // this one
      interval = srand_range( interval_min, interval_max );

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

    // always increment the duration, even if no Event is active, because that
    // would just mean that last time we didn't have any Events available and
    // we're just playing vanilla for <interval> seconds
    duration++;
  }


  array<uint> filter_events() {
    // generate a range to exclude events based on their weight, e.g. a rolled
    // range of 50 means all weights below 50 should not be included in the pool
    uint range = srand_range( 1, 100 );

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
    // towards 3 and 4 than any other number

    int range = srand_range( 2, 12 );
    if ( range == 2 ) {
      return 2;
    }
    else if ( range <= 6 ) {
      return 3;
    }
    else if ( range <= 11 ) {
      return 4;
    }
    else if ( range <= 12 ) {
      return 5;
    }
    else {
      return 4;
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

    int default_x = -750;
    int y_offset = 0;
    int subtext_y_offset = 40;

    if ( duration < 7 ) {
      default_x -= uint( 300 - ( duration * 30 ) );
    }

    int y_gap = 100;
    int starting_point = ( -( y_gap / 2 ) * ( current_num_active_events ) );

    for ( uint i = 0; i < current_num_active_events; i++ ) {
      // call the event's draw method itself
      events[ active_event_indexes[ i ] ].draw( sub_frame );

      int x = default_x;
      int y = ( starting_point + ( y_gap * i ) );

      // attempt to dynamically spread out the event text across the screen, so
      // that they don't overlap, depending on how many are active
      draw_text( event_textfields[ i ], x, y, 1, 1, 0 );
      if ( event_subtextfields[ i ].text() != "" ) {
        draw_text( event_subtextfields[ i ], x, y + subtext_y_offset, 1, 1, 0 );
      }
    }
  }

  void draw_text( textfield@ text, float x, float y, float scale_x = 1.0, float scale_y = 1.0, float rotation = 0.0 ) {
    if ( text.text() != "" ) {
      // draw the text on layer 0, sublayer 0
      text.draw_hud( 22, 22, x, y, scale_x, scale_y, rotation % 360 );
    }
  }
}
