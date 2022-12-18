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
      SwapDoors(),
    };
  }

  return array<NexusEvent@> = {
    ChangeDoorTypes(),
    // remove FastDoors() for now, as it doesn't really do much
    // FastDoors(),
    OpenClose(),
    ShyDoors(),
    SlowDoors(),
    SwapDoors(),
    TeleportToDoor(),
  };
}
