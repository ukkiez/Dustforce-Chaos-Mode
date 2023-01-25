#include "../../CycleEvent.cpp";

class DashForever : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 100, "Literally Unplayable", "Dash Forever (bye)" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  DashForever() {}

  void step( int entities ) {
    if ( player.state() == 9 ) {
      player.state_timer( 0 );
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
