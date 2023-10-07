#include "../CycleEvent.cpp";
#include "../../lib/math.cpp";

class VortexEnemy {
  uint id;
  float x;
  float y;
  float angle;
  float radius;
  float radius_increase;

  VortexEnemy( uint id, float x, float y, float angle, float radius, float radius_increase ) {
    this.id = id;
    this.x = x;
    this.y = y;
    this.angle = angle;
    this.radius = radius;
    this.radius_increase = radius_increase;
  }
}

// enemies start circling around the player once they get close enough
class Vortex : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 200, "Vortex" );
  }

  scene@ g;
  dustman@ player;

  script@ _script;

  // circle
  uint segments = 60;
  float _step = PI2 / segments;

  float starting_radius = 400;
  float radius_min = 50;
  float radius_max = 500;
  float radius_increase = 3;

  array<VortexEnemy@> vortex = {};

  uint frames = 0;

  bool initialized = false;

  Vortex() {}

  void step( int entities ) {
    if ( _script.turbo_mode ) {
      return;
    }

    int enemy_count = g.get_entity_collision( player.y() - 400, player.y() + 400, player.x() - 400, player.x() + 400, 1 );
    if ( enemy_count > 0 ) {
      for ( int i = 0; i < enemy_count; i++ ) {
        entity@ enemy = g.get_entity_collision_index( i );

        bool broke = false;
        for ( uint j = 0; j < vortex.length; j++ ) {
          if ( vortex[ j ].id == enemy.id() ) {
            broke = true;
            break;
          }
        }
        if ( broke ) {
          // this enemy is already in the vortex
          continue;
        }

        float playerEnemyRadius = distance( player.x(), player.y(), enemy.x(), enemy.y() );
        float radius = starting_radius;
        if ( playerEnemyRadius < starting_radius ) {
          radius = playerEnemyRadius;
        }
        float angle = angleFromCos( enemy.x(), player.x(), distance( player.x(), player.y(), enemy.x(), enemy.y() ) );

        // for some reason we need to inverse the angles depending on the point
        // relation between the enemy and the player, but my mathematics are so
        // embarrassing I don't have an inkling as to why and I can't be
        // bothered at this point to figure it out
        if ( enemy.y() > player.y() ) {
          vortex.insertLast( VortexEnemy( enemy.id(), enemy.x(), enemy.y(), angle, radius, -radius_increase ) );
        }
        else {
          vortex.insertLast( VortexEnemy( enemy.id(), enemy.x(), enemy.y(), -angle, radius, -radius_increase ) );
        }
      }
    }

    if ( vortex.length > 0 ) {
      runVortex();
    }

    frames++;
  }

  void runVortex() {
    if ( vortex.length <= 0 ) {
      return;
    }

    // array<int> removable_enemy_ids = {};

    for ( uint i = 0; i < vortex.length; i++ ) {
      entity@ enemy = entity_by_id( vortex[ i ].id );
      if ( @enemy == null ) {
        // removable_enemy_ids.insertLast( vortex[ i ].id );

        // enemy no longer exists
        continue;
      }

      VortexEnemy@ v_enemy = vortex[ i ];

      float x = player.x() + ( v_enemy.radius * cos( v_enemy.angle ) );
      float y = player.y() + ( v_enemy.radius * sin( v_enemy.angle ) );
      enemy.x( x );
      enemy.y( y );

      v_enemy.angle += _step;
      v_enemy.radius += v_enemy.radius_increase;
      if ( v_enemy.radius <= radius_min ) {
        v_enemy.radius_increase = radius_increase;
      }
      else if ( v_enemy.radius >= radius_max ) {
        v_enemy.radius_increase = -radius_increase;
      }
    }
  }

  float angleFromCos( float x, float origin_x, float radius ) {
    return acos( ( x - origin_x ) / radius );
  }
  float angleFromSin( float y, float origin_y, float radius ) {
    return asin( ( y - origin_y ) / radius );
  }

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();

    // get the main script object
    @_script = cast<script@>( get_script() );

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

    frames = 0;

    initialized = false;
  }
}
