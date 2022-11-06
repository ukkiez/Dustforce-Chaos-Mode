void draw_text( textfield@ text, float x, float y, float scale_x = 1.0, float scale_y = 1.0, float rotation = 0.0 ) {
  if ( text.text() != "" ) {
    text.draw_hud( 22, 22, x, y, scale_x, scale_y, rotation % 360 );
  }
}
