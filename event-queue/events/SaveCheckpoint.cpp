#include "../../lib/util/text.cpp";

#include "../QueueEvent.cpp";

class SaveCheckpoint : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 20, 60, "Save Checkpoint" );
  }

  scene@ g;
  dustman@ player;

  textfield@ tf;

  int text_position_offset_y = 0;
  int text_display_time = 0;

  float text_x;
  float text_y;

  uint colour_index = 0;

  bool _draw_text = false;

  SaveCheckpoint() {}

  void step( int entities ) {
    if ( _draw_text ) {
      if ( text_display_time % 2 == 0 ) {
        if ( text_display_time > 30 ) {
          colour_index++;
          if ( colour_index >= TEXT_FADE_COLOURS.length ) {
            _draw_text = false;
            return;
          }
          tf.colour( TEXT_FADE_COLOURS[ colour_index ] );
        }

        text_position_offset_y+= 2;
      }

      text_display_time++;
    }
  }
  void draw( float sub_frame ) {
    if ( _draw_text ) {
      draw_text( true, tf, text_x, text_y - text_position_offset_y );
    }
  }

  void initialize() {
    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }

    if ( @player == null ) {
      return;
    }

    g.save_checkpoint( int( player.x() ), int( player.y() ) );

    @tf = create_textfield();
    tf.set_font( "Caracteres", 36 );
    tf.text( "Saved Checkpoint!" );
    tf.colour( TEXT_FADE_COLOURS[ 0 ] );
    text_x = player.x();
    text_y = player.y() - ( 120 * player.scale() );

    _draw_text = true;
  }
}
