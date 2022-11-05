#include "./Mode.cpp";

class Big : Mode {
  ModeConfig get_mode_config() {
    return ModeConfig( 50, "Beeg Man Mode" );
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

    player.scale( 1.5, false );

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
