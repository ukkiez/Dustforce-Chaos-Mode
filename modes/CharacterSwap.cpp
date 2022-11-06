#include "../lib/Random.cpp";
#include "./Mode.cpp";

#include "../data/characters.cpp";

class CharacterSwap : Mode, Random {
  ModeConfig get_mode_config() {
    return ModeConfig( 50, "Character Swap" );
  }

	script@ script;
  scene@ g;
  dustman@ player;

  bool initialized = false;

  uint iteration;

  bool roulette = false;

  uint duration = 0;
  uint interval = 6;
  uint index = 0;

  CharacterSwap() {}

  void step( int entities ) {
    if ( roulette ) {
      // swap between janitors every interval
      if ( ( duration % interval ) == 0 ) {
        string character = JANITORS[ index ];
        player.character( character );

        duration = 0;
        index++;

        if ( index >= JANITORS.length ) {
          index = 0;
        }
      }

      duration++;
    }
  }

  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @script = cast<script@>( get_script() );
    iteration = script.iteration;

    @g = get_scene();
    @player = controller_controllable( 0 ).as_dustman();

    if ( srandom() % 2 == 0 ) {
      // start the roulette, which constantly changes your character every X
      // interval in step()
      roulette = true;
      initialized = true;
      return;
    }

    // make sure not to swap to the same character the player already is
    array<string> swappable_characters = {};
    for ( uint i = 0; i <= ALL_CHARACTERS.length-1; i++ ) {
      if ( player.character() == ALL_CHARACTERS[ i ] ) {
        continue;
      }

      swappable_characters.insertLast( ALL_CHARACTERS[ i ] );
    }

    string character = swappable_characters[ srand_range( 0, swappable_characters.length-1 ) ];
    player.character( character );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    if ( roulette ) {
      duration = 0;
      index = 0;
      roulette = false;
    }

    if ( BOSSES.find( player.character() ) >= 0 || V_BOSSES.find( player.character() ) >= 0 ) {
      // return the player to their original character, as we can't keep them a
      // boss character if they want to actually SS the level (all assuming they
      // didn't start as a boss character)
      player.character( "default" );
    }

    initialized = false;
  }
}
