class SeedGenerator {
	script@ script;

  uint current_seed;
  uint level_name_seed;

  uint current_frame;

  SeedGenerator() {}

  void init() {
    @script = cast<script@>( get_script() );

    level_name_seed = generate_level_name_seed();
  }

  uint generate_seed() {
    // generate a seed based off of the player's positional history and the
    // number of frames passed by, as well as the level name so we have
    // different starting modes for each map
    uint _seed = ( script.time + generate_position_seed() + level_name_seed );

    if ( current_frame == script.time ) {
      // we're attempting to create multiple seeds within the same step, so use
      // the previous seed as the base of the next
      _seed = uint( _seed + current_seed );
    }
    else {
      // remember that we created a seed this frame, in case we want to generate
      // multiple and need to prevent duplicates
      current_frame = script.time;
    }

    current_seed = _seed;

    return _seed;
  }

  uint generate_position_seed() {
    // take the average of the 5 last positions to generate a seed
    uint _seed = 0;
    uint sum = 0;
    for ( uint i = 0; i < script.position_history.length; i++ ) {
      sum += script.position_history[ i ];
    }
    _seed = uint( sum / script.position_history.length );

    return _seed;
  }

  // seed specific to levels by map name
  uint generate_level_name_seed() {
    uint _seed = 0;
    for ( uint i = 0; i < script.g.map_name().length; i++ ) {
      // seed the generator using the last randomly generated number based on
      // the current index map name character; this way, we will get different
      // results even if two map's characters are the same, or their character
      // byte values would sum up to the same total
      srand( _seed + script.g.map_name()[ i ] );
      _seed = rand();
    }

    return _seed;
  }
}
