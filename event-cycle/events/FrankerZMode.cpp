#include "../../lib/data/characters.cpp";

#include "../CycleEvent.cpp";

const uint FRANKERZ_COLOUR = 0xCCFFFFFF;

class FrankerZ {
  float x;
  float y;
  uint time = 0;
  uint duration = 120;
  int rotation = 0;
  float scale_x = 1.0;
  float scale_y = 1.0;
  uint colour = FRANKERZ_COLOUR;

  FrankerZ( float x, float y, int rotation = 0, float scale_x = 1.0, float scale_y = 1.0, uint colour = FRANKERZ_COLOUR ) {
    this.x = x;
    this.y = y;
    this.rotation = rotation;
    this.scale_x = scale_x;
    this.scale_y = scale_y;
    this.colour = colour;
  }
  FrankerZ( float x, float y, int duration, int rotation, float scale_x = 1.0, float scale_y = 1.0, uint colour = FRANKERZ_COLOUR ) {
    this.x = x;
    this.y = y;
    this.duration = duration;
    this.rotation = rotation;
    this.scale_x = scale_x;
    this.scale_y = scale_y;
    this.colour = colour;
  }
}

class FrankerZMode : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 400, "FrankerZ Mode", "FrankerZ FrankerZ FrankerZ" );
  }

  scene@ g;
  dustman@ player;
  sprites@ spr;

  int frame = 1;
  int palette = 1;

  int screen_x_boundary = 825;
  int screen_y_boundary = 475;

  array<FrankerZ@> frankerz_hud_sprites = {};
  array<FrankerZ@> frankerz_world_sprites = {};

  bool light_intent_zeroed = true;
  bool heavy_intent_zeroed = true;
  bool jump_intent_zeroed = true;
  bool fall_intent_zeroed = true;

  bool in_special = false;
  float special_attack_timer;
  uint special_frankerz_frame_interval = 8;
  array<entity@> special_effects = {};

  uint frames = 0;

  bool initialized = false;

  FrankerZMode() {}

  void step( int entities ) {
    // light attacks
    if ( light_intent_zeroed && player.light_intent() == 10 ) {
      light_intent_zeroed = false;
      frankerz_world_sprites.insertLast( random_frankerz( player.x(), player.y() ) );
    }
    if ( !light_intent_zeroed && player.light_intent() != 10 ) {
      light_intent_zeroed = true;
    }

    // heavy attacks
    if ( heavy_intent_zeroed && player.heavy_intent() == 10 ) {
      heavy_intent_zeroed = false;
      frankerz_world_sprites.insertLast( random_frankerz( player.x(), player.y() ) );
    }
    if ( !heavy_intent_zeroed && player.heavy_intent() != 10 ) {
      heavy_intent_zeroed = true;
    }

    // jumping
    if ( jump_intent_zeroed && player.jump_intent() != 0 ) {
      jump_intent_zeroed = false;
      frankerz_world_sprites.insertLast( random_frankerz( player.x(), player.y() ) );
    }
    if ( !jump_intent_zeroed && player.jump_intent() == 0 ) {
      jump_intent_zeroed = true;
    }

    // dashing
    if ( player.dash_intent() == 1 ) {
      frankerz_world_sprites.insertLast( random_frankerz( player.x(), player.y() ) );
    }
    else if ( player.fall_intent() == 1) {
      frankerz_world_sprites.insertLast( random_frankerz( player.x(), player.y() ) );
    }

    // taunt
    if ( player.taunt_intent() == 1 ) {
      frankerz_world_sprites.insertLast( random_frankerz( player.x(), player.y() ) );
    }

    // super
    if ( !in_special && player.attack_state() == 3 ) {
      in_special = true;
    }
    else if ( in_special && player.attack_state() != 3 ) {
      in_special = false;
      special_effects.resize( 0 );
    }
    if ( in_special ) {
      for ( int i = 0; i < entities; i++ ) {
        entity@ e = entity_by_index( i );
        if ( @e == null ) {
          continue;
        }

        effect@ eff = e.as_effect();
        if ( @eff == null ) {
          continue;
        }

        if ( eff.sprite_index() != ( string( CHARACTER_ACRONYMS[ player.character() ] ) + "strike" ) ) {
          continue;
        }

        bool broke = false;
        for ( uint j = 0; j < special_effects.length; j++ ) {
          if ( special_effects[ j ].is_same( e ) ) {
            broke = true;
            break;
          }
        }
        if ( broke ) {
          continue;
        }

        frankerz_hud_sprites.insertLast( random_frankerz() );
        frankerz_world_sprites.insertLast( random_frankerz( e.x(), e.y() ) );
        special_effects.insertLast( e );
      }
    }

    for ( uint i = 0; i < frankerz_hud_sprites.length; i++ ) {
      if ( frankerz_hud_sprites[ i ].time >= frankerz_hud_sprites[ i ].duration ) {
        frankerz_hud_sprites.removeAt( i );
      }
      else {
        frankerz_hud_sprites[ i ].time++;
      }
    }
    for ( uint i = 0; i < frankerz_world_sprites.length; i++ ) {
      if ( frankerz_world_sprites[ i ].time >= frankerz_world_sprites[ i ].duration ) {
        frankerz_world_sprites.removeAt( i );
      }
      else {
        frankerz_world_sprites[ i ].time++;
      }
    }

    frames++;
  }
  void draw( float sub_frame ) {
    for ( uint i = 0; i < frankerz_hud_sprites.length; i++ ) {
      draw_frankerz( frankerz_hud_sprites[ i ] );
    }
    for ( uint i = 0; i < frankerz_world_sprites.length; i++ ) {
      draw_frankerz( frankerz_world_sprites[ i ], true );
    }
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

    @spr = create_sprites();
    spr.add_sprite_set( "script" );

    initialized = true;
  }

  FrankerZ random_frankerz() {
    return FrankerZ(
      srand_range( -screen_x_boundary, screen_x_boundary ),
      srand_range( -screen_y_boundary, screen_y_boundary ),
      srand_range( 0, 360 ),
      srand_range_flt( 0.2, 1.8 ),
      srand_range_flt( 0.2, 1.8 )
    );
  }
  FrankerZ random_frankerz( float x, float y ) {
    return FrankerZ(
      x,
      y,
      srand_range( 0, 360 )
    );
  }

  void draw_frankerz( FrankerZ@ frankerz, bool world = false ) {
    if ( world ) {
      spr.draw_world(
        19,
        22,
        "FrankerZ",
        frame,
        palette,
        frankerz.x,
        frankerz.y,
        frankerz.rotation,
        frankerz.scale_x,
        frankerz.scale_y,
        frankerz.colour
      );
    }
    else {
      spr.draw_hud(
        20,
        22,
        "FrankerZ",
        frame,
        palette,
        frankerz.x,
        frankerz.y,
        frankerz.rotation,
        frankerz.scale_x,
        frankerz.scale_y,
        frankerz.colour
      );
    }

  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
