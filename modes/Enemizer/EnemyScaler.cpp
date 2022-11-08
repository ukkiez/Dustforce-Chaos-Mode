#include "../../lib/Random.cpp";
#include "../Mode.cpp";

class EnemyScaler : Mode, Random {
  ModeConfig get_mode_config() {
    return ModeConfig( 50, "Enemizer", "Sizerizer" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  bool scaled = false;

  EnemyScaler() {}

  void step( int entities ) {
    if ( scaled ) {
      return;
    }

    for ( int i = 0; i < entities; i++ ) {
      entity @e = entity_by_index( i );

      if ( e.id() == player.id() ) {
        continue;
      }

      if ( @e.as_hittable() != null ) {
        // have a 50% chance of shrinking or gigantifying; since the ranges for
        // shrinking and gigantifying is different, we'd otherwise have the
        // probability be too tilted, and most often have entities not be shrunk
        float r;
        if ( srand_range( 0, 1 ) == 0 ) {
          r = srand_range_flt( 0.4, 1 );
        }
        else {
          r = srand_range_flt( 1, 2 );
        }
        e.as_hittable().scale( r );
      }
    }

    scaled = true;
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

    if ( @player == null ) {
      return;
    }

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    scaled = false;

    initialized = false;
  }
}
