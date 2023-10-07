#include "../../CycleEvent.cpp";

#include "./fog.as";
#include "../../../lib/math.cpp";
#include "../../../lib/geom.cpp";
#include "../../../lib/circle.cpp";
#include "../../../lib/tiles.cpp";

const float HALF_WIDTH = 96;
const float INNER_WIDTH = 70;

class LaserMode : CycleEvent, callback_base {
  CycleEventConfig get_config() {
    return CycleEventConfig( 200, "Alex's Artillery" );
  }

  float knockback = 1600;

  scene@ g;
  controllable@ p;
  dustman@ dm;
  camera@ cam;

  script@ _script;

  bool hit = true;
  hitbox@ h;

  canvas@ c;
  int laser_timer = 0;
  float laser_x;
  float laser_y;
  float laser_a;
  float laser_l;

  float charge_x;
  float charge_y;

  [entity] int normal_fog_trigger;
  [entity] int bright_fog_trigger;

  fog_setting@ normal_fog;
  fog_setting@ bright_fog;
  bool change_fog = false;

  uint event_cycle_iteration = 0;

  bool initialized = false;

  LaserMode() {}

  void checkpoint_load()
  {
      @p = controller_controllable(0);
      @dm = p.as_dustman();
      dm.on_subframe_end_callback(this, "subframe_end_callback", 0);

      float fog_speed;
      int trigger_size;
      get_fog_setting(entity_by_id(normal_fog_trigger), normal_fog, fog_speed, trigger_size);
      get_fog_setting(entity_by_id(bright_fog_trigger), bright_fog, fog_speed, trigger_size);
  }

  void entity_on_add(entity@ e)
  {
      if (e.type_name() == "effect")
      {
          sprites@ spr = e.get_sprites();
          spr.add_sprite_set("script");
          e.set_sprites(spr);
      }

      if (e.type_name() == "filth_ball")
      {
          g.remove_entity(e);
      }
  }

  void step( int entities ) {
    if (laser_timer > 0) --laser_timer;
    if (change_fog)
    {
        change_fog = false;
        cam.change_fog(normal_fog, 0.2);
    }
  }


