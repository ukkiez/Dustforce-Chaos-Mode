#include "../../CycleEvent.cpp";

class Pogo : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 200, "Pogo" );
  }

  int MAX_PLAYERS = 4;
  float FRICTION = 0;
  float WAIT_FRAMES = 2;

  scene@ g;
  dustman@ player;

	float yv = 0;
	float yv_checkpoint = 0;
	int waiting = 0;
	bool had_air_charge = true;

  bool initialized = false;

  Pogo() {}

  void step( int entities ) {
		for (int i = 0; i < MAX_PLAYERS; i++) {
			/* Try to get a 'dustman' object handle for each player. */
			controllable@ c = controller_controllable(i);
			if (@c == null) {
				continue;
			}
			dustman@ dm = c.as_dustman();
			if (@dm == null) {
				continue;
			}

			if (dm.land_fric() != FRICTION) {
				dm.land_fric(FRICTION);
			}
			yv_checkpoint = -dm.hop_a();

			if (!had_air_charge && dm.dash() > 0 && !dm.ground() && dm.state() != 19) {
				bounce(dm);
			}
			had_air_charge = dm.dash() > 0;

			if (dm.ground()) {
				if (waiting >= WAIT_FRAMES) {
					// dm.state(10);
					dm.state_timer(0);
					bounce(dm);
				} else if (yv != 0) {
					waiting++;
					// dm.state(10);
				}
			} else if (waiting > 0) {
				// dm.state(8);
				bounce(dm);
			}

			if (waiting == 0) {
				yv = -dm.jump_a();
				if (dm.y_speed() > 0 && abs(dm.y_speed()) > abs(yv)) {
					yv = dm.y_speed();
				}
			}
		}
  }

  void bounce( dustman@ dm ) {
		float j = -yv * 1.25;
		if (dm.jump_intent() == 0) {
			j *= 0.6;
		}
		dm.set_speed_xy(dm.x_speed(), j);
		waiting = 0;
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

    yv = 0;
    yv_checkpoint = 0;
    waiting = 0;
    had_air_charge = true;

    initialized = false;
  }
}
