#include "../QueueEvent.cpp";

#include "./Audioizer/LovelyTune.cpp";
#include "./Audioizer/RandomSfx.cpp";

#include "./Enemizer/EnemyBarrelizer.cpp";
#include "./Enemizer/EnemyScaler.cpp";
#include "./Enemizer/EnemyTimeWarp.cpp";

#include "./MaxSuper.cpp";
#include "./PolishDriver.cpp";
#include "./PropSwap/index.cpp";
#include "./SpawnApples.cpp";
#include "./SussyPorcupine.cpp";

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
    };
  }

  return array<QueueEvent@> = {
    // Audioizer
    LovelyTune(),
    RandomSfx(),

    // Enemizer
    // EnemyBarrelizer(),
    EnemyScaler(),
    EnemyTimeWarp(),

    MaxSuper(),
    PolishDriver(),
    PropSwap(),
    SpawnApples(),
    SussyPorcupine(),

    // Tilerizer
    SpawnBlock(),
    SpawnZip(),
    Spikeify(),
    SwapSprites(),
  };
}
