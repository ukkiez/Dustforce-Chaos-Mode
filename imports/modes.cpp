#include "../modes/Mode.cpp";

#include "../modes/Audioizer/LovelyTune.cpp";
#include "../modes/Audioizer/RandomSfx.cpp";

#include "../modes/Big.cpp";
#include "../modes/CharacterSwap.cpp";
#include "../modes/DisableAttack.cpp";
#include "../modes/DisableDash.cpp";
#include "../modes/DisableJump.cpp";
#include "../modes/Disco.cpp";

#include "../modes/Enemizer/EnemyBarrelizer.cpp";
#include "../modes/Enemizer/EnemyScaler.cpp";
#include "../modes/Enemizer/EnemyTimeWarp.cpp";

#include "../modes/InfiniteAircharges.cpp";
#include "../modes/Letterbox.cpp";
#include "../modes/MaxSuper.cpp";
#include "../modes/MinecraftMode.cpp";
#include "../modes/NoFriction.cpp";
#include "../modes/PropSwap/index.cpp";
#include "../modes/PolishDriver.cpp";
#include "../modes/SpawnApples.cpp";
#include "../modes/SussyPorcupine.cpp";

#include "../modes/Tilerizer/SpawnBlock.cpp";
#include "../modes/Tilerizer/SpawnZip.cpp";
#include "../modes/Tilerizer/SwapSprites.cpp";

#include "../modes/TauntAndDie.cpp";
#include "../modes/TimeWarp.cpp";
#include "../modes/Tiny.cpp";

#include "../modes/Unplayable/DashMacro.cpp";
#include "../modes/Unplayable/Rotator.cpp";

array<Mode@> get_modes() {
  return array<Mode@> = {
    // Audioizer
    LovelyTune(),
    RandomSfx(),

    Big(),
    CharacterSwap(),
    DisableAttack(),
    DisableDash(),
    DisableJump(),
    Disco(),

    // Enemizer
    // EnemyBarrelizer(),
    EnemyScaler(),
    EnemyTimeWarp(),

    InfiniteAircharges(),
    Letterbox(),
    MaxSuper(),
    MinecraftMode(),
    NoFriction(),
    PolishDriver(),
    PropSwap(),
    SpawnApples(),
    SussyPorcupine(),

    // Tilerizer
    SpawnBlock(),
    SpawnZip(),
    SwapSprites(),

    TauntAndDie(),
    TimeWarp(),
    Tiny(),

    // Unplayable
    DashMacro(),
    Rotator(),
  };
}