    void create_laser(float x, float y, float a)
    {
        laser_timer = 20;
        laser_x = x;
        laser_y = y;
        laser_a = a;

        float end_x = x;
        float end_y = y;
        float dx = cos(a);
        float dy = sin(a);
        float l = 0;
        while (true)
        {
            l += 48;
            end_x += 48 * dx;
            end_y += 48 * dy;

            if (l > 5000)
                break;

            float x1 = end_x;
            float x2 = end_x + HALF_WIDTH * dy;
            float x3 = end_x - HALF_WIDTH * dy;
            float y1 = end_y;
            float y2 = end_y - HALF_WIDTH * dx;
            float y3 = end_y + HALF_WIDTH * dx;
            if (g.get_tile(int(floor(x1 / 48)), int(floor(y1 / 48))).solid() and
                g.get_tile(int(floor(x2 / 48)), int(floor(y2 / 48))).solid() and
                g.get_tile(int(floor(x3 / 48)), int(floor(y3 / 48))).solid())
            {
                float _;
                tileinfo@ t1 = g.get_tile(int(floor(x1 / 48)), int(floor(y1 / 48)));
                tileinfo@ t2 = g.get_tile(int(floor(x2 / 48)), int(floor(y2 / 48)));
                tileinfo@ t3 = g.get_tile(int(floor(x3 / 48)), int(floor(y3 / 48)));
                x1 = ((x1 % 48) + 48) % 48;
                x2 = ((x2 % 48) + 48) % 48;
                x3 = ((x3 % 48) + 48) % 48;
                y1 = ((y1 % 48) + 48) % 48;
                y2 = ((y2 % 48) + 48) % 48;
                y3 = ((y3 % 48) + 48) % 48;
                if (point_in_tile(x1, y1, 0, 0, t1.type(), _, _) and
                    point_in_tile(x2, y2, 0, 0, t2.type(), _, _) and
                    point_in_tile(x3, y3, 0, 0, t3.type(), _, _)) break;
            }
        }
        laser_l = l;

        float top = min(y, y + l * dy) - HALF_WIDTH;
        float bottom = max(y, y + l * dy) + HALF_WIDTH;
        float left = min(x, x + l * dx) - HALF_WIDTH;
        float right = max(x, x + l * dx) + HALF_WIDTH;

        array<float> points = {
            x + HALF_WIDTH * dy, y - HALF_WIDTH * dx,
            x - HALF_WIDTH * dy, y + HALF_WIDTH * dx,
            x - HALF_WIDTH * dy + l * dx, y + HALF_WIDTH * dx + l * dy,
            x + HALF_WIDTH * dy + l * dx, y - HALF_WIDTH * dx + l * dy
        };

        bool removed = true;
        while (removed)
        {
            removed = false;
            int n = g.get_entity_collision(top, bottom, left, right, 1);
            for (int i=0; i<n; ++i)
            {
                controllable@ e = g.get_controllable_collision_index(i);
                if (e is null) continue;

                collision@ col = e.hit_collision();
                rectangle@ r = col.rectangle();

                if (point_in_polygon(r.left(), r.top(), points) or
                    point_in_polygon(r.right(), r.top(), points) or
                    point_in_polygon(r.left(), r.bottom(), points) or
                    point_in_polygon(r.right(), r.bottom(), points))
                {
                    g.remove_entity(e.as_entity());

                    entity@ em = create_emitter(46, (r.left() + r.right()) / 2, (r.top() + r.bottom()) / 2, int(r.right() - r.left()), int(r.bottom() - r.top()), 18, 0);
                    g.add_entity(em);
                    @em = create_emitter(46, (r.left() + r.right()) / 2, (r.top() + r.bottom()) / 2, int(r.right() - r.left()), int(r.bottom() - r.top()), 18, 1);
                    g.add_entity(em);
                    dm.dash(dm.dash_max());
                    dm.combo_count(dm.combo_count() + 1);
                    dm.skill_combo(dm.skill_combo() + 1);
                    dm.combo_timer(1);
                    g.play_script_stream("sizzle", 0, 0, 0, false, 1.0);
                    removed = true;
                }
            }
        }
    }

    void draw(float)
    {
        draw_laser(laser_x + rand() % 20, laser_y + rand() % 20, laser_a, laser_l);
    }

  void draw_laser(float x, float y, float a, float l)
    {
        if (h !is null and not hit)
        {
            float w = h.state_timer() / h.activate_time() * HALF_WIDTH;
            float inner_w = max(0.f, w - HALF_WIDTH + INNER_WIDTH);
            drawing::fill_circle(g, 18, 15, charge_x, charge_y, w, 20, 0xffff0000, 0xffff0000);
            drawing::fill_circle(g, 18, 15, charge_x, charge_y, inner_w, 20, 0xffffffff, 0xffffffff);
        }

        if (laser_timer <= 0) return;

        c.reset();

        // c.rotate(a, x, y); // Doesn't work :(

        // c.multiply( 1,  0 , 0,  1, x - x, y - y); // 0
        // c.multiply( 0, -1,  1,  0, x + y, y - x); // 90
        // c.multiply(-1,  0,  0, -1, x + x, y + y); // 180
        // c.multiply( 0,  1, -1,  0, x - y, y + x); // 270

        c.multiply(cos(a), -sin(a), sin(a), cos(a), x - x * cos(a) + y * sin(a), y - x * sin(a) - y * cos(a));

        drawing::fill_circle(g, 18, 15, x + HALF_WIDTH * cos(a), y + HALF_WIDTH * sin(a), HALF_WIDTH, 20, 0xffff0000, 0xffff0000);
        c.draw_rectangle(x + HALF_WIDTH, y - HALF_WIDTH, x + l, y + HALF_WIDTH, 0, 0xFFFF0000);
        drawing::fill_circle(g, 18, 15, x + HALF_WIDTH * cos(a), y + HALF_WIDTH * sin(a), INNER_WIDTH, 20, 0xffffffff, 0xffffffff);
        c.draw_rectangle(x + HALF_WIDTH, y - INNER_WIDTH, x + l, y + INNER_WIDTH, 0, 0xFFFFFFFF);
    }

entity@ create_emitter(const int id, const float x, const float y, const int width, const int height, const int layer, const int sub_layer, const int rotation=0)
{
    entity@ emitter = create_entity("entity_emitter");
    varstruct@ vars = emitter.vars();
    emitter.layer(layer);
    vars.get_var("emitter_id").set_int32(id);
    vars.get_var("width").set_int32(width);
    vars.get_var("height").set_int32(height);
    vars.get_var("draw_depth_sub").set_int32(sub_layer);
    vars.get_var("r_area").set_bool(true);
    vars.get_var("e_rotation").set_int32(rotation);
    emitter.set_xy(x, y);

    return emitter;
}


