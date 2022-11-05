#include "./Mode.cpp";

class MaxSuper : Mode {
  ModeConfig get_mode_config() {
    return ModeConfig( 50, "Max Super" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  MaxSuper() {}

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

      if ( player.skill_combo() < 120 ) {
        player.skill_combo( 120 );
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
