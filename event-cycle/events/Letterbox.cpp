#include "../CycleEvent.cpp";

class Letterbox : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 35, "Letterbox" );
  }

  scene@ g;

  [persistent] int letterbox_max_width = 175;

  int letterbox_width = 0;
  bool letterbox_drawn = false;
  bool overlay_disabled = false;

  Letterbox() {
    @g = get_scene();
  }

  void step( int entities ) {}
  void draw( float sub_frame ) {
    if ( !overlay_disabled ) {
      // disable the score overlay, since we're blocking it
      g.disable_score_overlay( true );
      overlay_disabled = true;
    }

    if ( !letterbox_drawn ) {
      uint increase = ( letterbox_max_width - letterbox_width ) / 7;

      if ( increase == 0 ) {
        increase = 1;
      }
      letterbox_width += increase;

      if ( letterbox_width >= letterbox_max_width ) {
        letterbox_drawn = true;
      }
    }

    int screen_radius = 450;

    // draw bars on the top and bottom side (for aesthetics)
    g.draw_rectangle_hud(
      20, 1, -800, -450, 1600, ( -screen_radius + letterbox_width ), 0, 0xFF000000
    );
    g.draw_rectangle_hud(
      20, 1, -800, 450, 1600, ( screen_radius - letterbox_width ), 0, 0xFF000000
    );
  }

  void initialize() {}
  void deactivate() {
    g.disable_score_overlay( false );
    overlay_disabled = false;
  }
}
