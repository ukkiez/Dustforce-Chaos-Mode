#include "../lib/Random.cpp";

#include "./BuilderMode.cpp";

#include "./events/index.cpp";

class NexusChaos : Random {
  script@ _script;
  scene@ g;
  dustman@ player;

  bool DEBUG_MODE = false;

  NexusBuilderMode@ builder_mode;

  array<NexusEvent@> @events;
  NexusEvent@ nexus_event;

  bool chose_to_activate = false;

  bool initialized = false;

  array<string> NO_DISPLAY_NAME_CHANGE_DOORS = {
    "newtutorial1",
    "newtutorial2",
    "newtutorial3",
  };
  array<string> NO_DISPLAY_NAME_CHANGE_NEXUSES = {
    "Single Player Nexus",
  };

  NexusChaos() {}

  void init() {
    @g = get_scene();

    controllable@ c = controller_controllable( 0 );
    if ( @c == null ) {
      return;
    }

    @player = c.as_dustman();
    if ( @player == null ) {
      return;
    }

    @builder_mode = NexusBuilderMode();
    builder_mode.init();

    // get the main script object
    @_script = cast<script@>( get_script() );

    initialized = true;
  }

  void checkpoint_load() {
    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }

    @builder_mode = NexusBuilderMode();
    builder_mode.init();
  }

  void step( int entities ) {
    if ( builder_mode.initialized ) {
      builder_mode.step( entities );
    }

    if ( !chose_to_activate && @nexus_event == null ) {
      if ( NO_DISPLAY_NAME_CHANGE_NEXUSES.find( _script.g.map_name() ) < 0 ) {
        // don't change the display name of doors in certain nexuses, like the
        // main Single Player Nexus, to make full-game runs a bit less annoying
        // and easier to understand

        for ( int i = 0; i < entities; i++ ) {
          entity@ e = entity_by_index( i );
          if ( @e == null ) {
            continue;
          }

          if ( e.type_name() == "level_door" ) {
            if ( NO_DISPLAY_NAME_CHANGE_DOORS.find( e.vars().get_var( "file_name" ).get_string() ) >= 0 ) {
              // don't change the display name of certain doors, like the
              // tutorials (mostly to avoid confusion/annoyance)
              continue;
            }

            e.vars().get_var( "display_name" ).set_string( "CHAOS" );
          }
        }
      }

      @events = get_nexus_events( DEBUG_MODE );

      srand( timestamp_now() );
      int rand_n = rand_range( 1, 4 );
      puts( rand_n );
      if ( DEBUG_MODE || rand_n > 1 ) {
        srand( timestamp_now() );
        // activate a random nexus event, 75% of the time
        @nexus_event = events[ rand_range( 0, events.length - 1 ) ];
      }

      chose_to_activate = true;
    }

    if ( @nexus_event != null ) {
      nexus_event.step( entities );
    }
  }
  void draw( float sub_frame ) {
    if ( builder_mode.initialized ) {
      builder_mode.draw( sub_frame );
    }

    if ( @nexus_event != null ) {
      nexus_event.draw( sub_frame );
    }
  }
}
