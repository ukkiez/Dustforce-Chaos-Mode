#include "../../lib/Random.cpp";
#include "../QueueEvent.cpp";

class Box {
	float left;
	float top;
	float right;
	float bottom;

	Box( float left = 0, float top = 0, float right = 0, float bottom = 0 ) {
		this.left = left;
		this.top = top;
		this.bottom = bottom;
		this.right = right;
	}

	void set( float left, float top, float right, float bottom ) {
		this.left = left;
		this.top = top;
		this.right = right;
		this.bottom = bottom;
	}

	bool is_inside( float box_pos_x, float box_pos_y, float x, float y ) {
		return ( x >= box_pos_x + left ) &&
    ( x <= box_pos_x + right ) &&
    ( y >= box_pos_y + top ) &&
    ( y <= box_pos_y + bottom );
	}
}

class PolishDriver : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 35, 300, "Polish Driver" );
  }

  scene@ g;
  dustman@ player;

  prop@ car;
  Box@ car_hitbox = Box( -150, -105, 145, 110 );

  audio@ CAR_PASSING;
  audio@ CAR_CRASH;
  audio@ CAR_HORN;

  bool car_spawned = false;
  bool hit = false;

  float scale_x = 1.5;

  float spawn_offset_x = 1700;
  float spawn_offset_y = -105;

  float move_x = -28;
  float move_x_crash = -24;

  float crash_scaling_x = 0.3;
  float crash_rotation1 = -32.5;
  float crash_rotation2 = -7;

  float stun_direction_x = -2500;
  float stun_direction_y = -750;

  int frames = 0;
  int crash_timer = 0;
  float freeze_time = 20;
  bool frozen = false;

  entity@ crash_effect;
  string sprite_set = "effects";
  string sprite_name = "dmhitfx";

  bool leftToRight = false;

  bool initialized = false;

  PolishDriver() {}

  void step( int entities ) {
    frames++;

    if ( @car_hitbox == null || @car == null ) {
      return;
    }

    if ( @CAR_HORN == null ) {
      string horn_audio = leftToRight ? "car_horn_left" : "car_horn_right";
      @CAR_HORN = g.play_script_stream( horn_audio, 3, 0, 0, false, 0.4 );
    }
    if ( @CAR_PASSING == null ) {
      string passing = leftToRight ? "car_passing_l_to_r" : "car_passing_r_to_l";
      @CAR_PASSING = g.play_script_stream( passing, 3, 0, 0, false, 0.5 );
    }

    // add the car prop to the scene
    if ( !car_spawned && frames >= 40 ) {
      car.x( player.x() + spawn_offset_x );
      car.y( player.y() + spawn_offset_y );

      g.add_prop( car );

      car_spawned = true;
    }

    if ( !car_spawned ) {
      return;
    }

    if ( !hit ) {
      // move the car
      car.x( car.x() + move_x );

      // animate the car via its scaling, to give it a bit of life
      if ( frames % 22 == 0 ) {
        car.scale_y( 1.75 );
        car.y( car.y() - 15 );
      }
      else if ( frames % 22 == 10 ) {
        car.scale_y( 1.5 );
        car.y( car.y() + 15 );
      }

      // check if a player is inside the car box, and if they are mark that they
      // were hit, unless they are supering
      int _collision = g.get_entity_collision( car.y()+car_hitbox.top, car.y()+car_hitbox.bottom, car.x()+car_hitbox.left, car.x()+car_hitbox.right, 5 );
      if ( _collision > 0 && player.attack_state() != 3 ) {
        hit = true;

        // hit player away and stun them for a bit
        player.stun( stun_direction_x, stun_direction_y );
        player.stun_timer( 20 );

        // remove combo
        player.combo_count( 0 );

        CAR_PASSING.volume( CAR_PASSING.volume() - 0.2 );
      }
    }
    else {
      crash_timer++;

      if ( !frozen ) {
        player.freeze_frame_timer( 5 );
        frozen = true;

        @crash_effect = g.add_follow_effect(
          sprite_set,
          sprite_name,
          player.x() + ( leftToRight ? 50 : -50 ),
          player.y() - 50,
          leftToRight ? 0 : -160, // Rotation
          1,
          1,
          60,
          player.as_entity(),
          false,
          false
        );
        crash_effect.as_effect().freeze_target( player.as_hittable() );
      }

      if ( frozen && crash_timer < freeze_time ) {
        if ( @CAR_CRASH == null ) {
          string crash_audio = leftToRight ? "car_crash_l_to_r" : "car_crash_r_to_l";
          @CAR_CRASH = g.play_script_stream( crash_audio, 3, 0, 0, false, 0.35 );
        }

        car.x( car.x() + ( leftToRight ? 10 : -10 ) );
        return;
      }

      // move the car
      car.x( car.x() + move_x_crash );

      // animate the car via its scaling, to give it a bit of life
      if ( car.scale_x() <= -1.5 || car.scale_x() >= 1.5 ) {
        crash_scaling_x = -crash_scaling_x;
      }
      car.scale_x( car.scale_x() + crash_scaling_x );

      // do some animation for just after the crash occurred
      if ( crash_timer <= 30 ) {
        car.y( car.y() - 5 );
        car.rotation( car.rotation() + crash_rotation1 );
      }
      else if ( crash_timer <= 50 ) {
        car.y( car.y() + 3 );
      }

      // make the car fly off after a second after the crash occurred
      if ( crash_timer >= 60 ) {
        car.y( car.y() - 20 );
        car.rotation( car.rotation() + crash_rotation2 );
        if ( CAR_PASSING.is_playing() ) {
          CAR_PASSING.stop();
        }
      }
    }
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

    // have a 50% of the car coming from the left or right
    leftToRight = ( srandom() % 2 == 0 ) ? true : false;
    if ( leftToRight ) {
      // update variables to account for the car coming from the left instead of
      // the right
      scale_x = -scale_x;
      @car_hitbox = Box( -150, -105, 160, 110 );

      spawn_offset_x = -spawn_offset_x;

      move_x = -move_x;
      move_x_crash = -move_x_crash;
      crash_rotation1 = -crash_rotation1;
      crash_rotation2 = -crash_rotation2;

      crash_scaling_x = -crash_scaling_x;

      stun_direction_x = -stun_direction_x;
    }

    @car = create_prop();
    car.prop_set( 3 );
    car.prop_group( 26 );
    car.prop_index( 2 );
    car.palette( srand_range( 0, 2 ) );
    car.layer( 19 );
    car.sub_layer( 22 );
    car.scale_x( scale_x );
    car.scale_y( 1.5 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    g.remove_prop( car );

    initialized = false;
  }
}
