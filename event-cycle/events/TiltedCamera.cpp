#include "../../lib/Random.cpp";

#include "../CycleEvent.cpp";

class TiltedCamera : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 100, "Tilted" );
  }

  scene@ g;
  camera@ cam;
  dustman@ player;

  bool ccw = false;

  int frames = 0;

  bool initialized = false;

  TiltedCamera() {}

  void step( int entities ) {
    if ( @cam == null ) {
      return;
    }

    if ( frames % 2 == 0 && ( cam.rotation() < 5 &&  cam.rotation() > -5 ) ) {
      cam.rotation( cam.rotation() + ( ccw ? -1 : 1 ) );
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

    @cam = get_active_camera();
    ccw = ( srandom() % 2 == 0 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    cam.rotation( 0 );
    frames = 0;

    initialized = false;
  }
}
