#include "../../CycleEvent.cpp";

class _Hitbox {
  bool added_to_scene = false;
  hitbox@ h;

  _Hitbox( hitbox@ _h ) {
    @this.h = _h;
  }
}

class WorthMode : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 200, "Literally Unplayable", "Worth Mode" );
  }

  scene@ g;
  dustman@ player;

  hitbox@ _hitbox;

  hitbox@ _player_hitbox;

  array<_Hitbox@> hitboxes = {};

  bool added = true;

  float jump_a;
  float hop_a;

  bool initialized = false;

  WorthMode() {}

  void entity_on_add( entity@ e ) {
    // if ( @e.as_hitbox() == null ) {
    //   return;
    // }

    // if ( e.type_name() != "hit_box_controller" ) {
    //   return;
    // }

    // for ( uint i = 0; i < hitboxes.length; i++ ) {
    //   if ( e.is_same( hitboxes[ i ].h.as_entity() ) ) {
    //     return;
    //   }
    // }

    // hitbox@ h = e.as_hitbox();

    // if ( @h.owner() != null ) {
    //   if ( h.owner().is_same( player.as_controllable() ) ) {
    //     rectangle@ r = h.base_rectangle();
    //     hitbox@ _hitbox = create_hitbox( player.as_controllable(), 4, h.x(), h.y(), r.top() * 3, r.bottom() * 3, r.left() * 4, r.right() * 4 );
    //     _hitbox.damage( h.damage() );
    //     _hitbox.state_timer( h.state_timer() );
    //     _hitbox.activate_time( h.activate_time() );
    //     _hitbox.timer_speed( h.timer_speed() );
    //     _hitbox.attack_ff_strength( h.attack_ff_strength() );
    //     _hitbox.attack_strength( h.attack_strength() );
    //     _hitbox.can_parry( h.can_parry() );
    //     _hitbox.filth_type( h.filth_type() );
    //     _hitbox.stun_time( h.stun_time() );
    //     _hitbox.attack_dir( h.attack_dir() );
    //     _hitbox.attack_effect( h.attack_effect() );
    //     _hitbox.effect_frame_rate( h.effect_frame_rate() );
    //     _hitbox.team( h.team() );

    //     hitboxes.insertLast( _Hitbox( _hitbox ) );
    //   }
    // }
  }

  void step( int entities ) {
    // if ( @_player_hitbox != null ) {
    //   _player_hitbox.base_rectangle( -500, 500, -500, 500 );
    // }

    // for ( uint i = 0; i < hitboxes.length; i++ ) {
    //   if ( !hitboxes[ i ].added_to_scene ) {
    //     g.add_entity( hitboxes[ i ].h.as_entity() );
    //     hitboxes[ i ].added_to_scene = true;
    //   }
    // }
  }

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();
    controllable@ c = controller_controllable( 0 );
    if ( @c != null ) {
      @player = c.as_dustman();
    }

    if ( @player == null ) {
      return;
    }

    jump_a = player.jump_a();
    hop_a = player.hop_a();

    player.jump_a( -1500 );
    player.hop_a( -1500 );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    player.jump_a( jump_a );
    player.hop_a( hop_a );

    initialized = false;
  }
}
