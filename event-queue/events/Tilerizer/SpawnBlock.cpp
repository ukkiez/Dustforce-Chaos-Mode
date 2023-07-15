#include "../../../lib/util/tiles.cpp";

#include "../../QueueEvent.cpp";

class SpawnBlock : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 650, 0, "Exec Func Ruin Level", "Surprise bloche" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  SpawnBlock() {
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

    block_on_player();

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }

  void block_on_player() {
    uint sprite_set = srand_range( 1, 4 );
    uint sprite_tile = srand_range( 1, 5 );
    for ( int i = 0; i <= 3; i++ ) {
      g.set_tile(
        tile_coord( player.x() + tile( i - 1 ) ), // x
        tile_coord( player.y() - tile( 2 ) ), // y
        19, // layer
        true, // Solid
        0, // Type (i.e. tile shape)
        sprite_set, // spriteSet
        sprite_tile, // spriteTile
        1 // palette
      );
      g.set_tile(
        tile_coord( player.x() + tile( i - 1 ) ), // x
        tile_coord( player.y() - tile( 1 ) ), // y
        19, // layer
        true, // Solid
        0, // Type (i.e. tile shape)
        sprite_set, // spriteSet
        sprite_tile, // spriteTile
        1 // palette
      );
      g.set_tile(
        tile_coord( player.x() + tile( i - 1 ) ), // x
        tile_coord( player.y() ), // y
        19, // layer
        true, // Solid
        0, // Type (i.e. tile shape)
        sprite_set, // spriteSet
        sprite_tile, // spriteTile
        1 // palette
      );
      g.set_tile(
        tile_coord( player.x() + tile( i - 1 ) ), // x
        tile_coord( player.y() + tile( 1 ) ), // y
        19, // layer
        true, // Solid
        0, // Type (i.e. tile shape)
        sprite_set, // spriteSet
        sprite_tile, // spriteTile
        1 // palette
      );
    }
  }
}
