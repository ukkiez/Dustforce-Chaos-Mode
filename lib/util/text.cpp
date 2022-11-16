void draw_text( textfield@ tf, float x, float y, float scale_x = 1.0, float scale_y = 1.0, float rotation = 0.0 ) {
  if ( tf.text() != "" ) {
    tf.draw_hud( 22, 22, x, y, scale_x, scale_y, rotation % 360 );
  }
}

void draw_text( bool world, textfield@ tf, float x, float y, float scale_x = 1.0, float scale_y = 1.0, float rotation = 0.0 ) {
  if ( tf.text() != "" ) {
    tf.draw_world( 22, 22, x, y, scale_x, scale_y, rotation % 360 );
  }
}

const array<uint> TEXT_FADE_COLOURS = {
  0xBBFFFFFF,
  0xAAFFFFFF,
  0x99FFFFFF,
  0x88FFFFFF,
  0x77FFFFFF,
  0x66FFFFFF,
  0x55FFFFFF,
  0x44FFFFFF,
  0x33FFFFFF,
  0x22FFFFFF,
  0x11FFFFFF,
};
