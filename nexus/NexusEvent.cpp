#include "../lib/Random.cpp";

abstract class NexusEvent : Random {
  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {}
  void deactivate() {}
}
