#include "../../CycleEvent.cpp";

class Runner : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 225, "Unplayable", "Runner" );
  }

  scene@ g;
  dustman@ player;

  uint FACTOR = 4;
  uint current_state = 0;
  bool speed_set = false;

  bool initialized = false;

  Runner() {}

  void step( int entities ) {
    // if player is in states "run", "wall_run" or "roof_run"
    if ( player.state() == 2 || player.state() == 11 || player.state() == 17 ) {
      if ( speed_set ) {
        return;
      }

      player.set_speed_xy( player.x_speed() * FACTOR, player.y_speed() * FACTOR );
      speed_set = true;
    }
    else {
      speed_set = false;
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
