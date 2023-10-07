#include "../CycleEvent.cpp";

// #include "./BecomeBook.cpp";
#include "./BuilderMode.cpp";
#include "./CharacterSwap.cpp";
#include "./Decorator.cpp";
#include "./DisableDash.cpp";
#include "./Disco.cpp";
#include "./EnemyKnockback/index.cpp";
#include "./ExplosionMode.cpp";
#include "./FrankerZMode.cpp";
#include "./InfiniteAircharges.cpp";
#include "./LaserMode/index.cpp";
#include "./Letterbox.cpp";
#include "./MinecraftMode.cpp";
#include "./NoFriction.cpp";
#include "./PlayerScale.cpp";
#include "./TauntAndDie.cpp";
#include "./TiltedCamera.cpp";
#include "./TimeWarp.cpp";

#include "./Unplayable/DashForever.cpp";
#include "./Unplayable/DashMacro.cpp";
#include "./Unplayable/HalfDash.cpp";
#include "./Unplayable/Hesitant.cpp";
#include "./Unplayable/InstantAttack.cpp";
#include "./Unplayable/JumpMacro.cpp";
#include "./Unplayable/NoClip.cpp";
#include "./Unplayable/NoWallrun.cpp";
#include "./Unplayable/Pogo.cpp";
#include "./Unplayable/Rotator.cpp";
#include "./Unplayable/Runner.cpp";
#include "./Unplayable/Superhot.cpp";
#include "./Unplayable/SuperOnly.cpp";
#include "./Unplayable/SupersDontHitEnemies.cpp";
#include "./Unplayable/WorthMode.cpp";

#include "./UkkiezMode.cpp";

array<CycleEvent@> get_cycle_events( bool DEBUG_MODE ) {
  if ( DEBUG_MODE ) {
    // putting events in here will only queue these, and give their configs a
    // 100 weight, guaranteeing them to be available to be picked every round
    // (though still not twice in a row, and retaining the existing limit of
    // concurrent number of events)
    return array<CycleEvent@> = {
      LaserMode(),
    };
  }

  return array<CycleEvent@> = {
    // don't use become book for the moment, as it has crashing behaviour;
    // not anymore with super, but it does with swapping characters
    // BecomeBook(),

    BuilderMode(),
    CharacterSwap(),
    Decorator(),
    DisableDash(),
    Disco(),
    EnemyKnockback(),
    ExplosionMode(),
    FrankerZMode(),
    InfiniteAircharges(),
    LaserMode(),
    Letterbox(),
    MinecraftMode(),
    NoFriction(),
    PlayerScale(),
    TauntAndDie(),
    TiltedCamera(),
    TimeWarp(),

    // Unplayable
    DashForever(),
    DashMacro(),
    HalfDash(),
    Hesitant(),
    InstantAttack(),
    JumpMacro(),
    NoClip(),
    NoWallrun(),
    Pogo(),
    Rotator(),
    Superhot(),
    SuperOnly(),
    SupersDontHitEnemies(),
    Runner(),
    WorthMode(),

    UkkiezMode(),
  };
}
