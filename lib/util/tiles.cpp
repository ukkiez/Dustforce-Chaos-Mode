int tile( int n ) {
  return n * 48;
}

int tile_coord( float v ) {
  return int( floor( v * ( 1.0 / 48 ) ) );
}

int center_coord_on_tile( int v ) {
  return v * 48 + 24;
}

// replace a tile, not setting a new one in empty space
void replace_tile( scene@ g, int x, int y, int sprite_set, int sprite_tile = -1, int type = -1, uint layer = 19, uint palette = 1 ) {
  tileinfo@ ti = g.get_tile( x, y );

  if ( ti.solid() ) {
    g.set_tile(
      x,
      y,
      layer,
      true,
      ( type == -1 ) ? ti.type() : type,
      sprite_set,
      ( sprite_tile == -1 ) ? ti.sprite_tile() : sprite_tile,
      palette
    );
  }
}
