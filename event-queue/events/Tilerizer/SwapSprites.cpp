#include "../../../lib/Random.cpp";
#include "../../../lib/util/tiles.cpp";

#include "../../QueueEvent.cpp";

#include "./data.cpp";

class SwapSprites : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 800, 120, "Exec Func Ruin Level", "Beautify" );
  }

  scene@ g;
  dustman@ player;

  uint frames = 0;
  uint interval = 10;

  bool initialized = false;

  SwapSprites() {
    @g = get_scene();
  }

  void step( int entities ) {
    if ( frames % interval == 0 ) {
      if ( frames < 30 ) {
        replace_tiles( 19 );
      }
      else if ( frames < 40 ) {
        replace_tiles( 15 );
      }
      else if ( frames < 50 ) {
        replace_tiles( 14 );
      }
      else if ( frames < 60 ) {
        replace_tiles( 13 );
      }
      else if ( frames < 70 ) {
        replace_tiles( 11 );
      }
      else if ( frames < 80 ) {
        replace_tiles( 10 );
      }
      else if ( frames < 90 ) {
        replace_tiles( 9 );
      }
      else if ( frames < 100 ) {
        replace_tiles( 8 );
      }
      else if ( frames < 110 ) {
        replace_tiles( 7 );
      }
      else if ( frames <= 120 ) {
        replace_tiles( 6 );
      }
    }

    frames++;
  }

  void replace_tiles( uint layer ) {
    uint sprite_set = srand_range( 1, 4 );
    uint sprite_tile = srand_range( 1, 5 );

    for ( uint i = 0; i <= 50; i++ ) {
      int x = tile_coord( player.x() + tile( srand_range( -60, 60 ) ) );
      int y = tile_coord( player.y() + tile( srand_range( -20, 20 ) ) );

      // attempt to create "bowl" shapes in random places
      replace_tile( g, x, y, sprite_set, sprite_tile, -1, layer );
      replace_tile( g, x+1, y, sprite_set, sprite_tile, -1, layer );
      replace_tile( g, x+2, y, sprite_set, sprite_tile, -1, layer );
      replace_tile( g, x-1, y, sprite_set, sprite_tile, -1, layer );
      replace_tile( g, x-2, y, sprite_set, sprite_tile, -1, layer );

      replace_tile( g, x+1, y+1, sprite_set, sprite_tile, -1, layer );
      replace_tile( g, x-1, y+1, sprite_set, sprite_tile, -1, layer );

      replace_tile( g, x, y+1, sprite_set, sprite_tile, -1, layer );
      replace_tile( g, x, y+2, sprite_set, sprite_tile, -1, layer );
    }
  }

  void initialize() {
    if ( initialized ) {
      return;
    }

    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

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
