#include "./Mode.cpp";

#include "../lib/Random.cpp"

class Tiny : Mode, Random {
  ModeConfig get_mode_config() {
    return ModeConfig( 50, "Tiny Man Mode" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  Tiny() {}

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

    player.scale( srand_range_flt( 0.5, 0.8 ), false );

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
