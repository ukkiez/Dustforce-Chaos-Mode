#include "../CycleEvent.cpp";

#include "./BecomeBook.cpp";
#include "./BuilderMode.cpp";
#include "./CharacterSwap.cpp";
#include "./DisableDash.cpp";
#include "./Disco.cpp";
#include "./EnemyKnockback/index.cpp";
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
#include "./Unplayable/JumpMacro.cpp";
#include "./Unplayable/NoClip.cpp";
#include "./Unplayable/Rotator.cpp";
#include "./Unplayable/WorthMode.cpp";

array<CycleEvent@> get_cycle_events( bool DEBUG_MODE ) {
  if ( DEBUG_MODE ) {
    // putting events in here will only queue these, and give their configs a
    // 100 weight, guaranteeing them to be available to be picked every round
    // (though still not twice in a row, and retaining the existing limit of
    // concurrent number of events)
    return array<CycleEvent@> = {
      EnemyKnockback(),
    };
  }

  return array<CycleEvent@> = {
    BecomeBook(),
    BuilderMode(),

    // don't use character swap for the moment, as it has crashing behaviour at
    // the moment
    // CharacterSwap(),

    DisableDash(),
    Disco(),
    FrankerZMode(),
    InfiniteAircharges(),
    Letterbox(),
    MinecraftMode(),
    NoFriction(),
    TauntAndDie(),
    TiltedCamera(),
    TimeWarp(),

    // Unplayable
    DashMacro(),
    JumpMacro(),
    NoClip(),
    Rotator(),
    WorthMode(),

    // don't use PlayerScale() for now, as it is simply too buggy in combination
    // with other events
    // PlayerScale(),
  };
}
