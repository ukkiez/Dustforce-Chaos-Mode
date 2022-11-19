#include "../NexusEvent.cpp";

#include "./ShyDoors.cpp";

array<NexusEvent@> get_nexus_events( bool DEBUG_MODE ) {
  if ( DEBUG_MODE ) {
    return array<NexusEvent@> = {
      ShyDoors(),
    };
  }

  return array<NexusEvent@> = {
    ShyDoors(),
  };
}
