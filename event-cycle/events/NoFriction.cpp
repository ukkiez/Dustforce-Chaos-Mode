#include "../CycleEvent.cpp";

class NoFriction : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 80, "No Friction" );
  }

  scene@ g;
  dustman@ player;

  bool initialized = false;

  float idle;
  float skid;
  float land;
  float roof;
  float skid_threshold;

  NoFriction() {}

  void step( int entities ) {}
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

    idle = player.idle_fric();
    skid = player.skid_fric();
    land = player.land_fric();
    roof = player.roof_fric();
    skid_threshold = player.skid_threshold();

    player.idle_fric( 0 );
    player.skid_fric( 0 );
    player.land_fric( 0 );
    player.roof_fric( 0 );
    player.skid_threshold( 0 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    player.idle_fric( idle );
    player.skid_fric( skid );
    player.land_fric( land );
    player.roof_fric( roof );
    player.skid_threshold( skid_threshold );

    initialized = false;
  }
}
