#include "../../lib/Random.cpp";
#include "../../lib/util/tiles.cpp";

#include "../Mode.cpp";

class SpawnZip : Mode, Random {
  ModeConfig get_mode_config() {
    return ModeConfig( 60, "Exec Func Ruin Level", "Surprise Zip" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  SpawnZip() {
    @g = get_scene();
  }

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

    zip();

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }

  void zip() {
    bool spawn_right = ( player.x_intent() == 1 );
    if ( player.x_intent() == 0 ) {
      spawn_right = player.face() == 1 ? true : false;
    }

    int type = spawn_right ? 19 : 18;
    uint sprite_set = srand_range( 1, 4 );
    uint sprite_tile = srand_range( 1, 5 );
    for ( int i = 0; i <= 16; i++ ) {
      // spawn a vertical zip in front of the player
      int x = tile( 2 );
      if ( !spawn_right ) {
        x = -x;
      }
      int y = tile( i - 6 );

      g.set_tile(
        tile_coord( player.x() + x ),
        tile_coord( player.y() - y ), // y
        19, // layer
        true, // Solid
        type, // Type (i.e. tile shape)
        sprite_set, // spriteSet
        sprite_tile, // spriteTile
        1 // palette
      );

      g.set_tile_filth(
        tile_coord( player.x() + x ),
        tile_coord( player.y() - y ),
        0, // top
        0, // bottom
        0, // left
        0, // right
        false, // affectSpikes
        true // overwrite
      );
    }
  }
}
