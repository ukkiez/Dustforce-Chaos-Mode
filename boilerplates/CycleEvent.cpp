#include "../CycleEvent.cpp";

class Example : CycleEvent {
  CycleEventConfig get_event_config() {
    return CycleEventConfig( 50, "CycleEvent Name" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  Example() {}

  void step( int entities ) {}
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

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
