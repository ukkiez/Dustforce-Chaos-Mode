#include "../QueueEvent.cpp";

class ForceSuper : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 150, 200, "Super Time" );
  }

  scene@ g;
  camera@ cam;
  dustman@ player;

  bool initialized = false;

  int frames = 0;

  ForceSuper() {}

  void step( int entities ) {
    if ( @cam == null ) {
      return;
    }

    if ( frames >= 119 ) {
      cam.script_camera( false );
    }

    if ( frames <= 50 ) {
      if ( frames % 10 == 0 ) {
        player.attack_state( 3 );
      }
    }

    frames++;
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

    @cam = get_active_camera();

    cam.script_camera( true );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
