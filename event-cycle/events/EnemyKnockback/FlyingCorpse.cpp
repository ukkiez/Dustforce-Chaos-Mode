class flying_corpse {
  scene@ g;
  controllable@ dead_c; //Handle to the dead controllable
  int timer = 0; //Timer which counts down how long until the enemy auto cleans
  uint player = 0; //Which player hit this corpse
  float speed = 15; //Speed the enemy translates away
  float rotation_speed = 15; //How fast enemy should rotate when flying
  int dirx = 0; //-1 for left, 1 for right
  int diry  = 0; //1 for up -1 for down 0 for neither
  int damage = 1; //How much damage the killing blow was
  bool dead = false; //Flag used to state if the flying corpse has outlived its timer and should be cleaned one final time to get rid of it
  bool removed = false; //Flag used to help state if this corpse has had 1 frame to die in order for whatever datastructure holding these objects to have dead
                        //references removed
  bool dirset = false; //Flag used to check if this entity has its directions set, could have used enums for this but decided not to
  bool setup = false; //We need 1 frame after this entity dies in order for dustman's hit effects to spawn
  bool kill = false; //Set this flag to true whenever you want to explode the corpse
  bool offset = false; //Flag stating whether the flying corpse has been offset yet. This is done to alleviate some clipping

  /*arguments:
    c: the handle to the corpse that should be flying
    time: the time the corpse has before being cleansed
    player_: the player number that cleaned this enemy
    dirX_: the x direction the player hit this enemy
    damage_: the damage of the final blow to this enemy
   */
  flying_corpse(controllable@ c, uint time, uint player_, int dirX_, int damage_) {
    @dead_c = @c;
    timer = time;
    player = player_;
    @g = get_scene();
    dirx = dirX_;
    damage = damage_;
    dead = false;
    removed = false;
  }

  /* Each flying corpse needs to be stepped each frame */
  void step() {
    entity@ e = dead_c.as_entity();
    if(!setup) {
      setup = true;
      //Move entity slightly off surface to avoid clipping
    } else if(timer > 0 && !kill) {
      fly();
      timer--;
    } else if(!dead) {
      die();
      dead = true;
    } else {
      removed = true;
    }
  }

  /* Called when corpse needs to be rotated and translated.*/
  void fly() {
    entity@ e = dead_c.as_entity();
    if(!offset) {
      offset_corpse();
    }

    e.x(e.x() + (speed * dirx));
    dead_c.set_speed_xy(0,0);
    float delta_y = diry == 0 ? e.y() - speed/2: e.y() - (speed * diry);
    e.y(delta_y);
    e.rotation(e.rotation() + rotation_speed);
  }

  /* Call to mark corpse for cleanup and clean next frame */
  void die() {
    controllable@ c;
    if(@controller_entity(0) != null && @controller_entity(0).as_controllable() != null) {
      @c = controller_entity(0).as_controllable();
    } else {
      g.remove_entity(dead_c.as_entity());
      give_aircharges();
      return;
    }

    hittable@ h = @dead_c.as_hittable();
    //If for whatever reason the enemy is null, just give aircharges back and return
    if(@h == null) {
      give_aircharges();
      return;
    }

    //Do not spawn another hitbox on apples as they cant die
    if(dead_c.type_name() != "hittable_apple") {
      finish_enemy(c, h);
    }
  }

  /* Used to offset the corpse by some hand picked values to try and help alleviate clipping. Basically
   * we are moving the corpse away of the direction of travel. e.g if enemy was hit with an up-right heavy,
   * set the position to be a bit down left from the current position. Jank but kinda works. Could use raycasting probably to
   * do this more precisely
   */
  void offset_corpse() {
    entity@ e = dead_c.as_entity();
    offset = true;
    if(diry != 0) {
      float newspeed = e.y() + (float(diry) * speed * 5);
      e.y(newspeed);
    }
    e.x(e.x() - (dirx * speed * 6.4));
  }

  /* Call to give player who did final blow to this enemy aircharges back when enemy dies */
  void give_aircharges() {
    if(@controller_controllable(player) != null) {
      if(@controller_controllable(player).as_dustman() != null) {
        dustman@ dm = controller_controllable(player).as_dustman();
        dm.dash(dm.dash_max());
      }
    }
  }

  /* Call to set the enemy's life to -1 and spawn a hitbox on it to finish it off */
  void finish_enemy(controllable@ c, hittable@ h) {
    h.life(-1);
    hitbox@ hb = create_hitbox(@c, 0, dead_c.x(), dead_c.y(), -1, 1, -1, 1);
    hb.damage(damage);
    g.add_entity(hb.as_entity());
    give_aircharges();
  }
}
