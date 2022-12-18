#include "../CycleEvent.cpp";

class PlayerScale : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 100, "Tiny/Beeg Mode" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  PlayerScale() {}

  void step( int entities ) {}
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

    // randomly choose between scaling up or down
    if ( srandom() % 2 == 0 ) {
      player.scale( srand_range_flt( 0.4, 0.6 ), false );
    }
    else {
      player.scale( 1.5, false );
    }

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    player.scale( 1, false );

    initialized = false;
  }
}
