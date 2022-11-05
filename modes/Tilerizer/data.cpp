class Layout {
  array<int> x;
  array<int> y;
  uint type;
  uint layer;
  uint sprite_set;
  uint sprite_tile;
  uint palette;

  Layout( array<int> x, array<int> y, uint type, uint layer = 19, uint sprite_set = 1, uint sprite_tile = 1, uint palette = 1 ) {
    this.x = x;
    this.y = y;
    this.type = type;
    this.layer = layer;
    this.sprite_set = sprite_set;
    this.sprite_tile = sprite_tile;
    this.palette = palette;
  }
}

const array<Layout@> layouts = {};

int tile( int n ) {
  return n * 48;
}
