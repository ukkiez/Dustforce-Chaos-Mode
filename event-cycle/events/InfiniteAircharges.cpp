#include "../CycleEvent.cpp";

class InfiniteAircharges : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 70, "Infinite Aircharges" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  InfiniteAircharges() {}

  void step( int entities ) {
    if ( @player == null ) {
      return;
    }

    if ( player.dash() <= 0 ) {
      // just always give the player max aircharges
      player.dash( player.dash_max() );
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

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
