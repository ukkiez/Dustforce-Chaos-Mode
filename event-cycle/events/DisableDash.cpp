#include "../CycleEvent.cpp";

class DisableDash : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 8, "Dash Disabled" );
  }

  scene@ g;

  bool initialized = false;

  DisableDash() {}

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    g.dash_enabled( true );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    g.dash_enabled( false );

    initialized = false;
  }
}
