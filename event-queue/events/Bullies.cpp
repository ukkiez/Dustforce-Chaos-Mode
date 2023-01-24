#include "../QueueEvent.cpp";

class Bullies : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 100, 605, "An invader", "has entered your world" );
  }

  scene@ g;
  dustman@ player;

  entity@ invader;

  array<string> ENTITIES = {
    "dust_man",
    "dust_girl",
    "dust_kid",
    "dust_worth",
  };

  uint frames = 0;

  bool initialized = false;

  Bullies() {}

  void step( int entities ) {
    if ( frames >= 600 ) {
      g.remove_entity( invader );
    }

    frames++;
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

    // get the main script object
    script@ s = cast<script@>( get_script() );

    @invader = create_entity( ENTITIES[ srand_range( 0, ENTITIES.length-1 ) ] );
    float x = s.position_history_x[ s.position_history_x.length-1 ];
    float y = s.position_history_y[ s.position_history_y.length-1 ];
    if ( x == 0 ) {
      x = player.x() - 500;
    }
    if ( y == 0 ) {
      y = player.y();
    }

    invader.x( x );
    invader.y( y );
    g.add_entity( invader );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
