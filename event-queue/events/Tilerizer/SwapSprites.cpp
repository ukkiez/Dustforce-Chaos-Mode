#include "../../../lib/Random.cpp";
#include "../../../lib/util/tiles.cpp";

#include "../../QueueEvent.cpp";

#include "./data.cpp";

#include "../../../lib/data/SpriteSet.cpp";

class SwapSprites : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 800, 120, "Exec Func Ruin Level", "Beautify" );
  }

  scene@ g;
  dustman@ player;

  script@ _script;

  uint frames = 0;
  uint interval = 10;

  bool done = false;

  bool initialized = false;

  SwapSprites() {
    @g = get_scene();
  }

  void step( int entities ) {
    if ( done ) {
      return;
    }

    if ( _script.turbo_mode ) {
      // during turbo mode, simply replace tiles in a random layer, just once so
      // as not to cause (too many) performance issues
      replace_tiles( srand_range( 6, 19 ) );
      done = true;
    }

    if ( frames % interval == 0 ) {
      if ( frames < 30 ) {
        replace_tiles( 19 );
      }
      else if ( frames < 40 ) {
        replace_tiles( 15 );
      }
      else if ( frames < 50 ) {
        replace_tiles( 14 );
        replace_tiles( 19, 25 );
      }
      else if ( frames < 60 ) {
        replace_tiles( 13 );
      }
      else if ( frames < 70 ) {
        replace_tiles( 11 );
        replace_tiles( 19, 25 );
      }
      else if ( frames < 80 ) {
        replace_tiles( 10 );
      }
      else if ( frames < 90 ) {
        replace_tiles( 9 );
        replace_tiles( 19, 25 );
      }
      else if ( frames < 100 ) {
        replace_tiles( 8 );
      }
      else if ( frames < 110 ) {
        replace_tiles( 7 );
        replace_tiles( 19, 25 );
      }
      else if ( frames <= 120 ) {
        replace_tiles( 6 );
      }
    }

    frames++;
  }

  void replace_tiles( uint layer, uint iterations = 50 ) {
    // choose a random sprite set; keep in mind they are 1-indexed
    SpriteSet@ sprite_set = SPRITE_SETS[ srand_range( 0, SPRITE_SETS.length - 2 ) ];

    // and a random tile from the set (also 1-indexed)
    int sprite_tile = srand_range( 1, sprite_set.size );

    for ( uint i = 0; i <= iterations; i++ ) {
      int x = tile_coord( player.x() + tile( srand_range( -60, 60 ) ) );
      int y = tile_coord( player.y() + tile( srand_range( -20, 20 ) ) );

      // attempt to create "bowl" shapes in random places
      replace_tile( g, x, y, sprite_set.index, sprite_tile, -1, layer );
      replace_tile( g, x+1, y, sprite_set.index, sprite_tile, -1, layer );
      replace_tile( g, x+2, y, sprite_set.index, sprite_tile, -1, layer );
      replace_tile( g, x-1, y, sprite_set.index, sprite_tile, -1, layer );
      replace_tile( g, x-2, y, sprite_set.index, sprite_tile, -1, layer );

      replace_tile( g, x+1, y+1, sprite_set.index, sprite_tile, -1, layer );
      replace_tile( g, x-1, y+1, sprite_set.index, sprite_tile, -1, layer );

      replace_tile( g, x, y+1, sprite_set.index, sprite_tile, -1, layer );
      replace_tile( g, x, y+2, sprite_set.index, sprite_tile, -1, layer );
    }
  }

  void initialize() {
    if ( initialized ) {
      return;
    }

    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

    // get the main script object
    @_script = cast<script@>( get_script() );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    frames = 0;
    done = false;

    initialized = false;
  }
}
