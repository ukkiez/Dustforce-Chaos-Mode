#include "../../QueueEvent.cpp";

class EnemyTimeWarp : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 50, 1, "Enemizer", "Fasterizer" );
  }

  scene@ g;
  dustman@ player;

  array<controllable@> warped_controllables;

  bool initialized = false;

  bool warped = false;

  EnemyTimeWarp() {}

  void step( int entities ) {
    if ( warped ) {
      return;
    }

    for ( int i = 0; i < entities; i++ ) {
      entity @e = entity_by_index( i );

      if ( @e == null ) {
        continue;
      }

      if ( e.id() == player.id() ) {
        continue;
      }

      controllable@ c = e.as_controllable();
      if ( @c != null ) {
        c.time_warp( 3 );
        warped_controllables.insertLast( c );
      }
    }

    warped = true;
  }
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    @player = controller_controllable( 0 ).as_dustman();

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    // uncomment below code to deactivate time warp from the affected entities

    // for ( uint i = 0; i <= warped_controllables.length-1; i++ ) {
    //   controllable@ c = warped_controllables[ i ];
    //   c.time_warp( 1 );
    // }

    warped_controllables.resize( 0 );

    warped = false;

    initialized = false;
  }
}
