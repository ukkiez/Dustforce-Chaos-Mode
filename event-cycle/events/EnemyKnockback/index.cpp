#include "../../CycleEvent.cpp";

#include "./FlyingCorpse.cpp";

/*
  Script courtesy of Jdude
*/

const dictionary V_ATTACK_DIRS = {
  { "groundstrikeu1", 1 },
  { "groundstrikeu2", 1 },
  { "groundstriked", -1 },
  { "groundstrike1", 0},
  { "groundstrike2", 0 },
  { "heavyu", 1 },
  { "heavyd", -1 },
  { "heavyf", 0 },

  { "airstriked1", -1 },
  { "airstriked2", -1},
  { "airheavyd", -1 }
};

class EnemyKnockback : CycleEvent, callback_base {
  CycleEventConfig get_config() {
    return CycleEventConfig( 300, "Jdude's Influence" );
  }

  script@ _script;
  scene@ g;
  dustman@ player;

  uint timer = 120;
  dictionary enemies;
  dictionary bounced_enemies;
  int attack_dir = 2;
  bool play_impact_heavy = false;
  bool play_impact_light = false;
  bool callbacks_setup = false;
  uint count = 0;

  uint event_cycle_iteration = 0;

  bool initialized = false;

  EnemyKnockback() {}

  void entity_on_add(entity@ e) {
    if ( _script.turbo_mode ) {
      // see: initialize()
      return;
    }

    if(e.type_name() == "filth_ball") {
      g.remove_entity(e);
    } else if(e.type_name() == "effect") {
      string spriteIndex = e.sprite_index();
      // For virtual characters, effects start with letter v
      int charLoc = spriteIndex.substr(0,1) == "v" ? 3 : 2;

      // Attempt to strip character specific characters off of sprite index
      string effect_name_suffix = spriteIndex.substr(charLoc);

      // Check if substring matches one of the attack direction sprites to
      // determine attack direction
      if(V_ATTACK_DIRS.exists(effect_name_suffix)) {
        attack_dir = int(V_ATTACK_DIRS[effect_name_suffix]);
      }
    }
  }
  void entity_on_remove(entity@ e) {
    if ( _script.turbo_mode ) {
      // see: initialize()
      return;
    }

    if(enemies.exists(""+e.id())) {
       flying_corpse@ f = cast<flying_corpse@>(enemies[""+e.id()]);
       f.kill = true;
    }
  }

  void step( int entities ) {
    if ( _script.turbo_mode ) {
      // see: initialize()
      return;
    }

    handle_hit_entities();
    if(play_impact_heavy) {
      play_effect("sfx_impact_heavy_1");
      play_effect("sfx_poly_heavy");
      play_impact_heavy = false;
    } else if(play_impact_light) {
      play_effect("sfx_poly_med");
      play_impact_light = false;
    }

    if(!callbacks_setup) {
      setup_callbacks();
    }
  }

  // Call this function every step. This function traverses the dictionary of
  // all corpses that should be flying, handles stepping them, and handles
  // cleaning them up
  void handle_hit_entities() {
    array<string> dict_keys = enemies.getKeys();

    for(uint i = 0; i < dict_keys.size(); i++) {
      flying_corpse@ f = cast<flying_corpse@>(enemies[dict_keys[i]]);
      int corpse_id = f.dead_c.as_entity().id();

      // If an enemy has collided with a wall, mark it for cleanup
      if(bounced_enemies.exists(""+corpse_id)) {
        bounced_enemies.delete(""+corpse_id);
        f.kill = true;
      }

      if(f.removed) {
        enemies.delete(dict_keys[i]);
        continue;
      }

      // We need to give one frame of leniency to get the attack direction
      if(!f.setup) {
        f.step();
        continue;
      }

      // Set what direction the prism should go, wont work in multiplayer 100%
      if(!f.dirset && attack_dir != 2) {
        f.dirset = true;
        f.diry = attack_dir;
      }

      f.step();
    }
  }

  void setup_callbacks() {
    for(uint i = 0; i < 4; i++) {
      dustman@ dm;

      if(@controller_entity(i) != null && @controller_entity(i).as_dustman() != null) {
        @dm = controller_entity(i).as_dustman();
      } else {
        continue;
      }
      dm.on_hit_callback(this, "player_hit_callback", event_cycle_iteration);
    }

    callbacks_setup = true;
  }

  void bounce_collision_callback(controllable@ ec, tilecollision@ tc, int side, bool moving, float snap_offset, int arg) {
    ec.check_collision(tc, side, moving, snap_offset);
    if(tc.hit() && ec.type_name() != "hittable_apple") {
      bounced_enemies[""+ec.id()] = true;
    }
  }

  void player_hit_callback(controllable@ attacker, controllable@ attacked, hitbox@ attack_hitbox, int arg) {
    if ( _script.event_cycle.iteration != event_cycle_iteration  ) {
      // we can't remove these callbacks after deactivating this cycle event,
      // but we can check if we're currently in a later cycle iteration than
      // when this event was originally initialized
      return;
    }

    if( @attacked == null || @attacked.as_hittable() == null ) {
      return;
    }

    hittable@ h = attacked.as_hittable();

    if(h.life() <= attack_hitbox.damage() || attacked.type_name() == "hittable_apple") {
      //Store the hitbox damage as we will be setting it to 0 below. It is
      //important so the game knows the correct attack sound to play
      uint damage = attack_hitbox.damage();

      if(!enemies.exists(''+h.id())) {
        //Large prisms die automatically if the damage is 3, no matter what
        //their life is
        attacked.set_collision_handler(this, "bounce_collision_callback", 0);

        //Tutorial enemies are weird, handle hardcoded damage/life values as a
        //special case here. We need to basically do 0 damage and play the hit
        //sounds ourselves.
        if(attacked.type_name() == "enemy_tutorial_hexagon") {
          play_impact_heavy = attack_hitbox.damage() == 3;
          play_impact_light = attack_hitbox.damage() == 1;
          attack_hitbox.damage(0);
        }

        //Set the enemy to have 99 life and put it in the dictionary of
        //currently flying enemies
        h.life(99);
        flying_corpse@ f = flying_corpse(@attacked, timer, attacker.player_index(), attacker.attack_face(), damage);
        enemies[''+h.id()] = @f;
      }
    }
  }

  void play_effect(string sfx_name) {
    g.play_sound(sfx_name, 3, 0, 1, false, false);
  }

  void initialize() {
    if ( initialized ) {
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
    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }

    if ( @player == null ) {
      return;
    }

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    for(uint i = 0; i < 4; i++) {
      dustman@ dm;

      if(@controller_entity(i) != null && @controller_entity(i).as_dustman() != null) {
        @dm = controller_entity(i).as_dustman();
      } else {
        continue;
      }
    }

    // deal with enemies who are still flying; if we leave the code as is, they
    // would stop flying, and be left with 99 hitpoints (see: on-hit callback);
    // instead, put their hitpoint total to 1
    array<string> dict_keys = enemies.getKeys();
    for ( uint i = 0; i < dict_keys.size(); i++ ) {
      flying_corpse@ f = cast<flying_corpse@>( enemies[ dict_keys[ i ] ] );
      f.dead_c.life( 1 );
    }

    initialized = false;
  }
}
