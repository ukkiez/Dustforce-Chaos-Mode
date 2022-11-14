#include "../CycleEvent.cpp";

class Big : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 45, "Beeg Man Mode" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  Big() {}

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

    player.scale( 1.25, false );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    player.scale( 1, false );

    initialized = false;
  }
}
