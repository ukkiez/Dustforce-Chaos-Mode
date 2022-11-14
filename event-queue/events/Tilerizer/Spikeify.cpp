#include "../../../lib/Random.cpp";
#include "../../../lib/util/tiles.cpp";

#include "../../QueueEvent.cpp";

#include "./data.cpp";

class Spikeify : QueueEvent, Random {
  QueueEventConfig get_config() {
    return QueueEventConfig( 40, 0, false, "Exec Func Ruin Level", "Spikeify" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  Spikeify() {
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

    uint x_tile = srand_range( 15, 25 );

    for ( uint i = 0; i <= 25; i++ ) {
      int x = tile_coord( player.x() + tile( x_tile ) );
      int y = tile_coord( player.y() + tile( i ) );

      bool set_filth = false;
      set_filth = replace_tf( x, y );
      set_filth = set_filth || replace_tf( x, -y );
      set_filth = set_filth || replace_tf( x, y );
      set_filth = set_filth || replace_tf( x, -y );

      if ( !set_filth ) {
        continue;
      }

      break;
    }

    initialized = true;
  }

  bool replace_tf( int x, int y ) {
    // check whether the tile has an exposed edge and whether it already has a
    // spike on it
    tileinfo@ ti = g.get_tile( x, y );
    tilefilth@ tf = g.get_tile_filth( x, y );
    bool set_filth = false;
    if ( ti.edge_top() != 0 && tf.top() >= 0 && tf.top() < 9 ) {
      tf.top( 9 );
      set_filth = true;
    }
    if ( ti.edge_left() != 0 &&tf.left() >= 0 && tf.left() < 9 ) {
      tf.left( 9 );
      set_filth = true;
    }
    if ( ti.edge_right() != 0 &&tf.right() >= 0 && tf.right() < 9 ) {
      tf.right( 9 );
      set_filth = true;
    }
    if ( ti.edge_bottom() != 0 &&tf.bottom() >= 0 && tf.bottom() < 9 ) {
      tf.bottom( 9 );
      set_filth = true;
    }

    if ( set_filth ) {
      g.set_tile_filth( x, y, tf );
    }

    return set_filth;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
