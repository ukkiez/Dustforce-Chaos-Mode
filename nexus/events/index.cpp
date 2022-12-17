#include "../NexusEvent.cpp";

#include "./ChangeDoorTypes.cpp";
#include "./FastDoors.cpp";
#include "./OpenClose.cpp";
#include "./ShyDoors.cpp";
#include "./SlowDoors.cpp";
#include "./SwapDoors.cpp";
#include "./TeleportToDoor.cpp";

array<NexusEvent@> get_nexus_events( bool DEBUG_MODE ) {
  if ( DEBUG_MODE ) {
    return array<NexusEvent@> = {
      ChangeDoorTypes(),
    };
  }

  return array<NexusEvent@> = {
    ChangeDoorTypes(),
    FastDoors(),
    OpenClose(),
    ShyDoors(),
    SlowDoors(),
    SwapDoors(),
    TeleportToDoor(),
  };
}
