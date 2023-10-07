#include "../QueueEvent.cpp";

class Misinput : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 100, 0, "Misinput" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  Misinput() {}

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

    int roll = srand_range( 0, 3 );
    switch ( roll ) {
      case 0:
        if ( player.state() != 9 && !player.ground() ) {
          player.dash_intent( 1 );

          break;
        }
      // pass-through
      case 1:
        if ( player.state() != 8 ) {
          if ( player.ground() ) {
            player.jump_intent( 1 );
          }
          else {
            if ( player.dash_max() <= 0 ) {
              player.dash_max( 1 );
            }

            player.jump_intent( 1 );
          }

          break;
        }
      case 2:
        player.light_intent( 1 );
        break;
      case 3:
        player.heavy_intent( 1 );
        break;
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
