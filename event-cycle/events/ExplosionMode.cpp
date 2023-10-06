#include "../CycleEvent.cpp";

#include "../../lib/Random.cpp";

class ExplosionAnimation : Random {
  float x;
  float y;
  uint following_entity_id;
  int active_frames = 0;
  uint frame_index = 0;

  float scaling = 2;

  bool sound_played = false;

  ExplosionAnimation( float x, float y, int entity_id ) {
    this.x = x - srand_range( 60, 100 );
    this.y = y - srand_range( 120, 140 );
    this.following_entity_id = entity_id;

    this.scaling = srand_range_flt( 1, 3 );
  }
}

class ExplosionMode : CycleEvent, callback_base {
  CycleEventConfig get_config() {
    return CycleEventConfig( 300, "Michael Mode" );
  }

  scene@ g;
  dustman@ player;

  uint frames = 0;
  sprites@ spr;

  bool added = false;

  uint gif_index = 1;
  uint gif_frames = 17;

  uint frame_interval = 3;

  float x_pos = 0;
  float y_pos = 0;

  bool initialized = false;

  callback_base@ base_obj;

  array<ExplosionAnimation@> active_animations = {};

  array<string> explosion_sfx = {
    "small_explosion",
    "medium_explosion",
    "harsh_explosion",
  };

  ExplosionMode() {}

  void entity_on_add( entity@ entity ) {
    if ( entity.type_name() == "hit_box_controller" ) {
      entity.as_hitbox().on_hit_filter_callback( this, "on_hit", 0 );
    }

    if ( player.attack_state() == 3 ) {
      if ( entity.type_name() == "effect" ) {
        active_animations.insertLast( ExplosionAnimation( entity.x(), entity.y(), entity.id() ) );
      }
    }
  }
  bool on_hit( hitbox@ h, hittable@ hit, int ) {
    active_animations.insertLast( ExplosionAnimation( hit.x(), hit.y(), hit.id() ) );
    return true;
  }

  void entity_on_remove( entity@ entity ) {
    if ( @entity.as_controllable() == null ) {
      return;
    }

    for ( uint i = 0; i < active_animations.length; i++ ) {
      if ( active_animations[ i ].following_entity_id == entity.id() ) {
        return;
      }
    }

    active_animations.insertLast( ExplosionAnimation( entity.x(), entity.y(), entity.id() ) );
  }

  void step( int entities ) {
    bool sound_played = false;
    for ( uint i = 0; i < active_animations.length; i++ ) {
      ExplosionAnimation@ anim = active_animations[ i ];

      if ( !anim.sound_played ) {
        if ( !sound_played ) {
          // limit the number of sounds played on the same frame
          play_explosion_sfx();
          sound_played = true;
        }
        anim.sound_played = true;
      }

      if ( anim.active_frames % frame_interval == 0 ) {
        anim.frame_index++;

        if ( anim.frame_index > gif_frames ) {
          active_animations.removeAt( i );
        }
      }

      anim.active_frames++;
    }
  }

  void play_explosion_sfx() {
    g.play_script_stream( explosion_sfx[ rand_range( 0, explosion_sfx.length-1 ) ], 3, 0, 0, false, 0.3 );
  }

  void draw( float subframe ) {
    uint frame = 1;
    uint palette = 1;
    uint colour = 0xFFFFFFFF;

    for ( uint i = 0; i < active_animations.length; i++ ) {
      ExplosionAnimation@ anim = active_animations[ i ];

      string gif_name = "explosion" + anim.frame_index;
      spr.draw_world( 20, 1, gif_name, frame, palette, anim.x, anim.y, 0, anim.scaling, anim.scaling, colour );
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

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}

