#include "../NexusEvent.cpp";

const array<uint> LOCKLESS_DOOR_TYPES = {
  1,
  4,
  5,
  8,
  9,
  12,
  13,
  16,
  17,
  20,
  21,
};

class ChangeDoorTypes : NexusEvent {
  scene@ g;
  dustman@ player;

  array<entity@> doors = {};

  int time = 0;

  ChangeDoorTypes() {
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

        e.vars().get_var( "door_set" ).set_int32( LOCKLESS_DOOR_TYPES[ rand_range( 0, LOCKLESS_DOOR_TYPES.length-1 ) ] );

        doors.insertLast( e );
      }
    }

    time++;
  }
}
