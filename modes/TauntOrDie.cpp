#include "../lib/util/text.cpp";

#include "./Mode.cpp";

class TauntOrDie : Mode {
  ModeConfig get_mode_config() {
    return ModeConfig( 10, "Mister Simon Says" );
  }

  scene@ g;
  dustman@ player;

  textfield@ text;
  textfield@ timer;

  bool initialized = false;

  bool completed = false;

  int time = 150;

  TauntOrDie() {
    @text = create_textfield();
    text.set_font( "ProximaNovaReg", 100 );
    text.align_horizontal( 0 );
    text.align_vertical( 0 );
    text.text( "press taunt or die" );
    text.colour( 0xFFFFFFFF );
  }

  void step( int entities ) {
    if ( completed ) {
      return;
    }

    if ( player.taunt_intent() == 1 ) {
      completed = true;
    }

    if ( time == 0 ) {
      text.text( "" );
      player.kill( true );
      completed = true;
      return;
    }

    time--;
  }
  void draw( float sub_frame ) {
    if ( completed ) {
      return;
    }

    draw_text( text, 0, 0, 1.0, 1.0 );
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

    time = 150;
    completed = false;
    text.text( "press taunt or die" );

    initialized = false;
  }
}
