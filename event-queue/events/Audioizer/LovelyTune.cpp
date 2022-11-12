#include "../../../lib/Random.cpp";
#include "../../../lib/data/SFX.cpp";

#include "../../QueueEvent.cpp";

class LovelyTune : QueueEvent, Random {
  QueueEventConfig get_config() {
    return QueueEventConfig( 50, -1, false, "Audioizer", "A Lovely Tune" );
  }

  scene@ g;
  dustman@ player;

  audio@ SOUND;

  array<string> sounds = {
    "sfx_hill_beep",
    "sfx_book_attack",
    "sfx_hud_cancel_4",
  };
  array<int> NOTE_LENGTHS_QUARTERS = {
    8,
    15,
    30,
    60,
  };
  array<int> NOTE_LENGTHS_TRIPLETS = {
    60,
    40,
    20,
    10,
  };

  array<int> note_lengths;

  int interval = 60;
  int frames = 0;

  bool initialized = false;

  LovelyTune() {}

  void step( int entities ) {
    if ( @SOUND == null || !SOUND.is_playing() ) {
      @SOUND = g.play_sound( sounds[ srand_range( 0, sounds.length-1 ) ], player.x(), player.y(), 1, false, true );
      SOUND.time_scale( srand_range_flt( 0.6, 1.4 ) );
      interval = note_lengths[ srand_range( 0, note_lengths.length-1 ) ];
    }
    else if ( frames % interval == 0 ) {
      SOUND.stop();
    }

    frames++;
  }
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

    note_lengths = ( srandom() % 2 == 0 ) ? NOTE_LENGTHS_QUARTERS : NOTE_LENGTHS_TRIPLETS;

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    SOUND.stop();

    initialized = false;
  }
}
