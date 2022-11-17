#include "../QueueEvent.cpp";

class MaxSuper : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 70, 0, "Max Super" );
  }

  scene@ g;
  dustman@ player;

  MaxSuper() {}

  void initialize() {
    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();

      if ( player.skill_combo() < 100 ) {
        player.skill_combo( 100 );
      }
    }
  }
}
