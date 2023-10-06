#include "../QueueEvent.cpp";

class CloseUp : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 200, 0, "Close-up" );
  }

  scene@ g;
  camera@ cam;
  dustman@ player;

  bool initialized = false;

  int screen_height = 400;

  CloseUp() {}

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
    camera_node@ current_node = cam.current_node();

    if ( @current_node != null ) {
      if ( current_node.type() == 2 ) {
        cam.screen_height( screen_height );
      }
      current_node.screen_height( screen_height );

      camera_node@ next_node = cam.next_node();
      if ( @next_node != null ) {
        next_node.screen_height( screen_height );
      }
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
