#include "../../../lib/Random.cpp";

#include "../../QueueEvent.cpp";

#include "./data.cpp";

class PropSwap : QueueEvent, Random {
  QueueEventConfig get_config() {
    return QueueEventConfig( 100, 0, false, "Prop Swap" );
  }

  scene@ g;
  controllable@ player;

  bool initialized = false;

  int prop_collision = 0;

  PropSwap() {}

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    @player = controller_controllable( 0 );

    prop_collision = g.get_prop_collision( player.y() - 1250, player.y() + 1250, player.x() - 1250, player.x() + 1250 );

    if ( prop_collision == 0 ) {
      // there are no props in the area
      initialized = true;
      return;
    }

    for ( int i = 0; i <= prop_collision - 1; i++ ) {
      prop@ p = g.get_prop_collision_index( i );

      PropReference reference = prop_references[ srand_range( 0, prop_references.length - 1 ) ];

      p.prop_set( reference.set );
      p.prop_group( reference.group );
      p.prop_index( reference.index );
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
