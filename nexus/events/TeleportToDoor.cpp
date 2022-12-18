#include "../NexusEvent.cpp";

class TeleportToDoor : NexusEvent {
  scene@ g;
  dustman@ player;

  nexus_api@ api;

  array<entity@> doors = {};
  array<entity@> teleportable_doors = {};

  bool fetched_api = false;

  bool teleported = false;

  TeleportToDoor() {
    @g = get_scene();

    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }
  }

  void step( int entities ) {
    if ( !fetched_api ) {
      @api = get_nexus_api();

      if ( @api != null ) {
        fetched_api = true;
      }
    }

    if ( @api == null ) {
      return;
    }

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

        if ( is_swappable( e ) ) {
          teleportable_doors.insertLast( e );
        }

        doors.insertLast( e );
      }
    }

    if ( !teleported && teleportable_doors.length > 0 ) {
      teleport_to_door();

      teleported = true;
    }
  }

  void teleport_to_door() {
    // choose a random door to teleport to

    uint index_1 = rand_range( 0, teleportable_doors.length-1 );
    entity@ door = teleportable_doors[ index_1 ];
    teleportable_doors.removeAt( index_1 );

    player.x( door.x() );
    player.y( door.y() );
  }

  bool is_swappable( entity@ door ) {
    string level_name = door.vars().get_var( "file_name" ).get_string();
    if ( level_name == "Main Nexus DX" ) {
      // also allow teleporting to Main Nexus door
      return true;
    }

    int thorough, finesse, key_type;
    float time;
    bool found = api.score_lookup( level_name, thorough, finesse, time, key_type );
    if ( found ) {
      // an "entry" was found by the Nexus API, which practically means a score
      // was submitted in this Nexus save, which are the doors we want don't to
      // teleport to; NOTE: this does NOT take into account whether it was SS'd
      // or Any%'d
      return false;
    }

    return true;
  }
}
