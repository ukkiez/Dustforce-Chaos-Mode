#include "../CycleEvent.cpp";

#include "../../lib/util/tiles.cpp";
#include "../../lib/data/SpriteSet.cpp";

// swaps tile sprites close to the player
class Decorator : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 700, "Decorator" );
  }

  scene@ g;
  dustman@ player;

  uint frames = 0;
  uint interval = 7;

  bool initialized = false;

  Decorator() {
    @g = get_scene();
  }

  void step( int entities ) {
    if ( frames % interval == 0 ) {
      // if the player is on the ground, wall running, or ceiling running,
      // replace tiles around the player (in different places, depending on the
      // action)
      if ( player.ground() ) {
        replace_tiles( 19 );
      }

      if ( player.roof() ) {
        replace_tiles( 19, -3 );
      }

      if ( player.wall_left() || player.wall_right() ) {
        replace_tiles( 19, -1 );
      }
    }

    frames++;
  }

  void replace_tiles( uint layer, int additional_y = 0 ) {
    // choose a random sprite set; keep in mind they are 1-indexed
    SpriteSet@ sprite_set = SPRITE_SETS[ srand_range( 0, SPRITE_SETS.length - 2 ) ];

    // and a random tile from the set (also 1-indexed)
    int sprite_tile = srand_range( 1, sprite_set.size );

    int x = tile_coord( player.x() );
    int y = tile_coord( player.y() );

    for ( int i = -1; i <= 1; i++ ) {
      int _x = x + i;
      // below player
      replace_tile( g, _x, y, sprite_set.index, sprite_tile, -1, layer );

      // above player
      if ( additional_y != 0 ) {
        replace_tile( g, _x, y + additional_y, sprite_set.index, sprite_tile, -1, layer );
      }
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

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    frames = 0;

    initialized = false;
  }
}
