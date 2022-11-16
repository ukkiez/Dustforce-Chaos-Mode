#include "../CycleEvent.cpp";

class BecomeBook : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 15, "Literary Terrorist", "Become a Book" );
  }

  array<uint> BOOK_COLOURS = {
    0xFFFFFFFF,
    0xCCFFFFFF,
    0xAAFFFFFF,
    0x66FFFFFF,
    0x33FFFFFF,
  };

  scene@ g;
  dustman@ player;
  sprites@ spr;
  sprites@ original_sprites;

  float original_fall_max;
  float original_hover_accel;
  int original_dash_max = 1;

  int animation_frame = 0;

  string current_sprite_name = "airidle";

  bool special_started = false;
  int special_frames = 0;
  uint colour_index = 0;

  int frames = 1;

  bool initialized = false;

  BecomeBook() {}

  void step( int entities ) {
    if ( !special_started ) {
      if ( !player.ground() && player.y_speed() > 1 ) {
        if ( current_sprite_name != "cue" ) {
          current_sprite_name = "cue";
          animation_frame = 5;
        }
      }
      else {
        current_sprite_name = "airidle";
      }

      if ( current_sprite_name == "airidle" && ( frames % 4 == 0 ) ) {
        animation_frame++;

        if ( animation_frame > spr.get_animation_length( current_sprite_name ) ) {
          animation_frame = 0;
        }
      }
      else if ( current_sprite_name == "cue" && ( frames % 4 == 0 ) ) {
        // frame 5 and 6 are the same
        animation_frame += 2;

        if ( animation_frame > spr.get_animation_length( current_sprite_name ) ) {
          animation_frame = 4;
        }
      }
    }

    if ( !special_started && player.attack_state() == 3 ) {
      special_started = true;
    }

    if ( special_started && player.attack_state() != 3 ) {
      special_started = false;
      special_frames = 0;
      colour_index = 0;
    }

    if ( special_started ) {
      if ( special_frames <= 8 ) {
        if ( colour_index < COLOURS.length && special_frames % 2 == 0 ) {
          colour_index++;
        }
      }

      special_frames++;
    }

    frames++;
  }

  void draw( float sub_frame ) {
    int facing = player.face();
    if ( player.attack_state() == 1 || player.attack_state() == 2 ) {
      facing = player.attack_face();
    }

    float y = -50;

    if ( current_sprite_name == "cue" && !special_started ) {
      y = -75;
    }

    if ( special_frames > 10 ) {
      // make the book disappear during special, like how dustman normally does
      return;
    }

    spr.draw_world(
      18,
      10,
      current_sprite_name,
      animation_frame,
      1,
      player.x(),
      player.y() + y,
      0,
      facing,
      1.5,
      BOOK_COLOURS[ colour_index ]
    );
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

    @original_sprites = player.get_sprites();
    original_fall_max = player.fall_max();
    original_hover_accel = player.hover_accel();
    original_dash_max = player.dash_max();

    // make player's sprite set invisible; this actually somehow has some effect
    // on physics in some situations but we don't really care
    player.set_sprites( create_sprites() );

    @spr = create_sprites();
    spr.add_sprite_set( "book" );

    player.fall_max( 750 );
    player.hover_accel( 1500 );

    player.dash_max( 3 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    player.set_sprites( original_sprites );
    player.fall_max( original_fall_max );
    player.hover_accel( original_hover_accel );

    player.dash_max( original_dash_max );

    original_fall_max = 0;
    original_hover_accel = 0;
    original_dash_max = 1;

    animation_frame = 0;

    current_sprite_name = "airidle";

    special_started = false;
    special_frames = 0;
    colour_index = 0;

    frames = 1;

    initialized = false;
  }
}
