#include "../../CycleEvent.cpp";

class JumpMacro : CycleEvent, callback_base {
  CycleEventConfig get_config() {
    return CycleEventConfig( 12, "Literally Unplayable", "Jump Macro" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  JumpMacro() {}

  void step( int entities ) {
    if ( player.ground() || check_nearby_wall() || ( player.state() >= 11 && player.state() <= 14 ) ) {
      player.jump_intent( 1 );
    }
  }

  bool check_nearby_wall() {
    int x = tile_coord( player.x() + 24 );
    int x2 = tile_coord( player.x() - 24 );
    tileinfo@ ti_right = g.get_tile( x, tile_coord( player.y() - 58 ), 19 );
    tileinfo@ ti_left = g.get_tile( x2, tile_coord( player.y() - 58 ), 19 );

    if ( ( ti_right.solid() && ti_right.type() == 0 ) || ( ti_left.solid() && ti_left.type() == 0 ) ) {
      return true;
    }

    return false;
  }

  void on_subframe_end( dustman@ dm, int ) {
    if ( dm.state() == 10 ) {
      dm.state( 8 );
    }
  }

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


    player.on_subframe_end_callback( this, "on_subframe_end", 0 );

    initialized = true;
  }
}
