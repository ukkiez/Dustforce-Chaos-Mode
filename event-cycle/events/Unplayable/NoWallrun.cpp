#include "../../CycleEvent.cpp";

class NoWallrun : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 160, "Unplayable", "No Wallrun" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  NoWallrun() {}

  void step( int entities ) {
    if ( player.state() == 11 ) {
      player.state( 13 );
    }
  }

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

    initialized = false;
  }
}
