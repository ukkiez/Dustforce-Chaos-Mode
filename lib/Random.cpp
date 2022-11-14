#include "./SeedGenerator.cpp";

mixin class Random {
  script@ _Random_script;
  SeedGenerator@ se;
  bool _Random_initialized = false;

  void try_init() {
    if ( !_Random_initialized ) {
      // get the seed generator from the script
      @_Random_script = cast<script@>( get_script() );

      @se = _Random_script.seed_generator;
      _Random_initialized = true;
    }
  }

  uint srandom() {
    try_init();

    srand( se.generate_seed() );
    return rand();
  }

  float srandom_flt() {
    try_init();

    srand( se.generate_seed() );
    return float( ( rand() * 16807 ) % 2147483647 ) / float( 0x7FFFFFFF ) + 0.000000000233;
  }

  // returns between min and max inclusive
  int srand_range( int min, int max ) {
	  return min + ( srandom() % ( max - min + 1 ) );
  }

  // returns between min and max exclusive
	float srand_range_flt( float min, float max ) {
		return srandom_flt() * ( max - min ) + min;
	}
}
