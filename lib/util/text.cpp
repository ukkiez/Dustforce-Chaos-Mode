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
