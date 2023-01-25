#include "../QueueEvent.cpp";

// spawns a big porcupine in front of the player, and plays the "dead body
// reported" SFX from amogus
class SussyPorcupine : QueueEvent, callback_base {
  QueueEventConfig get_config() {
    return QueueEventConfig( 250, 0, "AMOGUS", "Sussy Porcupine" );
  }

  scene@ g;
  dustman@ player;

  entity@ porcupine;
  audio@ SUSSY;

  bool initialized = false;

  SussyPorcupine() {}

  void step( int entities ) {
    if ( @SUSSY == null || !SUSSY.is_playing() ) {
      return;
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

    if ( @player == null ) {
      return;
    }

    @porcupine = create_entity( "enemy_porcupine" );

    int x = pick_spawn_point();

    porcupine.x( player.x() + x );
    porcupine.y( player.y() );
    porcupine.face( -player.face() );
    porcupine.as_hittable().scale( 7 );

    g.add_entity( porcupine );

    porcupine.as_hittable().on_hurt_callback( this, "on_hurt", 0 );

    @SUSSY = g.play_script_stream( "sussy", 3, 0, 0, false, 0.5 );

    initialized = true;
  }

  void on_hurt( controllable@ attacked, controllable@ attacker, hitbox@ attack_hitbox, int arg ) {
    if ( SUSSY.is_playing() ) {
      SUSSY.stop();
    }
  }

  int pick_spawn_point() {
    int x = 750;
    if ( player.face() == -1 ) {
      x = -x;
    }

    tileinfo@ ti = g.get_tile( tile_coord( player.x() + x ), tile_coord( player.y() ) );
    while ( ti.solid() ) {
      if ( player.face() == -1 ) {
        x += tile( 2 );
      }
      else {
        x -= tile( 2 );
      }

      int diff = tile_coord( player.x() ) - x;
      if ( diff > -tile( 3 ) && diff < tile( 3 ) ) {
        x = 0;
        return x;
      }

      @ti = g.get_tile( tile_coord( player.x() + x ), tile_coord( player.y() ) );
    }

    return x;
  }

  int tile_coord( float v ) {
	  return int( floor( v * ( 1.0 / 48 ) ) );
  }

  int tile( int n ) {
    return n * 48;
  }
}
