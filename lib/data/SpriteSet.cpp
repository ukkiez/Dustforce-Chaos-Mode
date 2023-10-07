// crude object to use for knowing how many tiles are in a given set, so that we
// can pull one out at random; note that the sets and tile indexes start at 1
class SpriteSet {
  int index;
  int size;

  SpriteSet( int index, int size ) {
    this.index = index;
    this.size = size;
  }
}

const array<SpriteSet@> SPRITE_SETS = {
  // Mansion
  SpriteSet( 1, 20 ),
  // Forest
  SpriteSet( 2, 12 ),
  // City
  SpriteSet( 3, 5 ),
  // Lab
  SpriteSet( 4, 8 ),
  // Tutorial
  SpriteSet( 5, 1 ),
};
