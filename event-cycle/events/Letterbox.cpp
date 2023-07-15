#include "../../lib/Random.cpp";

#include "../CycleEvent.cpp";

class Letterbox : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 350, "Letterbox", "or pillarbox" );
  }

  scene@ g;

  int screen_radius_y = 450;
  int screen_radius_x = 800;

  int letterbox_max_width = 175;
  int pillarbox_max_width = 250;

  int l_x1 = -800;
  int l_y1 = 450;
  int l_x2 = 1600;

  int p_x1 = 800;
  int p_y1 = -450;
  int p_y2 = 450;

  int width = 0;
  bool drawn = false;

  bool pillarbox = false;

  bool overlay_disabled = false;

  Letterbox() {
    @g = get_scene();
  }

  void step( int entities ) {
    if ( !drawn ) {
      if ( pillarbox ) {
        uint increase = ( pillarbox_max_width - width ) / 7;

        if ( increase == 0 ) {
          increase = 1;
        }
        width += increase;

        if ( width >= pillarbox_max_width ) {
          drawn = true;
        }
      }
      else {
        uint increase = ( letterbox_max_width - width ) / 7;

        if ( increase == 0 ) {
          increase = 1;
        }
        width += increase;

        if ( width >= letterbox_max_width ) {
          drawn = true;
        }
      }
    }
  }
  void draw( float sub_frame ) {
    if ( !overlay_disabled ) {
      // disable the score overlay, since we're blocking it
      g.disable_score_overlay( true );
      overlay_disabled = true;
    }

    if ( pillarbox ) {
      // draw bars on the left and right side
      g.draw_rectangle_hud(
        20, 1, -p_x1, p_y1, ( -screen_radius_x + width ), p_y2, 0, 0xFF000000
      );
      g.draw_rectangle_hud(
        20, 1, p_x1, p_y1, ( screen_radius_x - width ), p_y2, 0, 0xFF000000
      );
    }
    else {
      // draw bars on the top and bottom side
      g.draw_rectangle_hud(
        20, 1, l_x1, -l_y1, l_x2, ( -screen_radius_y + width ), 0, 0xFF000000
      );
      g.draw_rectangle_hud(
        20, 1, l_x1, l_y1, l_x2, ( screen_radius_y - width ), 0, 0xFF000000
      );
    }
  }

  void initialize() {
    pillarbox = ( srandom() % 2 == 0 ) ? true : false;
  }
  void deactivate() {
    g.disable_score_overlay( false );
    overlay_disabled = false;
    width = 0;
    drawn = false;
  }
}
