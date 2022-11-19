#include "../NexusEvent.cpp";

class SlowDoors : NexusEvent {
  scene@ g;
  dustman@ player;

  array<entity@> doors = {};

  SlowDoors() {
    @g = get_scene();

    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }
  }

  void step( int entities ) {
    for ( int i = 0; i < entities; i++ ) {
      entity@ e = entity_by_index( i );
      if ( @e == null ) {
        continue;
      }

      if ( e.type_name() == "level_door" ) {
        bool broke = false;
        for ( uint j = 0; j < doors.length; j++ ) {
          if ( e.is_same( doors[ j ] ) ) {
            broke = true;
            break;
          }
        }
        if ( broke ) {
          continue;
        }

        e.time_warp( rand_range( 0.01, 0.5 ) );
        doors.insertLast( e );
      }
    }
  }
}
