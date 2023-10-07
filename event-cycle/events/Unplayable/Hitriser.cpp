#include "../../CycleEvent.cpp";

class Hitriser : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 180, "Hitriser" );
  }

  scene@ g;
  dustman@ player;

  float original_hitrise_speed;

  bool initialized = false;

  Hitriser() {}

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

    original_hitrise_speed = player.hitrise_speed();
    player.hitrise_speed( srand_range_flt( -500, -5000 ) );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    player.hitrise_speed( original_hitrise_speed );

    initialized = false;
  }
}
