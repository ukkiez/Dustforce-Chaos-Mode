#include "../../lib/Random.cpp";
#include "../../lib/data/characters.cpp";

#include "../CycleEvent.cpp";

class CharacterSwap : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 250, "Character Swap" );
  }

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

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }

    if ( srandom() % 4 == 0 ) {
      // start the roulette, which constantly changes your character every X
      // interval in step()
      roulette = true;
      initialized = true;
      return;
    }

    // make sure not to swap to the same character the player already is
    array<string> swappable_characters = {};
    for ( uint i = 0; i <= ALL_JANITORS.length-1; i++ ) {
      if ( player.character() == ALL_JANITORS[ i ] ) {
        continue;
      }

      swappable_characters.insertLast( ALL_JANITORS[ i ] );
    }

    string character = swappable_characters[ srand_range( 0, swappable_characters.length-1 ) ];
    player.character( character );

    // TODO: figure out a way in which scaling the player up or down would not
    // result in boss characters spreading dust for a frame; even if we put
    // filth_type to 0, this still happens

    // // make sure we're not spreading dust as boss characters, as that often
    // // mostly ruins any fun, as it means you just have to wait around and do
    // // nothing until you get swapped back again for fear of spreading dust;
    // // don't check for the character, because sometimes player.character() isn't
    // // actually even accurate
    // player.filth_type( 0 );

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
