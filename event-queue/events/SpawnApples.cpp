#include "../../lib/Random.cpp";

#include "../QueueEvent.cpp";

class SpawnApples : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 100, 0, "Skyhawk's Favour" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  SpawnApples() {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

    spawn_apples();

    initialized = true;
  }

  void spawn_apples() {
    uint max = 7;
    if ( g.level_type() != 0 && g.level_type() != 6 ) {
      // spawn less apples outside of levels, to combat lag issues; in e.g.
      // nexuses, entities will not disappear upon exiting / entering the nexus
      // or its levels, and on top of that entities remain specially loaded from
      // very far away
      max = 1;
    }

    array<int> apples_x = {};
    uint num_apples = srand_range( 1, 7 );
    for ( uint i = 0; i <= num_apples; i++ ) {
      entity@ apple = create_entity( "hittable_apple" );

      int x, y;
      x = ( ( srandom() % 500 ) - ( srandom() % 500 ) );
      if ( apples_x.find( x ) >= 0 ) {
        x += ( srandom() % 200 - 100 );
      }

      apples_x.insertLast( x );

      y = 600 - ( srandom() % 200 );

      apple.x( player.x() + x );
      apple.y( player.y() - y );
      g.add_entity( apple );
    }
  }
}
