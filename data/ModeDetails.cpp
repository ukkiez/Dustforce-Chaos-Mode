class ModeDetails {
  // these determine the text and their colour displayed in the on-screen list
  // of active Chaos Modes, whenever they get picked; subtext and colour are
  // optional
  uint weight;
  string name;
  string subtext;
  uint colour;

  ModeDetails( uint weight = 1, string name = "", string subtext = "", uint colour = 0xBBFFFFFF ) {
    this.weight = weight;
    this.name = name;
    this.subtext = subtext;
    this.colour = colour;
  }

  ModeDetails() {}
}

/*
Weight is a number 1-100 which specifies what percent chance the Mode has to be
in the pool of activateable Modes, every time the script wants to activate new
ones.

The way we pick modes is first to generate a number n from 1 to 100, and filter
out all modes that have a weight below n. What we are left with is a pool of
Modes available to be actually activated. Finally, we pick a random number of
modes from this leftover pool. It's also not possible for Modes to be picked
twice in a row
*/

// NOTE: This list is required to be in the same order as the @modes array in
// the main Script class (/main.cpp), as we're simply picking by array index
const array<ModeDetails> mode_details = {
  // Big
  ModeDetails( 50, "Beeg Man Mode" ),

  // CharacterSwap
  ModeDetails( 50, "Character Swap" ),

  // DisableAttack
  ModeDetails( 10, "Attack Disabled" ),
  // DisableDash
  ModeDetails( 10, "Dash Disabled" ),
  // DisableJump
  ModeDetails( 10, "Jump Disabled" ),

  // Disco
  ModeDetails( 35, "FRIDAY FUNKIN'" ),

  // Enemizer/Barrelizer
  ModeDetails( 50, "Enemizer", "Barrelizer" ),
  // Enemizer/EnemyScaler
  ModeDetails( 100, "Enemizer", "Sizerizer" ),
  // Enemizer/EnemyTimeWarp
  ModeDetails( 50, "Enemizer", "Fasterizer" ),

  // InfiniteAircharges
  ModeDetails( 70, "Infinite Aircharges" ),

  // Letterbox
  ModeDetails( 35, "Letterbox" ),

  // MaxSuper
  ModeDetails( 50, "Max Super" ),

  // NoFriction
  ModeDetails( 80, "No Friction" ),

  // PropSwap
  ModeDetails( 100, "Prop Swap" ),

  // SFX
  ModeDetails( 100, "Audioizer", "Random SFX" ),

  // SpawnApples
  ModeDetails( 100, "Skyhawk's Favour" ),

  // Tilerizer/SpawnBlock
  ModeDetails( 60, "Exec Func Ruin Level", "Surprise bloche" ),
  // Tilerizer/SpawnZip
  ModeDetails( 60, "Exec Func Ruin Level", "Surprise Zip" ),

  // SussyPorcupine
  ModeDetails( 25, "AMOGUS", "Sussy Porcupine" ),

  // TimeWarp
  ModeDetails( 50, "Time Warp", ".5x speed" ),

  // Tiny
  ModeDetails( 50, "Tiny Man Mode" ),
};
