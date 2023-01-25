#include "../../CycleEvent.cpp";

class SupersDontHitEnemies : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 130, "Literally Unplayable", "Cool Super" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  SupersDontHitEnemies() {}

  void step( int entities ) {
    if ( player.attack_state() == 3 ) {
      player.team( 0 );
    }
    else if ( player.team() != 1 ) {
      player.team( 1 );
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

    if ( player.team() != 1 ) {
      player.team( 1 );
    }

    initialized = false;
  }
}
