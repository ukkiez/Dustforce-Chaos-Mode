#include "../lib/util/tiles.cpp";

#include "./Mode.cpp";

class MinecraftMode : Mode {
  ModeConfig get_mode_config() {
    return ModeConfig( 40, "Minecraft Mode" );
  }

  scene@ g;
  dustman@ player;

  array<hitbox@> hitboxes = {};

  bool initialized = false;

  MinecraftMode() {}

  void step( int entities ) {
    for ( int i = 0; i < entities; i++ ) {
      entity@ e = entity_by_index( i );
      if ( @e == null ) {
        return;
      }

      if ( e.type_name() == "hit_box_controller" ) {
        bool broke = false;
        for ( uint j = 0; j < hitboxes.length; j++ ) {
          if ( e.is_same( hitboxes[ j ].as_entity() ) ) {
            broke = true;
            break;
          }
        }
        if ( broke ) {
          continue;
        }

        hitbox@ h = e.as_hitbox();

        if ( @h == null ) {
          continue;
        }

        if ( h.owner().id() == player.id() && !h.triggered() ) {
          hitboxes.insertLast( h );
        }
      }
    }

    step_hitboxes();
  }

  void step_hitboxes() {
    for ( uint i = 0; i < hitboxes.length; i++ ) {
      hitbox@ h = hitboxes[ i ];

      bool removed_a_tile = false;
      if ( h.triggered() ) {
        if ( h.hit_outcome() == 4 ) {
          // these types of hit outcomes happen when the player canceled the
          // attack, like landing after a downlight
          hitboxes.removeAt( i );
          continue;
        }

        rectangle@ r = h.base_rectangle();

        // find tiles inside the hitbox area, as well as on the outer edges

        int num_tiles_height = ceil_int( r.get_height() / tile( 1 ) );
        int num_tiles_width = ceil_int( r.get_width() / tile( 1 ) );
        float top = h.y() + r.top();
        float bottom = h.y() + r.bottom();
        float left = h.x() + r.left();
        float right = h.x() + r.right();

        float segment_width = r.get_width() / num_tiles_width;
        float segment_height = r.get_height() / num_tiles_height;
        for ( int j = 0; j <= num_tiles_height; j++ ) {
          for ( int w = 0; w <= num_tiles_width; w++ ) {
            bool removed_tile = false;
            removed_tile = check_and_remove_tile(
              left + ( w * segment_width ),
              top + ( j * segment_height )
            );

            if ( removed_tile ) {
              removed_a_tile = true;
            }
          }
        }

        hitboxes.removeAt( i );
      }

      if ( removed_a_tile ) {
        if ( h.hit_outcome() == 2 ) {
          // freeze the player for a small moment to give some weight behind
          // breaking blocks; only do this when the attack didn't hit anything
          // else, otherwise the freeze frame timers might stack with that
          player.freeze_frame_timer( 1 );
        }

        array<string> sounds = {
          "sfx_trash_light_1",
          "sfx_trash_light_2",
          "sfx_trash_light_3",
          "sfx_trash_light_4",
          "sfx_trash_light_5",
          "sfx_trash_light_6",
          "sfx_trash_light_7",
          "sfx_trash_light_8",
          "sfx_trash_light_9",
          "sfx_trash_light_10"
        };
        g.play_sound( sounds[ rand_range( 0, sounds.length-1 ) ], h.x(), h.y(), 1, false, true );
      }
    }
  }

  bool check_and_remove_tile( float x, float y ) {
    int _x = tile_coord( x );
    int _y = tile_coord( y );

    tileinfo@ tile = g.get_tile( _x, _y );
    if ( !tile.solid() || tile.is_dustblock() ) {
      return false;
    }

    // first remove the dust from the tile, because otherwise if the attack
    // doesn't clear the dust itself the particles will remain floating in the
    // air; though this would not mess with level dust completion, it's nicer
    // visually
    tilefilth@ tf = g.get_tile_filth( _x, _y );
    remove_filth( tf, _x, _y );

    array<string> particles = { "dust1", "dust2", "dust3" };

    entity@ e = g.add_effect( "particles", particles[ rand_range( 0, 2 ) ], x, y, rand_range( 0, 180 ), 1, 1, 45 );

    // remove the tile
    tile.solid( false );
    g.set_tile( _x, _y, 19, tile, true );

    // give the player some combo for their troubles; make sure to set the combo
    // timer as otherwise it will just keep blinking for some reason
    player.combo_count( player.combo_count() + 1 );
    player.combo_timer( 1 );
    player.skill_combo( player.skill_combo() + 1 );

    return true;
  }

  bool remove_filth( tilefilth@ tf, int x, int y ) {
    bool has_dust = ( tf.top() > 0 ) ||
    ( tf.bottom() > 0 ) ||
    ( tf.left() > 0 ) ||
    ( tf.right() > 0 );

    if ( has_dust ) {
      // simply overwrite the existing tilefilth with a fresh instance
      tilefilth@ empty_tf = create_tilefilth();
      g.set_tile_filth( x, y, empty_tf );
    }

    return has_dust;
  }

  void draw( float sub_frame ) {}

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

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }

  int ceil_int( float x ) {
	  return int( ceil( x ) );
  }

  int rand_range( int min, int max ) {
	  return min + ( rand() % ( max - min + 1 ) );
  }
}
