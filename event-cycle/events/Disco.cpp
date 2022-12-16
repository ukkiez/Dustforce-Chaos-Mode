#include "../CycleEvent.cpp";

const array<uint> COLOURS = {
  0xFFFF0000,
  0xFF00FF00,
  0xFF0000FF,
  0xFFFFFFFF,
};

class Disco : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 250, "FRIDAY FUNKIN'" );
  }

  scene@ g;
  dustman@ player;

  audio@ FUNKYMUSIC;

  camera@ camera;
  fog_setting@ fog;

  array<uint> original_fog_colours;
  array<float> original_fog_percents;

  uint colour_index = 0;

  uint duration = 0;
  uint interval = 30;

  bool initialized = false;

  Disco() {}

  void step( int entities ) {
    if ( ( duration % interval ) == 0 ) {
      for ( uint i = 0; i <= 20; i++ ) {
        fog.layer_colour( i, COLOURS[ colour_index ] );
        fog.layer_percent( i, 0.1 );
      }

      camera.change_fog( fog, 0 );

      colour_index++;
      if ( colour_index == COLOURS.length-1 ) {
        colour_index = 0;
      }

      duration = 0;
    }

    duration++;
  }
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

    @camera = get_active_camera();
    @fog = camera.get_fog();

    // store each individual original layer colour and fog percent, since just
    // keeping a copy of the old fog trigger does not seem to work; keep in mind
    // this only copies sublayer 10, which we chose because it's the interior
    // tile sublayer
    for ( uint i = 0; i <= 20; i++ ) {
      original_fog_colours.insertLast( fog.colour( i, 10 ) );
      original_fog_percents.insertLast( fog.percent( i, 10 ) );
    }

    @FUNKYMUSIC = g.play_script_stream( "funkin", 3, 0, 0, false, 0.35 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    for ( uint i = 0; i <= 20; i++ ) {
      original_fog_colours.insertLast( fog.colour( i, 10 ) );
      original_fog_percents.insertLast( fog.percent( i, 10 ) );

      fog.layer_colour( i, original_fog_colours[ i ] );
      fog.layer_percent( i, original_fog_percents[ i ] );
    }

    camera.change_fog( fog, 0 );

    FUNKYMUSIC.stop();

    initialized = false;
  }
}
