#include "./lib/std.cpp";
#include "./lib/util/text.cpp";

const array<uint> ELEMENT_COLOURS = {
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

class ListElement {
  textfield@ text_field;
  textfield@ subtext_field;

  ListElement( textfield@ text, textfield@ subtext ) {
    @this.text_field = text;
    @this.subtext_field = subtext;
  }

  bool has_subtext() {
    return this.subtext_field.text() != "";
  }
}

class EventList {
  scene@ g;
  dustman@ player;

  // Cycle elements
  array<ListElement@> cycle_elements = {};
  uint cycle_text_display_time = 0;

  // Queue elements
  ListElement@ queue_element;
  uint queue_text_display_time = 0;
  uint text_fadeout_time = 120;
  uint colours_index = 0;

  bool initialized = false;

  EventList() {
    @g = get_scene();
  }

  void init() {
    if ( !initialized ) {
      controllable@ c = controller_controllable( 0 );
      if ( @c == null ) {
        return;
      }

      @player = c.as_dustman();

      initialized = true;
    }
  }

  void step( int entities ) {
    if ( cycle_elements.length > 0 ) {
      cycle_text_display_time++;
    }

    if ( @queue_element != null ) {
      // manage the fade in and fade out of the queue list element

      if ( queue_text_display_time == 0 ) {
        colours_index = ELEMENT_COLOURS.length-1;
      }

      queue_element.text_field.colour( ELEMENT_COLOURS[ colours_index ] );

      if ( queue_element.has_subtext() ) {
        queue_element.subtext_field.colour( ELEMENT_COLOURS[ colours_index ] );
      }

      if ( queue_text_display_time >= text_fadeout_time ) {
        if ( colours_index >= 0 ) {
          if ( queue_text_display_time % 2 == 0 ) {
            if ( colours_index == 0 ) {
              @queue_element = null;
            }
            else {
              colours_index--;
            }
          }
        }
      }

      queue_text_display_time++;
    }
  }

  void draw( float sub_frame ) {
    if ( cycle_elements.length > 0 ) {
      int default_x = -750;
      int default_y = 50;
      int subtext_y_offset = 40;

      if ( cycle_text_display_time < 7 ) {
        default_x -= uint( 300 - ( cycle_text_display_time * 30 ) );
      }

      int y_gap = 100;

      for ( uint i = 0; i < cycle_elements.length; i++ ) {
        int x = default_x;
        int y = ( default_y - ( y_gap * i ) );

        // attempt to dynamically spread out the event text across the screen,
        // so that they don't overlap, depending on how many are active
        draw_text( cycle_elements[ i ].text_field, x, y, 0.8, 0.8 );
        if ( cycle_elements[ i ].has_subtext() ) {
          draw_text( cycle_elements[ i ].subtext_field, x, y + subtext_y_offset, 0.8, 0.8 );
        }
      }
    }

    if ( @queue_element != null ) {
      // draw the queue list element
      int default_x = -750;
      int default_y = 175;
      int subtext_y_offset = 40;

      if ( queue_text_display_time < 7 ) {
        // have the text come moving in from the left
        default_x -= uint( 300 - ( queue_text_display_time * 30 ) );
      }

      draw_text( queue_element.text_field, default_x, default_y );
      if ( queue_element.has_subtext() ) {
        draw_text( queue_element.subtext_field, default_x, default_y + subtext_y_offset );
      }
    }

    // // draw a separator between the list's cycle and queue elements
    // float x1 = g.hud_screen_width( false ) / 2.1;
    // float x2 = g.hud_screen_width( false ) / 4;
    // float y = g.hud_screen_height( false ) / 6.7;
    // g.draw_line_hud( 22, 22, -x1, y, -x2, y, 5, 0x33abf1fc );
  }

  void iterate_event_cycle() {
    // a new event cycle happened, so remove all previously displayed list
    // elements as they will be deactivated
     cycle_elements.resize( 0 );
     cycle_text_display_time = 0;
  }

  void add_queue_element( string text, string subtext ) {
    @queue_element = ListElement( create_text_field( text, true ), create_subtext_field( subtext, true ) );
    queue_text_display_time = 0;
  }

  void add_cycle_element( string text, string subtext ) {
    cycle_elements.insertLast(
      ListElement( create_text_field( text ), create_subtext_field( subtext ) )
    );
  }

  textfield@ create_text_field( string text, bool queue_text = false ) {
    uint colour = 0xBBFFFFFF;
    textfield@ tf = create_textfield();

    if ( queue_text ) {
      tf.set_font( "ProximaNovaReg", 36 );
    }
    else {
      tf.set_font( "ProximaNovaReg", 58 );
    }

    tf.align_horizontal( -1 );
    tf.align_vertical( 0 );
    tf.text( text );
    tf.colour( colour );
    return tf;
  }
  textfield@ create_subtext_field( string text = "", bool queue_text = false ) {
    uint colour = 0xBBFFFFFF;
    textfield@ tf = create_textfield();

    if ( queue_text ) {
      tf.set_font( "ProximaNovaReg", 26 );
    }
    else {
      tf.set_font( "ProximaNovaReg", 42 );
    }

    tf.align_horizontal( -1 );
    tf.align_vertical( 0 );
    tf.text( text );
    tf.colour( colour );
    return tf;
  }
}
