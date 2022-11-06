#include "../Mode.cpp";

class Tilted : Mode, callback_base {
  ModeConfig get_mode_config() {
    return ModeConfig( 25, "Literally Unplayable", "Tilted" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  Tilted() {}

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

    // Set up custom collision handling functions
    player.set_collision_handler( this, "collision_handler", 0 );

    // Slope min/max, slant min/max angles.
    // Required for non-45deg slope sliding
    c.set_ground_angles( 0, 50, 0, 0 );

    initialized = true;
  }

  void collision_handler( controllable@ c, tilecollision@ t, int side, bool moving, float snap_offset, int arg ) {
    if ( side == 3 ) {
      c.check_collision( t, side, moving, snap_offset );
      return;
    }

    bool wallrun = c.check_collision( t, side, false, snap_offset );
    if ( wallrun ) {
      c.ground( true );
    }
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    // Set up custom collision handling functions
    player.set_collision_handler( null, "collision_handler", 0 );

    player.set_ground_angles( 40, 50, 10, 40 );

    initialized = false;
  }
}
