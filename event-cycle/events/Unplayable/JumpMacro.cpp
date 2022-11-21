#include "../../CycleEvent.cpp";

class JumpMacro : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 12, "Literally Unplayable", "Jump Macro" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  JumpMacro() {}

  void step( int entities ) {
    if ( player.ground() || ( player.state() >= 11 && player.state() <= 14 ) ) {
      player.jump_intent( 1 );
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
}