  void subframe_end_callback(dustman@ dm, int arg)
  {
      if ( _script.event_cycle.iteration != event_cycle_iteration  ) {
        // we can't remove these callbacks after deactivating this cycle event,
        // but we can check if we're currently in a later cycle iteration than
        // when this event was originally initialized
        return;
      }

      if (h !is null and not hit)
      {
          float a = h.attack_dir();
          if (abs(a - 85) < 1) a = 90;
          if (abs(a + 85) < 1) a = -90;
          a = (a - 90) * DEG2RAD;

          charge_x = p.x() + HALF_WIDTH * cos(a);
          charge_y = p.y() - 48 + HALF_WIDTH * sin(a);
          entity@ e = h.as_entity();
          e.x(1e8);
          e.y(1e8);

          if (h.hit_outcome() != 0)
          {
              if (h.hit_outcome() != 4)
              {
                  // Shooting
                  cam.change_fog(bright_fog, 0);
                  change_fog = true;
                  g.play_script_stream("shoot", 0, 0, 0, false, 0.5);

                  float dx = -knockback * cos(a);
                  float dy = -knockback * sin(a);

                  float vx = dm.x_speed() + dx;
                  if (dx > 0) vx = max(vx, dx);
                  if (dx < 0) vx = min(vx, dx);

                  float vy = dm.y_speed() + dy;
                  if (dy > 0) vy = max(vy, dy);
                  if (dy < 0) vy = min(vy, dy);

                  dm.set_speed_xy(vx, vy);

                  create_laser(dm.x(), dm.y() - 48, a);
              }
              hit = true;
          }
      }
      if (p.heavy_intent() == 99) p.heavy_intent(0);

      if (hit and p.heavy_intent() == 11)
      {
          // Charging
          p.heavy_intent(100);
          g.play_script_stream("charge", 0, 0, 0, false, 0.5);
          @h = p.hitbox();
          g.remove_entity(h.as_entity());
          hit = false;
      }
  }

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    @p = controller_controllable( 0 );
    if ( @p != null ) {
      @dm = p.as_dustman();
    }

    if ( @dm == null ) {
      return;
    }

    // get the main script object
    @_script = cast<script@>( get_script() );

    if ( _script.turbo_mode ) {
      // don't bother initializing nor running the script in turbo mode, it
      // wouldn't have any effect and we'd just be needlessly creating
      // on_hit_callbacks that can begin to lag the game if enough are added
      // over a long period of time; NOTE: this check should be added to all
      // functions above, that are run by the Cycle, as well
      return;
    }

    event_cycle_iteration = _script.event_cycle.iteration;

    @g = get_scene();
    @c = create_canvas(false, 18, 15);
    @cam = get_camera(0);
    dm.on_subframe_end_callback(this, "subframe_end_callback", 0);

    float fog_speed;
    int trigger_size;
    get_fog_setting( entity_by_id(normal_fog_trigger), normal_fog, fog_speed, trigger_size );
    get_fog_setting( entity_by_id(bright_fog_trigger), bright_fog, fog_speed, trigger_size );

    if ( srand_range( 0, 3 ) == 3 ) {
      // have a 1/4th chance to reverse the laser knockback
      knockback = -knockback;
    }

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
