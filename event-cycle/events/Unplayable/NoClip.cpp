#include "../../CycleEvent.cpp";

class NoClip : CycleEvent, callback_base {
  CycleEventConfig get_config() {
    return CycleEventConfig( 10, "Literally Unplayable", "No Clip Mode" );
  }

  scene@ g;
  dustman@ player;

  float di_speed;
  float x_speed;
  float y_speed;

  bool initialized = false;

  NoClip() {}

  void step( int entities ) {
    player.state( 7 );
    player.fall_intent( 0 );

    player.set_speed_xy( x_speed * player.x_intent(), y_speed * player.y_intent() );
  }

  void collision_handler( controllable@ ec, tilecollision@ tc, int side, bool moving, float snap_offset, int arg ) {
    tc.hit( false );
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

    di_speed = player.di_speed();

    player.set_collision_handler( this, "collision_handler", 0 );

    player.dash( 0 );
    player.dash_max( 0 );
    player.di_speed( 0 );

    x_speed = max( 500, int( player.x_speed() ) );
    y_speed = max( 500, int( player.y_speed() ) );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    player.set_collision_handler( null, "", 0 );

    player.dash_max( player.character() == "dustkid" ? 2 : 1 );
    player.di_speed( di_speed );

    initialized = false;
  }
}
