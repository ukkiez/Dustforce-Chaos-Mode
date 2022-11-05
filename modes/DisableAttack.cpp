#include "./Mode.cpp";

class DisableAttack : Mode {
  ModeConfig get_mode_config() {
    return ModeConfig( 10, "Attack Disabled" );
  }

  scene@ g;

  bool initialized = false;

  DisableAttack() {}

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    g.attack_enabled( true );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    g.attack_enabled( false );

    initialized = false;
  }
}
