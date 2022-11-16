#include "../../../lib/Random.cpp";
#include "../../../lib/util/tiles.cpp";

#include "../../QueueEvent.cpp";

#include "./data.cpp";

class SwapSprites : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 80, 0, "Exec Func Ruin Level", "Beautify" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  SwapSprites() {
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

    uint sprite_set = srand_range( 1, 4 );
    uint sprite_tile = srand_range( 1, 5 );

    for ( uint i = 0; i <= 50; i++ ) {
      int x = tile_coord( player.x() + tile( srand_range( -60, 60 ) ) );
      int y = tile_coord( player.y() + tile( srand_range( -20, 20 ) ) );

      // attempt to create "bowl" shapes in random places
      replace_tile( g, x, y, sprite_set, sprite_tile );
      replace_tile( g, x+1, y, sprite_set, sprite_tile );
      replace_tile( g, x+2, y, sprite_set, sprite_tile );
      replace_tile( g, x-1, y, sprite_set, sprite_tile );
      replace_tile( g, x-2, y, sprite_set, sprite_tile );

      replace_tile( g, x+1, y+1, sprite_set, sprite_tile );
      replace_tile( g, x-1, y+1, sprite_set, sprite_tile );

      replace_tile( g, x, y+1, sprite_set, sprite_tile );
      replace_tile( g, x, y+2, sprite_set, sprite_tile );
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
