#include "../NexusEvent.cpp";

class SwapDoors : NexusEvent {
  scene@ g;
  dustman@ player;

  nexus_api@ api;

  array<entity@> doors = {};
  array<entity@> swappable_doors = {};

  bool fetched_api = false;

  bool swapped = false;

  int time = 0;

  SwapDoors() {
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
          swappable_doors.insertLast( e );
        }

        doors.insertLast( e );
      }
    }

    if ( !swapped && swappable_doors.length > 1 ) {
      swap_doors();

      swapped = true;
    }

    time++;
  }

  void swap_doors() {
    // choose two random doors to swap positions; keep in mind they both need to
    // be loaded in at the moment, but we don't need special logic to account
    // for this as they wouldn't be able to be found and put in the list of
    // doors in the first place if they weren't loaded in currently

    uint index_1 = rand_range( 0, swappable_doors.length-1 );
    entity@ door_1 = swappable_doors[ index_1 ];
    swappable_doors.removeAt( index_1 );

    uint index_2 = rand_range( 0, swappable_doors.length-1 );
    entity@ door_2 = swappable_doors[ index_2 ];
    swappable_doors.removeAt( index_2 );

    float door_1_x = door_1.x();
    float door_1_y = door_1.y();

    door_1.x( door_2.x() );
    door_1.y( door_2.y() );

    door_2.x( door_1_x );
    door_2.y( door_1_y );

    puts( "Swapped: " + door_1.vars().get_var( "file_name" ).get_string() );
    puts( "Swapped: " + door_2.vars().get_var( "file_name" ).get_string() );
  }

  bool is_swappable( entity@ door ) {
    string level_name = door.vars().get_var( "file_name" ).get_string();
    if ( level_name == "Main Nexus DX" ) {
      // don't swap the Main Nexus door, as that might cause some painful
      // searching for the player
      return false;
    }

    int thorough, finesse, key_type;
    float time;
    bool found = api.score_lookup( level_name, thorough, finesse, time, key_type );
    if ( found ) {
      // an "entry" was found by the Nexus API, which practically means a score
      // was submitted in this Nexus save, which means we should not bother
      // swapping them; NOTE: this does NOT take into account whether it was
      // SS'd or Any%'d
      return false;
    }

    return true;
  }
}
