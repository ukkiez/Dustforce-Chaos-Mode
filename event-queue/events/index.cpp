#include "../QueueEvent.cpp";

#include "./Audioizer/LovelyTune.cpp";
#include "./Audioizer/RandomSfx.cpp";

#include "./Enemizer/EnemyScaler.cpp";
#include "./Enemizer/EnemyTimeWarp.cpp";

#include "./Bullies.cpp";
#include "./LoopTheLoop.cpp";
#include "./CloseUp.cpp";
#include "./DiscordPing.cpp";
#include "./ForceSuper.cpp";
#include "./Maintenance.cpp";
// #include "./MaxSuper.cpp";
#include "./Misinput.cpp";
#include "./PolishDriver.cpp";
#include "./PropSwap/index.cpp";
#include "./RevengeArmy.cpp";
#include "./SaveCheckpoint.cpp";
#include "./SpawnApples.cpp";
#include "./SussyPorcupine.cpp";
#include "./Tetris.cpp";

#include "./Tilerizer/RemoveTiles.cpp";
#include "./Tilerizer/SpawnBlock.cpp";
#include "./Tilerizer/SpawnZip.cpp";
#include "./Tilerizer/Spikeify.cpp";
#include "./Tilerizer/SwapSprites.cpp";

array<QueueEvent@> get_queue_events( bool DEBUG_MODE ) {
  if ( DEBUG_MODE ) {
    // putting events in here will only queue these, and give their configs a
    // 100 weight, guaranteeing them to be available to be picked every round
    // (though still not twice in a row, and retaining the existing limit of
    // concurrent number of events)
    return array<QueueEvent@> = {
      LoopTheLoop(),
    };
  }

  return array<QueueEvent@> = {
    // Audioizer
    LovelyTune(),
    RandomSfx(),

    // Enemizer
    EnemyScaler(),
    EnemyTimeWarp(),

    CloseUp(),
    DiscordPing(),
    ForceSuper(),
    LoopTheLoop(),
    Maintenance(),
    Misinput(),
    PolishDriver(),
    PropSwap(),
    RevengeArmy(),
    SaveCheckpoint(),
    SpawnApples(),
    SussyPorcupine(),
    Tetris(),

    // Tilerizer
    RemoveTiles(),
    SpawnBlock(),
    SpawnZip(),
    Spikeify(),
    SwapSprites(),

    // // don't add Bullies for the plugin, since AI controlled dustman
    // // entities only work in Dustmod-type levels unfortunately
    // Bullies(),

    // don't add MaxSuper() for now, as the game could crash if you get super
    // while being a book (which we try hard to prevent by removing and
    // disabling super when being a book, but this doesn't circumvent the
    // MaxSuper event of course); NOTE: this seems to not be the case anymore
    // (pending more testing), but the event is kind of lame anyway
    // MaxSuper(),
  };
}
