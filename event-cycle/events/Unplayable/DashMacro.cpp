#include "../../CycleEvent.cpp";

class DashMacro : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 120, "Literally Unplayable", "Dash Macro" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  int frames = 0;

  DashMacro() {}

  void step( int entities ) {
    if ( player.y_intent() != 1 || player.ground() ) {
      if ( frames % 12 == 0 ) {
        player.state( 9 );
      }
    }
    else if ( frames % 1 == 0 ) {
      player.fall_intent( 1 );
    }

    if ( frames % 12 == 11 && player.x_intent() != 0 ) {
      player.face( player.x_intent() );
    }

    frames++;
  }
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }

    if ( @player == null ) {
      return;
    }

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    frames = 0;

    initialized = false;
  }
}
