#include "../NexusEvent.cpp";

#include "./OpenClose.cpp";
#include "./ShyDoors.cpp";
#include "./SlowDoors.cpp";

array<NexusEvent@> get_nexus_events( bool DEBUG_MODE ) {
  if ( DEBUG_MODE ) {
    return array<NexusEvent@> = {
      ShyDoors(),
    };
  }

  return array<NexusEvent@> = {
    OpenClose(),
    ShyDoors(),
    SlowDoors(),
  };
}
