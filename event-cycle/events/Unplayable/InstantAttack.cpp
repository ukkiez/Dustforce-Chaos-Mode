#include "../../CycleEvent.cpp";

class InstantAttack : CycleEvent, callback_base {
  CycleEventConfig get_config() {
    return CycleEventConfig( 275, "Literally Unplayable", "Turbo Attack" );
  }

  script@ _script;
  scene@ g;
  dustman@ player;

  uint event_cycle_iteration = 0;

  bool callback_setup = false;

  bool initialized = false;

  InstantAttack() {}

  void step( int entities ) {
    if ( !initialized ) {
      return;
    }

    if ( @player == null ) {
      return;
    }

    if ( !callback_setup ) {
      player.on_subframe_end_callback( this, "on_subframe_end", 0 );
      callback_setup = true;
    }
  }

  void initialize() {
    if ( initialized ) {
      return;
    }

    // get the main script object
    @_script = cast<script@>( get_script() );
    event_cycle_iteration = _script.event_cycle.iteration;

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

  void on_subframe_end(dustman@ dm, int) {
    if ( _script.event_cycle.iteration != event_cycle_iteration ) {
      // we can't remove these callbacks after deactivating this cycle event,
      // but we can check if we're currently in a later cycle iteration than
      // when this event was originally initialized
      return;
    }

    if (dm.attack_timer() > 0) {
      hitbox@ h = dm.hitbox();
      if (h !is null) { // light or heavy attacks
        h.state_timer(h.activate_time());
      } else { // super
      }
      dm.freeze_frame_timer(0);
    }

    if (dm.attack_timer() < 0 and dm.attack_timer() != -1) {
      dm.attack_timer(0);
    }
  }


  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
