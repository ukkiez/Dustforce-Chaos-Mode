#include "./Mode.cpp";

#include "../lib/Random.cpp";

class TimeWarp : Mode, Random {
  ModeConfig get_mode_config() {
    return ModeConfig( 50, "Time Warp", ".5x speed" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  TimeWarp() {}

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

    srandom() % 2 == 0 ? player.time_warp( 0.5 ) : player.time_warp( 1.5 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    player.time_warp( 1 );

    initialized = false;
  }
}
