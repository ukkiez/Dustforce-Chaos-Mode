#include "../QueueEvent.cpp";

const array<string> loading_text = {
  "oops, sorry Fresh\njust patching some things…",
  "uninstalling MacOS...",
  "mining bitcoin...",
  "oh god, it's on fire.",
  "Unloading loading screen...",
  "Okay, we're good. Sorry about that!",
};

class LoadingScreen : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 1, 60, "try{chaos( )} catch( ){}" );
  }

  scene@ g;
  dustman@ player;

  textfield@ main_text;
  textfield@ subtext;
  int text_index = 0;

  float bar_back_x1 = -420;
  float bar_back_y1 = 0;
  float bar_back_x2 = 420;
  float bar_back_y2 = 65;

  float empty_bar_x1 = bar_back_x1+5;
  float empty_bar_y1 = bar_back_y1+5;
  float empty_bar_x2 = bar_back_x2-5;
  float empty_bar_y2 = bar_back_y2-5;

  float bar_width = empty_bar_x1;

  uint BAR_BACK_COLOUR = 0x99abf1fc;
  uint EMPTY_BAR_COLOUR = 0xAA000000;

  array<int> lag_array( 1000000 );

  int frames = 0;

  bool initialized = false;

  LoadingScreen() {
    @main_text = create_textfield();
    main_text.set_font( "Caracteres", 92 );
    main_text.text( "Loading..." );
    main_text.colour( 0xDDFFFFFF );

    @subtext = create_textfield();
    subtext.set_font( "Caracteres", 36 );
    subtext.text( loading_text[ 0 ] );
    subtext.colour( 0xDDFFFFFF );
  }

  void step( int entities ) {
    puts( "" + frames );
    frames++;
    for ( uint i = 0; i < lag_array.length; i++ ) {}


    if ( bar_width > empty_bar_x2 ) {
      return;
    }

    if ( frames < 20 ) {
      bar_width += ( frames );
    }
    else if ( frames < 30 ) {
      // no-op
    }
    else if ( frames < 50 ) {
      bar_width += ( frames );
    }
    else if ( frames < 60 ) {
      bar_width += ( frames + 1 );
    }

    if ( frames > 15 && frames % 10 == 0 ) {
      subtext.text( loading_text[ ++text_index ] );
    }

    if ( bar_width > empty_bar_x2 ) {
      bar_width = empty_bar_x2;
      subtext.text( "Unloading loading screen..." );
      return;
    }
  }
  void draw( float sub_frame ) {
    g.draw_rectangle_hud(
      20, 2, bar_back_x1, bar_back_y1, bar_back_x2, bar_back_y2, 0, BAR_BACK_COLOUR
    );
    g.draw_rectangle_hud(
      20, 2, empty_bar_x1, empty_bar_y1, bar_width, empty_bar_y2, 0, EMPTY_BAR_COLOUR
    );

    draw_text( main_text, 0, -100 );
    draw_text( subtext, 0, 150 );


    // draw an opaque black rectangle across the entire screen to darken it
    g.draw_rectangle_hud(
      20, 1, -800, -450, 800, 450, 0, 0x22000000
    );
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

    initialized = false;
  }
}
