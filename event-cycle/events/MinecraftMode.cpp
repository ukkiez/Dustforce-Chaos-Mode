#include "../../lib/util/tiles.cpp";

#include "../CycleEvent.cpp";

class MinecraftMode : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 400, "Minecraft Mode" );
  }

  scene@ g;
  dustman@ player;

  array<hitbox@> hitboxes = {};

  // indicates whether dust_man or an attack's hit_box_controller comes before
  // or after in the iteration order, to determine when we need to replace tiles
  // with dustblocks; this is because attacks can come out a frame early/late
  // depending on this; if the hit_box_controller is before dust_man, then a
  // light attack will for example take 9 frames to come out, but otherwise it
  // would take 8 frames
  bool hit_box_iteration_before_dustman = false;

  float speed_x;
  float speed_y;
  bool just_cleaned = false;
  bool buffered_jump = false;
  bool buffered_dash = false;

  uint special_timer = 0;
  float super_hitbox_side = 12.5;
  int super_entity_count = -1;
  uint initial_effect_count = 0;
  bool special_started = false;
  bool super_hitbox_scaled = false;
  bool can_clean = false;

  array<entity@> effects = {};

  bool initialized = false;

  MinecraftMode() {}

  void step( int entities ) {
    if ( just_cleaned ) {
      // see the below setting of the "just_cleaned" variable for context
      player.set_speed_xy( speed_x, ( speed_y > 0 ) ? 0 : speed_y );
      speed_x = 0;
      speed_y = 0;
      just_cleaned = false;

      // allow the inputs the player had buffered to come out now
      if ( buffered_jump ) {
        player.jump_intent( 1 );
        buffered_jump = false;
      }
      if ( buffered_dash ) {
        player.dash_intent( 1 );
        buffered_dash = false;
      }
    }

    for ( int i = 0; i < entities; i++ ) {
      entity@ e = entity_by_index( i );
      if ( @e == null ) {
        continue;
      }

      if ( player.attack_state() == 3 ) {
        effect@ eff = e.as_effect();

        if ( @eff != null && ( eff.sprite_set() == player.character() ) ) {
          bool broke = false;
          for ( uint j = 0; j < effects.length; j++ ) {
            if ( effects[ j ].is_same( eff.as_entity() ) ) {
              broke = true;
              break;
            }
          }
          if ( broke ) {
            continue;
          }

          effects.insertLast( eff.as_entity() );
        }
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

        if ( @h == null || @h.owner() == null ) {
          continue;
        }

        if ( h.owner().id() == player.id() && !h.triggered() ) {
          hitboxes.insertLast( h );
        }
      }
    }

    step_hitboxes( entities );

    manage_special();
  }

  void step_hitboxes( int entities ) {
    for ( uint i = 0; i < hitboxes.length; i++ ) {
      hitbox@ h = hitboxes[ i ];

      // change the tiles that would be in the dustblock clear rectangle 1 frame
      // before the rectangle appears, so that we don't sometimes change tiles
      // to dustblocks 1 frame after the clear happens

      // see the default "hit_box_iteration_before_dustman" variable at the top
      // for an explanation of why we care about iteration order
      for ( int j = 0; j < entities; j++ ) {
        entity@ e = entity_by_index( j );
        if ( @e == null ) {
          continue;
        }

        if ( e.is_same( player.as_entity() ) ) {
          hit_box_iteration_before_dustman = false;
          break;
        }

        if ( e.is_same( h.as_entity() ) ) {
          hit_box_iteration_before_dustman = true;
          break;
        }
      }

      float state_increment = h.timer_speed() / 60;
      bool _check = hit_box_iteration_before_dustman ? ( h.state_timer() >= ( h.activate_time() - state_increment ) ) : h.triggered();
      if ( _check ) {
        if ( h.hit_outcome() == 4 ) {
          // these types of hit outcomes happen when the player canceled the
          // attack, e.g. landing during a downlight
          hitboxes.removeAt( i );
          continue;
        }

        rectangle@ r = h.base_rectangle();

        // find tiles inside the hitbox area, as well as on the outer edges

        float top = h.y() + r.top();
        float bottom = h.y() + r.bottom();
        float left = h.x() + r.left();
        float right = h.x() + r.right();

        // convert all tiles within the dustblock clear rectangle, that this
        // given hitbox would clean
        int count = clean_filth_block_rec(
          int( left - adjust_for_scale( 30, player ) ),
          int( top - adjust_for_scale( 30, player ) ),
          int( right + adjust_for_scale( 30, player ) ),
          int( bottom + adjust_for_scale( 30, player ) )
        );

        if ( count > 0 ) {
          // we *could* try to figure out where the player will have moved over
          // the course of the next frame, and then translate the dustblock
          // clear rectangle accordingly; alternatively we can just set the
          // player speed to 0 for this one frame and then set it back after,
          // because I don't trust my mathematics for even a frame
          speed_x = player.x_speed();
          speed_y = player.y_speed();
          player.set_speed_xy( 0, 0 );
          just_cleaned = true;

          // disallow immediate buffers as the player's speed would be messed up
          // immediately the frame after it comes out, due to setting their
          // speed above; instead, we'll make sure the input comes out
          // afterwards in step()
          if ( player.jump_intent() == 1 ) {
            player.jump_intent( 0 );
            buffered_jump = true;
          }
          if ( player.dash_intent() == 1 ) {
            player.dash_intent( 0 );
            buffered_dash = true;
          }
        }

        hitboxes.removeAt( i );
      }
    }
  }

  int adjust_for_scale( int n, dustman@ dm ) {
    // take into account player scale
    return int( n * dm.scale() );
  }

  int clean_filth_block_rec( int tile_start_x, int tile_start_y, int tile_end_x, int tile_end_y ) {
    int count = 0;

    tile_start_x = int( floor( ( tile_start_x ) / 48 ) );
    tile_start_y = int( floor( ( tile_start_y ) / 48 ) );
    tile_end_x = int( floor( ( tile_end_x ) / 48 ) );
    tile_end_y = int( floor( ( tile_end_y ) / 48 ) );
    int start_x = int( min( tile_start_x, tile_end_x ) );
    int width =  int( max( tile_start_x, tile_end_x ) - min( tile_start_x, tile_end_x ) );
    int start_y = int( min( tile_start_y, tile_end_y) );
    int height =  int( max( tile_start_y, tile_end_y ) - min( tile_start_y, tile_end_y ) );
    for ( int i = 0; i <= width; i++ ) {
      for ( int j = 0; j <= height; j++ ) {
        bool cleaned = clean_tile( start_x + i, start_y + j );
        if ( cleaned ) {
          count++;
        }
      }
    }

    return count;
  }

  void manage_special() {
    if ( player.attack_state() == 3 ) {
      if ( !special_started && player.attack_timer() > 1.5 ) {
        special_started = true;
      }

      if ( special_started ) {
        if ( !super_hitbox_scaled ) {
          super_hitbox_side *= player.scale();
          super_hitbox_scaled = true;
        }

        if ( super_entity_count == -1 ) {
          // get all the hittables that will be supered
          super_entity_count = g.get_entity_collision(
            player.y() - tile( super_hitbox_side ),
            player.y() + tile( super_hitbox_side ),
            player.x() - tile( super_hitbox_side ),
            player.x() + tile( super_hitbox_side ),
            7
          );

          // also don't take into account the player himself
          super_entity_count -= 1;

          initial_effect_count = effects.length;
        }

        if ( super_entity_count != -1 ) {
          if ( super_entity_count == 0 || ( int( effects.length - initial_effect_count ) >= ( super_entity_count * 2 ) ) ) {
            can_clean = true;
          }

          if ( can_clean && special_timer < 12 ) {
            // turn two lines of blocks around the player to dustblocks per frame;
            // for aesthetic effect, and to account for timewarp if the player is
            // sped up

            int x = 10;
            if ( special_timer >= 5 ) {
              x -= ( special_timer - 4 );
            }

            // right from the player
            for ( int i = 0; i <= x; i++ ) {
              clean_tile(
                tile_coord( player.x() + tile( i ) ),
                tile_coord( player.y() - tile( special_timer ) )
              );
              if ( special_timer > 0 ) {
                clean_tile(
                  tile_coord( player.x() + tile( i ) ),
                  tile_coord( player.y() + tile( special_timer ) )
                );
              }
            }

            // left from the player
            for ( int i = 1; i <= x; i++ ) {
              clean_tile(
                tile_coord( player.x() - tile( i ) ),
                tile_coord( player.y() - tile( special_timer ) )
              );
              if ( special_timer > 0 ) {
                clean_tile(
                  tile_coord( player.x() - tile( i ) ),
                  tile_coord( player.y() + tile( special_timer ) )
                );
              }
            }

            special_timer++;
          }
        }
      }
    }

    if ( special_timer > 0 && player.attack_state() != 3 ) {
      // note that we've finished the last special and the work that needed to
      // be done
      special_timer = 0;
      super_hitbox_side = 12.5;
      super_entity_count = -1;
      initial_effect_count = 0;
      special_started = false;
      super_hitbox_scaled = false;
      can_clean = false;
      effects.resize( 0 );
    }
  }

  bool clean_tile( int x, int y ) {
    tileinfo@ tile = g.get_tile( x, y );
    if ( !tile.solid() || tile.is_dustblock() ) {
      return false;
    }

    tile.set_dustblock( tile.sprite_set() );
    g.set_tile( x, y, 19, tile, true );

    return true;
  }

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

    hitboxes.resize( 0 );
    hit_box_iteration_before_dustman = false;

    just_cleaned = false;
    buffered_jump = false;
    buffered_dash = false;

    special_timer = 0;
    super_hitbox_side = 12.5;
    super_entity_count = -1;
    initial_effect_count = 0;
    special_started = false;
    super_hitbox_scaled = false;
    can_clean = false;
    effects.resize( 0 );

    initialized = false;
  }
}
