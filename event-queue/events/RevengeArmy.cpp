#include "../QueueEvent.cpp";

class RevengeArmy : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 150, 60, "Revenge Army" );
  }

  scene@ g;
  dustman@ player;

  uint interval = 5;

  int count = 0;

  uint frames = 0;

  array<string> enemy_types = {
    "enemy_stonebro",
    "enemy_stoneboss",
    "enemy_trash_tire",
    "enemy_hawk",
    "enemy_scrolls",
    "enemy_book",
    "enemy_wolf",
    "enemy_trash_can",
    "enemy_bear",
  };

  string enemy_type = "";

  bool initialized = false;

  RevengeArmy() {}

  void step( int entities ) {
    if ( frames % interval == 0 ) {
      spawn_enemy( player.x() + srand_range_flt( -500, 500 ), player.y() - srand_range_flt( 100, 550 ) );
    }

    frames++;
  }

  void spawn_enemy( float x, float y ) {
    string enemy_type = enemy_types[ srand_range( 0, enemy_types.length - 1 ) ];

    entity@ enemy = create_entity( enemy_type );

    enemy.x( x );
    enemy.y( y );
    g.add_entity( enemy );

    entity@ ai = create_entity( "AI_controller" );
    ai.x( enemy.x() );
    ai.y( enemy.y() );

    varstruct@ vars = ai.vars();
    vars.get_var( "puppet_id" ).set_int32( enemy.id() );
    vararray@ nodes = vars.get_var( "nodes" ).get_array();
    vararray@ nodes_wait_time = vars.get_var( "nodes_wait_time" ).get_array();

    nodes.resize( 1 );
    nodes_wait_time.resize( 1 );
    nodes.at( 0 ).set_vec2( enemy.x(), enemy.y() );
    nodes_wait_time.at( 0 ).set_int32( 0 );

    g.add_entity( ai );
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

    interval = srand_range( 2, 8 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    uint interval = 5;
    int count = 0;
    uint frames = 0;

    initialized = false;
  }
}
