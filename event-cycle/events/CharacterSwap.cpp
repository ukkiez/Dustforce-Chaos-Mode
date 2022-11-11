#include "../../lib/Random.cpp";
#include "../../lib/data/characters.cpp";

#include "../CycleEvent.cpp";

class CharacterSwap : CycleEvent, Random {
  CycleEventConfig get_config() {
    return CycleEventConfig( 50, "Character Swap" );
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

    // @script = cast<script@>( get_script() );
    // iteration = script.iteration;

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

    // make sure we're not spreading dust as boss characters, as that often
    // mostly ruins any fun, as it means you just have to wait around and do
    // nothing until you get swapped back again for fear of spreading dust;
    // don't check for the character, because sometimes player.character() isn't
    // actually even accurate
    player.filth_type( 0 );

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

    initialized = false;
  }
}
