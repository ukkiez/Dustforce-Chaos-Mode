#include "../../QueueEvent.cpp";

class EnemyBarrelizer : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 50, 1, "Enemizer", "Barrelizer" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  bool spawned = false;

  EnemyBarrelizer() {}

  void step( int entities ) {
    if ( !spawned ) {
      uint barrels = 0;
      for ( int i = 0; i < entities; i++ ) {
        entity@ e = entity_by_index( i );

        if ( @e != null ) {
          if ( e.is_same( player.as_entity() ) ) {
            continue;
          }

          if ( @e.as_controllable() == null ) {
            continue;
          }

          if ( e.as_controllable().state() >= 27 ) {
            // these states are mostly for cleansed entities; in other words,
            // don't raise enemies from the dead back as slime barrels
            continue;
          }

          if ( e.type_name() != "enemy_slime_barrel" && e.type_name() != "hittable_apple" ) {
            entity@ barrel = create_entity( "enemy_slime_barrel" );

            barrel.x( e.x() );
            barrel.y( e.y() );
            g.add_entity( barrel );
            g.remove_entity( e );

            entity@ ai = create_entity( "AI_controller" );
            ai.x( barrel.x() );
            ai.y( barrel.y() );

            varstruct@ vars = ai.vars();
            vararray@ nodes = vars.get_var( "nodes" ).get_array();
            vararray@ nodes_wait_time = vars.get_var( "nodes_wait_time" ).get_array();

            nodes.resize( 2 );
            nodes_wait_time.resize( 2 );
            nodes.at( 0 ).set_vec2( barrel.x(), barrel.y() );
            nodes_wait_time.at( 0 ).set_int32( 0 );

            nodes.at( 1 ).set_vec2( barrel.x() + 100, barrel.y() );
            nodes_wait_time.at( 1 ).set_int32( 0 );

            g.add_entity( ai );
            ai.vars().get_var( "puppet_id" ).set_int32( barrel.id() );

            barrels++;

            if ( barrels == 6 ) {
              // create a maximum of 6 barrels
              break;
            }
          }
        }
      }
    }

    spawned = true;
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

    spawned = false;

    initialized = false;
  }
}
