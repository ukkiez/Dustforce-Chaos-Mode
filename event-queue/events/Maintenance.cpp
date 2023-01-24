#include "../QueueEvent.cpp";

class Maintenance : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 75, 100, "Surprise Maintenance" );
  }

  string loading_text_start = "Sorry, hold on...";
  string loading_text_finish = "Unloading loading bar...";

  array<string> loading_text = {
    "Uninstalling MacOS...",
    "Mining Bitcoin...",
    "Hiding body...",
    "Multiplying lag...",
    "Refactoring garbage code...",
    "Publishing ramen...",
    "Splicing run...",
    "Pause buffering...",
    "Creating Dashjump Macro...",
    "Giving 1 heart on Atlas...",
    "Crashing the game...",
    "Installing Trojan...",
    "Updating Windows...",
    "Looking in Dustforce rulebook...",
    "Blaming Dustmod...",
    "DDOS'ing Dustkid...",
  };

  script@ _script;
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

  uint bar_increase_interval = 5;

  uint current_text_interval = 10;

  uint start_delay = 120;

  int lag_array_length = 300000;
  array<int> lag_array( 0 );

  uint frames = 0;

  bool done = false;
  uint end_frames = 0;
  uint end_frames_max = 60;

  bool draw_screen_fader = false;

  Maintenance() {
    @main_text = create_textfield();
    main_text.set_font( "Caracteres", 92 );
    main_text.text( "Maintenance" );
    main_text.colour( 0xDDFFFFFF );

    @subtext = create_textfield();
    subtext.set_font( "ProximaNovaReg", 58 );
    subtext.text( loading_text_start );
    subtext.colour( 0xDDFFFFFF );

    @g = get_scene();
  }

  void step( int entities ) {
    if ( done ) {
      return;
    }

    frames++;

    if ( frames < start_delay ) {
      return;
    }

    if ( main_text.text() != "Loading..." ) {
      main_text.text( "Loading..." );
      subtext.set_font( "ProximaNovaReg", 36 );
    }

    if ( bar_width >= empty_bar_x2 ) {
      if ( end_frames >= end_frames_max ) {
        done = true;
        return;
      }

      // keep lagging, but just a bit less
      for ( uint i = 0; i < ( lag_array.length / 4 ); i++ ) {}

      end_frames++;
      return;
    }

    for ( uint i = 0; i < lag_array.length; i++ ) {}

    if ( frames % bar_increase_interval == 0 ) {
      bar_width += srand_range( 1, 60 );
    }

    if ( bar_width >= empty_bar_x2 ) {
      bar_width = empty_bar_x2;
      subtext.text( loading_text_finish );
      return;
    }

    if ( loading_text.length > 0 && frames > 25 && frames % current_text_interval == 0 ) {
      int index = srand_range( 0, loading_text.length-1 );
      subtext.text( loading_text[ index ] );
      loading_text.removeAt( index );

      current_text_interval = srand_range( 15, 45 );
    }
  }

  void draw( float sub_frame ) {
    if ( done ) {
      return;
    }

    g.draw_rectangle_hud(
      20, 2, bar_back_x1, bar_back_y1, bar_back_x2, bar_back_y2, 0, BAR_BACK_COLOUR
    );
    g.draw_rectangle_hud(
      20, 2, empty_bar_x1, empty_bar_y1, bar_width, empty_bar_y2, 0, EMPTY_BAR_COLOUR
    );

    draw_text( main_text, 0, -100 );
    draw_text( subtext, 0, 125 );

    if ( draw_screen_fader ) {
      // draw an opaque black rectangle across the entire screen to darken it
      g.draw_rectangle_hud(
        20, 1, -800, -450, 800, 450, 0, 0x22000000
      );
    }
  }

  void initialize() {
    @g = get_scene();

    // get the main script object
    @_script = cast<script@>( get_script() );

    if ( !_script.turbo_mode ) {
      lag_array.resize( lag_array_length );
      draw_screen_fader = true;
    }

    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }

    if ( @player == null ) {
      return;
    }
  }
}
