#include "../../lib/util/text.cpp";
#include "../../lib/Random.cpp";

#include "../CycleEvent.cpp";

class TauntAndDie : CycleEvent, Random {
  CycleEventConfig get_config() {
    return CycleEventConfig( 10, "Mr. Simon Says" );
  }

  scene@ g;
  dustman@ player;

  textfield@ text;
  textfield@ timer;

  bool initialized = false;

  bool completed = false;

  int time = 160;

  float text_scale = 1.0;
  float text_scaling = 0.004;
  bool scale_text_up = false;

  // either the prompt will say "press taunt OR die" or "TO die", this variable
  // determines which of the two it will be
  bool press_to_die = false;

  TauntAndDie() {
    @text = create_textfield();
    text.set_font( "ProximaNovaReg", 72 );
    text.align_horizontal( 0 );
    text.align_vertical( 0 );
    text.text( "Press TAUNT or die" );
    text.colour( 0xEEFFFFFF );
  }

  void step( int entities ) {
    if ( completed ) {
      return;
    }

    if ( press_to_die ) {
      if ( player.taunt_intent() == 1 ) {
        player.kill( true );
        text.text( "" );
        completed = true;
      }
    }
    else {
      if ( player.taunt_intent() == 1 ) {
        completed = true;
      }

      if ( time == 0 ) {
        player.kill( true );

        text.text( "" );
        completed = true;
        return;
      }

      time--;
    }

    if ( scale_text_up ) {
      text_scale += text_scaling;
    }
    else {
      text_scale -= text_scaling;
    }

    if ( text_scale <= 0.9 ) {
      scale_text_up = true;
    }
    else if ( text_scale >= 1.0 ) {
      scale_text_up = false;
    }
  }
  void draw( float sub_frame ) {
    if ( completed ) {
      return;
    }

    draw_text( text, 0, 200, text_scale, text_scale );
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

    if ( srandom() % 2 == 0 ) {
      // either the prompt will say "press taunt OR die" or "TO die", this
      // variable determines which of the two it will be; 50% chance for either
      press_to_die = true;
      text.text( "Press TAUNT to die" );
    }
    else {
      text.text( "Press TAUNT or die" );
    }

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    time = 160;
    completed = false;
    press_to_die = false;

    initialized = false;
  }
}
