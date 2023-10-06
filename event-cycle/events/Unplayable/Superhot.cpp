#include "../../CycleEvent.cpp";

class Superhot : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 150, "Unplayable", "SUPERHOT" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  int MAX_PLAYERS = 4;

  float TW_SLOW = 0.05f;
  // int INTERP_FRAMES = 10;
  float TW_ACCEL = 0.05f;
  float TW_DECEL = 0.05f;
  int FIRST_FRAME = 55;

  int SPEED_CAP = 2500;
  int SLOW_THRESHOLD = 200;
  float INPUT_BONUS = 5;
  float SLOW_INPUT_BONUS = 3;
  float NO_INPUT_NERF = 0.2f;

  float MAX_TW_CHANGE = 0.04f;

	int frames;
	bool finished;
	int last_input;
	float tw;
	float tw_last;

  Superhot() {}

	void on_level_end() {
		finished = true;

		g.time_warp(1);

		// for (int i = 0; i < MAX_PLAYERS; i++) {
			// /* Try to get a 'dustman' object handle for each player. */
			// controllable@ c = controller_controllable(i);
			// if (@c == null) {
				// continue;
			// }
			// dustman@ dm = c.as_dustman();
			// if (@dm == null) {
				// continue;
			// }

			// dm.time_warp(1);

		// }
	}

	void step(int entities) {
		frames++;

		// if (frames <= FIRST_FRAME) {
			// g.time_warp(interp(frames, 40, FIRST_FRAME, 1, TW_SLOW));

			// for (int i = 0; i < MAX_PLAYERS; i++) {
				// /* Try to get a 'dustman' object handle for each player. */
				// controllable@ c = controller_controllable(i);
				// if (@c == null) {
					// continue;
				// }
				// dustman@ dm = c.as_dustman();
				// if (@dm == null) {
					// continue;
				// }

				// dm.time_warp(interp(frames, 40, 55, 1, PLAYER_TW));

			// }
		// }

		if (!finished && frames > FIRST_FRAME) {
			// if input_detected
				// g.time_warp(interp(frames, last_input, last_input + INTERP_FRAMES, TW_SLOW, 1));

			// if (input_detected()) {
				// tw += TW_ACCEL;
			// } else {
				// tw -= TW_DECEL;
			// }

			// if (tw > 1) {
				// tw = 1;
			// } else if (tw < TW_SLOW) {
				// tw = TW_SLOW;
			// }
			// g.time_warp(tw);

			entity@ e = controller_entity(0);
			if (@e == null) {
				return;
			}
			dustman@ dm = e.as_dustman();
			if (@dm == null) {
				return;
			}

			tw = dm.speed() / SPEED_CAP;
			if (tw < TW_SLOW) {
				tw = TW_SLOW;
			}

			if (input_detected()) {
				tw *= INPUT_BONUS;
				if (dm.speed() < SLOW_THRESHOLD) {
					tw *= SLOW_INPUT_BONUS;
				}
			} else {
				tw *= NO_INPUT_NERF;
			}

			if (abs(tw - tw_last) > MAX_TW_CHANGE) {
				if (tw < tw_last) {
					tw = tw_last - MAX_TW_CHANGE;
				} else {
					tw = tw_last + MAX_TW_CHANGE;
				}
			}

			if (tw < TW_SLOW) {
				g.time_warp(TW_SLOW);
			}
      else if (tw > 1) {
				g.time_warp(1);
			}
      else {
				g.time_warp(tw);
			}

			tw_last = tw;

			// puts("" + tw);


			// for (int i = 0; i < entities; i++) {
				// entity@ e = entity_by_index(i);
				// if (@e == null) {
					// continue;
				// }

				// // check hitbox
				// if (e.type_name() == "hit_box_controller") {
					// hitbox@ hb = e.as_hitbox();
					// controllable@ owner = hb.owner();
					// if (@owner == null) {
						// continue;
					// }
					// dustman@ dm = owner.as_dustman();
					// if (@dm == null) {
						// continue;
					// }

					// e.time_warp(4);
				// }
			// }

			for (int i = 0; i < MAX_PLAYERS; i++) {
				/* Try to get a 'dustman' object handle for each player. */
				controllable@ c = controller_controllable(i);
				if (@c == null) {
					continue;
				}
				dustman@ _dm = c.as_dustman();
				if (@_dm == null) {
					continue;
				}

				// kill if hit by enemy
				if (_dm.stun_timer() != 0) {
					_dm.kill(true);
				}
			}
		}
	}

  bool input_detected() {
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

			if (dm.x_intent() != 0) {
				return true;
			}
			if (dm.y_intent() != 0) {
				return true;
			}
			if (dm.light_intent() != 0) {
				return true;
			}
			if (dm.heavy_intent() != 0) {
				return true;
			}
			if (dm.jump_intent() != 0) {
				return true;
			}
			if (dm.dash_intent() != 0) {
				return true;
			}
			if (dm.fall_intent() != 0) {
				return true;
			}
		}

		return false;
	}

	float interp( float x, float x1, float x2, float y1, float y2 ) {
		float y = (x - x1) * (y2 - y1) / (x2 - x1) + y1;
		if (y1 <= y2 && y < y1) {
			return y1;
		} else if (y1 >= y2 && y > y1) {
			return y1;
		} else if (y2 <= y1 && y < y2) {
			return y2;
		} else if (y2 >= y1 && y > y2) {
			return y2;
		}
		return y;
	}

  void reset_time( int entities ) {
		g.time_warp( 1 );

		for ( int i = 0; i < entities; i++ ) {
			entity@ e = entity_by_index( i );
			if ( @e == null ) {
				continue;
			}

			e.time_warp( 1 );
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

		frames = 0;
		finished = false;
		last_input = 0;
		tw = TW_SLOW;
		tw_last = TW_SLOW;

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    g.time_warp( 1 );

		frames = 0;
		finished = true;
		last_input = 0;
		tw = TW_SLOW;
		tw_last = TW_SLOW;

    initialized = false;
  }
}
