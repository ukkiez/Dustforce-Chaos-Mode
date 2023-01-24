#include "../../CycleEvent.cpp";

class HalfDash : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 130, "HD", "(half dash)" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  HalfDash() {}

  void step( int entities ) {
    if ( player.state() == 9 ) {
      if ( player.state_timer() < 0.5 ) {
        player.state_timer( player.state_timer() + 2.5 );
      }
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
