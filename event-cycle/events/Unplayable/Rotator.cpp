#include "../../CycleEvent.cpp";

class Rotator : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 200, "Unplayable", "Rotator" );
  }

  scene@ g;
  controllable@ player;

  bool initialized = false;

  int time = 0;

  Rotator() {}

  void step( int entities ) {
    player.rotation( 90 );

    if ( time % 20 == 0 ) {
      player.face( -player.face() );
      player.attack_face( -player.attack_face() );
    }

    time++;
  }
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    @player = controller_controllable( 0 );
    if ( @player == null ) {
      return;
    }

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    time = 0;

    initialized = false;
  }
}
