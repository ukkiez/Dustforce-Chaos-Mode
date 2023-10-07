#include "../QueueEvent.cpp";

class LoopTheLoop : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 175, 60, "Loop-the-loop" );
  }

  scene@ g;
  dustman@ player;

  script@ _script;

  // circle
  float origin_x;
  float origin_y;

  float radius = 200;
  uint segments = 60;
  float _step = PI2 / segments;
  float angle = _step;

  uint frames = 0;

  bool initialized = false;

  LoopTheLoop() {}

  void step( int entities ) {
    if ( _script.turbo_mode ) {
      return;
    }

    if ( frames % ( 60 / segments ) == 0 ) {
      teleportPlayer();
    }

    frames++;
  }

  void teleportPlayer() {
    // teleport the player around points on a circle
    float x = origin_x + ( radius * cos( angle ) );
    float y = origin_y + ( radius * sin( angle ) );

    angle += _step;

    player.x( x );
    player.y( y );
  }

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();

    // get the main script object
    @_script = cast<script@>( get_script() );

    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }

    if ( @player == null ) {
      return;
    }

    origin_x = player.x();
    origin_y = player.y();

    // start the player on a random point around the circle
    angle = _step + srand_range( 0, 5 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    angle = _step;

    frames = 0;

    initialized = false;
  }
}
