#include "../../QueueEvent.cpp";
#include "../../../lib/util/tiles.cpp";

class RemoveTiles : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 250, 0, "Tilerizer", "Remove Fun" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  RemoveTiles() {}

  void remove_tiles() {
    uint count = 0;
    uint max = 60;
    uint search_range = 100;
    for ( uint i = 0; i <= search_range; i++ ) {
      uint x_tile = srand_range( 0, 25 );
      int x = tile_coord( player.x() + tile( x_tile ) );
      int y = tile_coord( player.y() + tile( i ) );

      if ( remove_tile( x, y ) ) {
        remove_tile_filth( g, x, y );
        count++;

        if ( count >= max ) {
          count = 0;
          break;
        }
      }
    }

    for ( uint i = 0; i <= search_range; i++ ) {
      uint x_tile = srand_range( 0, 25 );
      int x = tile_coord( player.x() + tile( x_tile ) );
      int y = tile_coord( player.y() - tile( i ) );

      if ( remove_tile( x, y ) ) {
        remove_tile_filth( g, x, y );
        count++;

        if ( count >= max ) {
          count = 0;
          break;
        }
      }
    }

    for ( uint i = 0; i <= search_range; i++ ) {
      uint x_tile = srand_range( 0, 25 );
      int x = tile_coord( player.x() - tile( x_tile ) );
      int y = tile_coord( player.y() - tile( i ) );

      if ( remove_tile( x, y ) ) {
        remove_tile_filth( g, x, y );
        count++;

        if ( count >= max ) {
          count = 0;
          break;
        }
      }
    }

    for ( uint i = 0; i <= search_range; i++ ) {
      uint x_tile = srand_range( 0, 25 );
      int x = tile_coord( player.x() - tile( x_tile ) );
      int y = tile_coord( player.y() + tile( i ) );

      if ( remove_tile( x, y ) ) {
        remove_tile_filth( g, x, y );
        count++;

        if ( count >= max ) {
          count = 0;
          break;
        }
      }
    }
  }

  bool remove_tile( int x, int y ) {
    tileinfo@ ti = g.get_tile( x, y );
    if ( @ti == null ) {
      return false;
    }

    set_tile( g, x, y, false );
    return true;
  }

  void remove_floor() {
    int x_offset = -5;
    if ( player.face() == -1 ) {
      x_offset = -8;
    }

    for ( uint i = 0; i < 15; i++ ) {
      set_tile( g, tile_coord( player.x() + tile( i + x_offset ) ), tile_coord( player.y() ), false );
      remove_tile_filth( g, tile_coord( player.x() + tile( i + x_offset ) ), tile_coord( player.y() ) );

      set_tile( g, tile_coord( player.x() + tile( i + x_offset ) ), tile_coord( player.y() + tile( 1 ) ), false );
      remove_tile_filth( g, tile_coord( player.x() + tile( i + x_offset ) ), tile_coord( player.y() + tile( 1 ) ) );

      set_tile( g, tile_coord( player.x() + tile( i + x_offset ) ), tile_coord( player.y() + tile( 2 ) ), false );
      remove_tile_filth( g, tile_coord( player.x() + tile( i + x_offset ) ), tile_coord( player.y() + tile( 2 ) ) );
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

    if ( player.ground() ) {
      remove_floor();
    }
    else {
      remove_tiles();
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
