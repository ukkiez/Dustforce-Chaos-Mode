#include "../CycleEvent.cpp";

#include "./BecomeBook.cpp";
#include "./CharacterSwap.cpp";
#include "./DisableDash.cpp";
#include "./Disco.cpp";
#include "./FrankerZMode.cpp";
#include "./InfiniteAircharges.cpp";
#include "./Letterbox.cpp";
#include "./MinecraftMode.cpp";
#include "./NoFriction.cpp";
#include "./PlayerScale.cpp";
#include "./TauntAndDie.cpp";
#include "./TiltedCamera.cpp";
#include "./TimeWarp.cpp";

#include "./Unplayable/DashMacro.cpp";
#include "./Unplayable/Rotator.cpp";

array<CycleEvent@> get_cycle_events( bool DEBUG_MODE ) {
  if ( DEBUG_MODE ) {
    // putting events in here will only queue these, and give their configs a
    // 100 weight, guaranteeing them to be available to be picked every round
    // (though still not twice in a row, and retaining the existing limit of
    // concurrent number of events)
    return array<CycleEvent@> = {
    };
  }

  return array<CycleEvent@> = {
    BecomeBook(),
    CharacterSwap(),
    DisableDash(),
    Disco(),
    FrankerZMode(),
    InfiniteAircharges(),
    Letterbox(),
    MinecraftMode(),
    NoFriction(),
    PlayerScale(),
    TauntAndDie(),
    TiltedCamera(),
    TimeWarp(),

    // Unplayable
    DashMacro(),
    Rotator(),
  };
}
