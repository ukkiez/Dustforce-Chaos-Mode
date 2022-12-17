#include "../../lib/Box.cpp";

#include "../NexusEvent.cpp";

class Door {
  int walk_count = 0;
  int walk_limit = 0;
  bool walked = false;
  entity@ entity;
  Door( entity@ door, int walk_limit ) {
    @this.entity = door;
    this.walk_limit = walk_limit;
  }
}

class ShyDoors : NexusEvent {
  scene@ g;
  dustman@ player;

  Box@ proximity_box = Box( -150, -50, 150, 50 );

  array<Door@> doors = {};

  uint door_walk_timer = 0;
  uint interval = 180;
  bool door_walk = false;
  int walking_distance = 100;

  ShyDoors() {
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
          if ( e.is_same( doors[ j ].entity ) ) {
            broke = true;
            break;
          }
        }
        if ( broke ) {
          continue;
        }

        srand( timestamp_now() );
        doors.insertLast( Door( e, rand_range( 1, 5 ) ) );
      }
    }

    manage_doors();
  }

  void manage_doors() {
    for ( uint i = 0; i < doors.length; i++ ) {
      Door@ _door = doors[ i ];
      if ( _door.walked ) {
        continue;
      }

      if ( _door.entity.vars().get_var( "door_set" ).get_int32() == 0 ) {
        // don't walk doors that are invisible
        continue;
      }

      entity@ door = _door.entity;

      // check if a player is inside the proximity box
      int _collision = g.get_entity_collision(
        door.y() + proximity_box.top,
        door.y() + proximity_box.bottom,
        door.x() + proximity_box.left,
        door.x() + proximity_box.right,
        5
      );

      if ( _collision > 0 ) {
        // player is inside range of a door, so have it start walking away
        bool right = false;
        if ( player.x() - door.x() > 0 ) {
          right = true;
        }
        door.x( door.x() + ( right ? -walking_distance : walking_distance ) );

        _door.walk_count++;
        if ( _door.walk_count >= _door.walk_limit ) {
          _door.walked = true;
        }
      }
    }
  }
}
