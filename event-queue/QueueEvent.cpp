#include "../lib/Random.cpp";

class QueueEventConfig {
  /*
    Weight is a number 1-100 that specifies what percent chance the Event has to
    be in the pool of activateable Events, every time the Queue wants to pick a
    new one.

    The way we pick Events is first to generate a number n from 1 to 100, and
    filter out all Events that have a weight below n. What we are left with is a
    pool of Events available to be actually activated. Finally, just pick a
    random Event from this pool
  */
  uint weight;

  // indicates in frames how long an Event should last; 0 indicates there's no
  // deactivate, step or draw function that needs to be run; -1 indicates no
  // preference and the Event Queue should simply pick a random duration
  int duration;

  // these determine the text and their colour displayed in the on-screen list
  // of active Chaos Events, whenever they get picked; subtext and colour are
  // optional
  string name;
  string subtext;
  uint colour;

  QueueEventConfig(
    uint weight = 1,
    int duration = -1,
    string name = "",
    string subtext = "",
    uint colour = 0xBBFFFFFF
  ) {
    this.weight = weight;
    this.duration = duration;
    this.name = name;
    this.subtext = subtext;
    this.colour = colour;
  }

  QueueEventConfig() {}
}

abstract class QueueEvent : Random {
  QueueEventConfig get_config() {
    return QueueEventConfig( 100, -1, "<NAME>", "<SUBTEXT>", 0xBBFFFFFF );
  }

  void step( int entities ) {}
  void draw( float sub_frame ) {}

  void initialize() {}
  void deactivate() {}

  void entity_on_add( entity@ e ) {}
  void entity_on_remove( entity@ e ) {}
}
