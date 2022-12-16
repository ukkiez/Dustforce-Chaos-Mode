#include "../../../lib/Random.cpp";
#include "../../../lib/data/SFX.cpp";
#include "../../../lib/data/characters.cpp";

#include "../../QueueEvent.cpp";

class RandomSfx : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 1000, -1, "Audioizer", "Random SFX" );
  }

  scene@ g;
  dustman@ player;

  array<string> sfx_in_order = sfx;
  array<string> sfx_to_shuffle;

  bool initialized = false;

  RandomSfx() {
    @g = get_scene();
  }

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    controllable@ c = controller_controllable( 0 );
    @player = c.as_dustman();

    string acronym = string( CHARACTER_ACRONYMS[ player.character() ] );
    string prefix = "sfx_" + acronym;

    // add the character-specific SFX to the pools
    for ( uint i = 0; i < character_sfx.length; i++ ) {
      sfx_in_order.insertLast( prefix + character_sfx[ i ] );
    }

    sfx_to_shuffle = sfx_in_order;

    // use the Fisher-Yates shuffle method to generate a shuffled array
    int j;
    string tmp;
    for ( uint i = sfx_to_shuffle.length - 1; i > 0; i-- ) {
      j = srand_range( 0, i );
      tmp = sfx_to_shuffle[ j ];
      sfx_to_shuffle[ j ] = sfx_to_shuffle[ i ];
      sfx_to_shuffle[ i ] = tmp;
    }

    // override sounds one by one, by index
    for ( uint i = 0; i < sfx_in_order.length; i++ ) {
      g.override_sound( sfx_in_order[ i ], sfx_to_shuffle[ i ], false );
    }

    initialized = true;
  }

  // deactivate the randomization
  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
