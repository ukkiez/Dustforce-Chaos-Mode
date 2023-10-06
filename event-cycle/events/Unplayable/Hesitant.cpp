#include "../../CycleEvent.cpp";

class Hesitant : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 180, "Hesitant" );
  }

  scene@ g;
  dustman@ player;


  bool initialized = false;

  Hesitant() {}

  void step( int entities ) {
    if ( player.state() == 2 || player.state() == 17 ) {
    // if player is in states "run" or "roof_run" stop them in their tracks, or
    // get them off of the ceiling
      player.set_speed_xy( player.speed(), player.speed() );
    }

    // if player is in state "dash"
    if ( player.state() == 9 ) {
      player.set_speed_xy( 0, 0 );
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
