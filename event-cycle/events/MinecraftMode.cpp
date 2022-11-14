#include "../../lib/util/tiles.cpp";

#include "../CycleEvent.cpp";

class MinecraftMode : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 40, "Minecraft Mode" );
  }

  scene@ g;
  dustman@ player;

  array<hitbox@> hitboxes = {};

  // indicates whether dust_man or an attack's hit_box_controller comes before
  // or after in the iteration order, to determine when we need to replace tiles
  // with dustblocks; this is because attacks can come out a frame early/late
  // depending on this; if the hit_box_controller is before dust_man, then a
  // light attack will for example take 59 frames to come out, but otherwise it
  // would take 58 frames
  bool hit_box_iteration_before_dustman = false;

  float speed_x;
  float speed_y;
  bool just_cleaned = false;
  bool buffered_jump = false;
  bool buffered_dash = false;

  bool initialized = false;

  MinecraftMode() {}

  void draw( float sub_frame ) {}

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

    step_hitboxes( entities );
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
          // attack, like landing after a downlight
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
          int( left - ks( 30, player ) ),
          int( top - ks( 30, player ) ),
          int( right + ks( 30, player ) ),
          int( bottom + ks( 30, player ) )
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

  int ks( int n, dustman@ dm ) {
    // take into account player scale
    return int( n * dm.scale() );
  }

  int clean_filth_block_rec( int t_s_x, int t_s_y, int t_e_x, int t_e_y ) {
    int count = 0;

    t_s_x = int( floor( ( t_s_x ) / 48 ) );
    t_s_y = int( floor ( ( t_s_y ) / 48 ) );
    t_e_x = int( floor ( ( t_e_x ) / 48 ) );
    t_e_y = int( floor ( ( t_e_y ) / 48 ) );
    int s_x = int( min ( t_s_x,t_e_x ) );
    int w =  int( max ( t_s_x,t_e_x ) - min( t_s_x,t_e_x ) );
    int s_y = int( min ( t_s_y,t_e_y) );
    int h =  int( max ( t_s_y,t_e_y ) - min( t_s_y,t_e_y ) );
    for ( int i = 0; i <= w; i++) {
      for ( int b = 0; b <= h; b++) {
        bool cleaned = clean_tile( s_x + i, s_y + b );
        if ( cleaned ) {
          count++;
        }
      }
    }

    return count;
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

    initialized = false;
  }
}
