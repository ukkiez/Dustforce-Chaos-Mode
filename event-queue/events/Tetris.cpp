#include "../../lib/util/tiles.cpp";

#include "../QueueEvent.cpp";

// configurations define where the tiles need to be placed relative to the
// original
const array<array<Coordinate@>> configurations = {
  // |
  // |
  // |_
  array<Coordinate@> = {
    Coordinate( 0, 0, true ),
    Coordinate( 1, 0 ),
    Coordinate( 0, -1, true ),
    Coordinate( 0, -2 ),
  },
  // |___
  array<Coordinate@> = {
    Coordinate( 0, 0, true ),
    Coordinate( 1, 0 ),
    Coordinate( 2, 0 ),
    Coordinate( 0, -1 ),
  },
  // ___|
  array<Coordinate@> = {
    Coordinate( 0, 0 ),
    Coordinate( 1, 0 ),
    Coordinate( 2, 0, true ),
    Coordinate( 2, -1 ),
  },
  // |-
  // |
  // |
  array<Coordinate@> = {
    Coordinate( 0, 0, true ),
    Coordinate( 0, -1, true ),
    Coordinate( 0, -2 ),
    Coordinate( 1, -2 ),
  },

  // |
  // |
  // |
  // |
  array<Coordinate@> = {
    Coordinate( 0, 0, true ),
    Coordinate( 0, -1, true ),
    Coordinate( 0, -2, true ),
    Coordinate( 0, -3 ),
  },
  // ____
  array<Coordinate@> = {
    Coordinate( 0, 0 ),
    Coordinate( 1, 0 ),
    Coordinate( 2, 0 ),
    Coordinate( 3, 0 ),
  },

  // []
  array<Coordinate@> = {
    Coordinate( 0, 0, true ),
    Coordinate( 0, -1 ),
    Coordinate( 1, 0, true ),
    Coordinate( 1, -1 ),
  },

  //  --
  // --
  array<Coordinate@> = {
    Coordinate( 0, 0 ),
    Coordinate( 1, 0, true ),
    Coordinate( 1, -1 ),
    Coordinate( 2, -1 ),
  },

  // --
  //  --
  array<Coordinate@> = {
    Coordinate( 0, 0 ),
    Coordinate( 1, 0 ),
    Coordinate( 1, 1, true ),
    Coordinate( 2, 1 ),
  },

  //  __|
  // |
  array<Coordinate@> = {
    Coordinate( 0, 0, true ),
    Coordinate( 0, -1 ),
    Coordinate( 1, -1, true ),
    Coordinate( 1, -2 ),
  },
  // |__
  //   |
  array<Coordinate@> = {
    Coordinate( 0, 0 ),
    Coordinate( 0, 1, true ),
    Coordinate( 1, 1 ),
    Coordinate( 1, 2, true ),
  },

  // _|_
  array<Coordinate@> = {
    Coordinate( 0, 0 ),
    Coordinate( 1, 0, true ),
    Coordinate( 1, -1 ),
    Coordinate( 2, 0 ),
  },
  // -|-
  array<Coordinate@> = {
    Coordinate( 0, 0 ),
    Coordinate( 1, 0 ),
    Coordinate( 1, 1, true ),
    Coordinate( 2, 0 ),
  },
};

class Tile {
  int x;
  int y;
  bool has_tile_above = false;
  Tile( int x, int y, bool has_tile_above = false ) {
    this.x = x;
    this.y = y;
    this.has_tile_above = has_tile_above;
  }
}

class Coordinate {
  int x;
  int y;
  bool has_tile_above = false;
  Coordinate( int x, int y, bool has_tile_above = false ) {
    this.x = x;
    this.y = y;
    this.has_tile_above = has_tile_above;
  }
}

class Tetromino {
  array<Tile@> tiles = {};

  bool spawned = false;
  bool landed = false;

  Tetromino( int x, int y, int configuration_index ) {
    for ( uint i = 0; i <= 3; i++ ) {
      Coordinate@ coord = configurations[ configuration_index ][ i ];
      tiles.insertLast(
        Tile(
          x + tile_coord( tile( coord.x ) ), y + tile_coord( tile( coord.y ) ), coord.has_tile_above
        )
      );
    }
  }
}

class Tetris : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 300, 600, "Tetris" );
  }

  scene@ g;
  dustman@ player;

  array<Tetromino@> tetrominoes = {};

  uint sprite_set = srand_range( 1, 4 );
  uint sprite_tile = srand_range( 1, 5 );

  uint frames = 0;

  uint interval = 30;

  bool initialized = false;

  Tetris() {}

  void step( int entities ) {
    if ( tetrominoes.length == 0 ) {
      generate_blocks();
    }

    if ( frames % interval == 0 ) {
      move_blocks();
    }

    frames++;
  }

  void generate_blocks() {
    // generate a set of tetrominoes somewhere near the player

    tetrominoes.insertLast(
      Tetromino(
        tile_coord( player.x() + ( tile( srand_range( -20, 20 ) ) ) ),
        tile_coord( player.y() - tile( srand_range( 5, 15 ) ) ),
        srand_range( 0, int( configurations.length - 1 ) )
      )
    );
  }

  void move_blocks() {
    for ( uint i = 0; i < tetrominoes.length; i++ ) {
      Tetromino@ block = tetrominoes[ i ];

      for ( uint j = 0; j < block.tiles.length; j++ ) {
        Tile@ _tile = block.tiles[ j ];
        if ( block.spawned && !_tile.has_tile_above ) {
          // remove the tile where this block previously was, so we can move it
          // down and simulate vertical movement
          g.set_tile(
            _tile.x,
            _tile.y,
            19, // layer
            false, // Solid
            0, // Type (i.e. tile shape)
            sprite_set, // spriteSet
            sprite_tile, // spriteTile
            1 // palette
          );
        }

        if ( block.spawned ) {
          // move the block down one tile
          _tile.y += tile_coord( tile( 1 ) );
        }

        g.set_tile(
          _tile.x,
          _tile.y,
          19, // layer
          true, // Solid
          0, // Type (i.e. tile shape)
          sprite_set, // spriteSet
          sprite_tile, // spriteTile
          1 // palette
        );

        remove_tile_filth( g, _tile.x, _tile.y );
      }

      if ( !block.spawned ) {
        block.spawned = true;
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

    initialized = false;
  }
}
