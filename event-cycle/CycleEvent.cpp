class CycleEventConfig {
  /*
    Weight is a number 1-100 that specifies what percent chance the Event has to
    be in the pool of activateable Events, every time the Cycler wants to
    activate new ones.

    The way we pick Events is first to generate a number n from 1 to 100, and
    filter out all Events that have a weight below n. What we are left with is a
    pool of Events available to be actually activated. Finally, we pick a random
    number of modes from this leftover pool. It's also not possible for Events
    to be picked twice in a row.
  */
  uint weight;

  // these determine the text and their colour displayed in the on-screen list
  // of active Chaos Events, whenever they get picked; subtext and colour are
  // optional
  string name;
  string subtext;
  uint colour;

  CycleEventConfig( uint weight = 1, string name = "", string subtext = "", uint colour = 0xBBFFFFFF ) {
    this.weight = weight;
    this.name = name;
    this.subtext = subtext;
    this.colour = colour;
  }

  CycleEventConfig() {}
}

interface CycleEvent {
  // force implementation of a function that specifies the event configuration,
  // which we'll aggregate on script load to get a readily-available list of all
  // configurations
  CycleEventConfig get_config();

  void step( int entities );
  void draw( float sub_frame );

  void initialize();
  void deactivate();
}
