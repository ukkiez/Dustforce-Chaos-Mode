#include "../lib/Random.cpp";

#include "./BuilderMode.cpp";

#include "./events/index.cpp";

class NexusChaos : Random {
  scene@ g;
  dustman@ player;

  bool DEBUG_MODE = false;

  NexusBuilderMode@ builder_mode;

  array<NexusEvent@> @events;
  NexusEvent@ nexus_event;

  bool initialized = false;

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

    if ( @nexus_event == null ) {
      for ( int i = 0; i < entities; i++ ) {
        entity@ e = entity_by_index( i );
        if ( @e == null ) {
          continue;
        }

        if ( e.type_name() == "level_door" ) {
          e.vars().get_var( "display_name" ).set_string( "CHAOS" );
        }
      }

      @events = get_nexus_events( DEBUG_MODE );

      srand( timestamp_now() );
      @nexus_event = events[ rand_range( 0, events.length - 1 ) ];
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
