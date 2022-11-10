#include "./SeedGenerator.cpp";

class Random {
  SeedGenerator@ seed_generator = SeedGenerator();

  bool _Random_initialized = false;

  void try_init() {
    if ( !_Random_initialized ) {
      seed_generator.init();
      _Random_initialized = true;
    }
  }

  uint srandom() {
    try_init();

    srand( seed_generator.generate_seed() );
    return rand();
  }

  float srandom_flt() {
    try_init();

    srand( seed_generator.generate_seed() );
    return float( ( rand() * 16807 ) % 2147483647 ) / float( 0x7FFFFFFF ) + 0.000000000233;
  }

  // returns between min and max inclusive
  int srand_range( int min, int max ) {
    try_init();

	  return min + ( srandom() % ( max - min + 1 ) );
  }

  // returns between min and max exclusive
	float srand_range_flt( float min, float max ) {
    try_init();

		return srandom_flt() * ( max - min ) + min;
	}
}
