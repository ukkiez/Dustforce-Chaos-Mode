class ModeConfig {
  /*
    Weight is a number 1-100 that specifies what percent chance the Mode has to
    be in the pool of activateable Modes, every time the script wants to
    activate new ones.

    The way we pick modes is first to generate a number n from 1 to 100, and
    filter out all modes that have a weight below n. What we are left with is a
    pool of Modes available to be actually activated. Finally, we pick a random
    number of modes from this leftover pool. It's also not possible for Modes to
    be picked twice in a row.
  */
  uint weight;

  // these determine the text and their colour displayed in the on-screen list
  // of active Chaos Modes, whenever they get picked; subtext and colour are
  // optional
  string name;
  string subtext;
  uint colour;

  ModeConfig( uint weight = 1, string name = "", string subtext = "", uint colour = 0xBBFFFFFF ) {
    this.weight = weight;
    this.name = name;
    this.subtext = subtext;
    this.colour = colour;
  }

  ModeConfig() {}
}

interface Mode {
  // force implementation of a function that specifies the mode configuration,
  // which we'll aggregate on script load to get a readily-available list of all
  // configurations
  ModeConfig get_mode_config();

  void step( int entities );
  void draw( float sub_frame );

  void initialize();
  void deactivate();
}
