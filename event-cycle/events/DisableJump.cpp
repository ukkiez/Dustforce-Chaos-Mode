#include "../CycleEvent.cpp";

class DisableJump : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 8, "Jump Disabled" );
  }

  scene@ g;

  bool initialized = false;

  DisableJump() {}

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    g.jump_enabled( true );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    g.jump_enabled( false );

    initialized = false;
  }
}
