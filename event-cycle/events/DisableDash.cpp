#include "../../lib/Random.cpp";
#include "../../lib/util/text.cpp";

#include "../CycleEvent.cpp";

class DisableDash : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 8, "Disable Dash" );
  }

  scene@ g;
  dustman@ player;

  textfield@ tf;
  array<uint> COLOURS = {
    0x11FFFFFF,
    0x22FFFFFF,
    0x33FFFFFF,
    0x44FFFFFF,
    0x55FFFFFF,
    0x66FFFFFF,
    0x77FFFFFF,
    0x88FFFFFF,
    0x99FFFFFF,
    0xAAFFFFFF,
    0xBBFFFFFF,
  };

  uint colour_index = 0;
  uint text_display_time = 0;

  bool initialized = false;

  DisableDash() {
    @tf = create_textfield();
    tf.set_font( "Caracteres", 26 );
    tf.text( "Dash Disabled" );
    tf.colour( 0xBBFFFFFF );
  }

  void step( int entities ) {
    // permanently override dash/fall intent
    player.dash_intent( 0 );
    player.fall_intent( 0 );

    if ( @tf != null ) {
      text_display_time++;
    }
  }
  void draw( float sub_frame ) {
    draw_text( true, tf, player.x(), player.y() - ( 120 * player.scale() ) );
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

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    colour_index = 0;
    text_display_time = 0;

    initialized = false;
  }
}
