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

  bool boss_character = false;

  bool initialized = false;

  uint iteration;

  CharacterSwap() {}

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @script = cast<script@>( get_script() );
    iteration = script.iteration;

    @g = get_scene();
    @player = controller_controllable( 0 ).as_dustman();

    string character = CHARACTERS[ srand_range( 0, CHARACTERS.length-1 ) ];
    if ( ( array<string> = { "dustwraith", "leafsprite", "trashking", "slimeboss" } ).find( character ) >= 0 ) {
      boss_character = true;
    }
    else {
      boss_character = false;
    }

    player.character( character );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    if ( iteration == script.iteration ) {
      // we likely loaded a checkpoint, hence why the script is attempting to
      // deactive and reinitialize the script, however in this case we don't
      // want to initialize as that would swap the character again
      return;
    }

    if ( boss_character ) {
      // return the player to their original character, as we can't keep them a
      // boss character if they want to actually SS the level (all assuming they
      // didn't start as a boss character)
      player.character( "default" );
    }
    boss_character = false;

    initialized = false;
  }
}
