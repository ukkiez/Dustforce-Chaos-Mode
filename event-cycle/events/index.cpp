#include "../CycleEvent.cpp";

#include "./Big.cpp";
#include "./CharacterSwap.cpp";
#include "./DisableDash.cpp";
#include "./Disco.cpp";

#include "./InfiniteAircharges.cpp";
#include "./Letterbox.cpp";
#include "./MinecraftMode.cpp";
#include "./NoFriction.cpp";

#include "./TauntAndDie.cpp";
#include "./TimeWarp.cpp";
#include "./Tiny.cpp";

#include "./Unplayable/DashMacro.cpp";
#include "./Unplayable/Rotator.cpp";

array<CycleEvent@> get_cycle_events() {
  return array<CycleEvent@> = {
    Big(),
    CharacterSwap(),
    DisableDash(),
    Disco(),

    InfiniteAircharges(),
    Letterbox(),
    MinecraftMode(),
    NoFriction(),

    TauntAndDie(),
    TimeWarp(),
    Tiny(),

    // Unplayable
    DashMacro(),
    Rotator(),
  };
}
