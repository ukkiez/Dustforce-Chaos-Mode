#include "../../CycleEvent.cpp";

// all attacks become a super instead
class SuperOnly : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 100, "Super Only" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  SuperOnly() {}

  void step( int entities ) {
    // infinite skill_combo
    player.skill_combo( player.skill_combo_max() );

    if ( player.attack_state() == 1 || player.attack_state() == 2 ) {
      player.attack_state( 3 );
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

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
