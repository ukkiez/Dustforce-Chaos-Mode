#include "./lib/std.cpp";
#include "./lib/Random.cpp";
#include "./lib/data/characters.cpp";
#include "./lib/util/text.cpp";

#include "./embeds/audio.cpp";

#include "./event-cycle/Cycle.cpp";
#include "./event-queue/Queue.cpp";
#include "./EventList.cpp";

class script : script_base, Random {
  scene@ g;
  dustman@ player;

  Cycle@ event_cycle;
  Queue@ event_queue;
  EventList@ event_list;

  uint time = 0;
  // uint level_start_delay = 55;
  uint level_start_delay = 0;

  bool checkpoint_loaded = false;

  [persist] bool turbo_mode = false;
  // chance of turbo happening = 1/turbo_mode_chance
  uint turbo_mode_chance = 2000;
  uint turbo_mode_time = 0;
  uint text_display_time = 120;
  // duration in seconds
  uint turbo_mode_duration = 3;
  textfield@ turbo_mode_tf;
  sprites@ turbo_warning_symbol;

  uint position_history_length = 5;
  array<uint> position_history( position_history_length );

  // define one seed_generator that all Random instances will use
  SeedGenerator@ seed_generator = SeedGenerator();

  script() {
    @g = get_scene();

    @event_cycle = Cycle();
    @event_queue = Queue();
    @event_list = EventList();

    @turbo_mode_tf = create_textfield();
    turbo_mode_tf.set_font( "Caracteres", 92 );
    turbo_mode_tf.text( "TURBO MODE\nACTIVATED" );
    turbo_mode_tf.colour( 0xBBFFFFFF );

    @turbo_warning_symbol = create_sprites();
    turbo_warning_symbol.add_sprite_set( "props5" );
  }

  void build_sounds( message @msg ) {
    build_audio_embeds( msg );
  }

  void on_level_start() {
    controllable@ c = controller_controllable( 0 );
    if ( @c == null ) {
      return;
    }

    @player = c.as_dustman();

    seed_generator.init();

    event_cycle.init();
    event_queue.init();
    event_list.init();
  }

  void checkpoint_save() {
    event_cycle.checkpoint_save();
    event_queue.checkpoint_save();
  }

  void checkpoint_load() {
    controllable@ c = controller_controllable( 0 );
    if ( @c == null ) {
      return;
    }

    @player = c.as_dustman();

    checkpoint_loaded = true;

    event_cycle.checkpoint_load();
    event_queue.checkpoint_load();
  }

  void step( int entities ) {
    if ( @player == null ) {
      controllable@ c = controller_controllable( 0 );
      if ( @c == null ) {
        return;
      }

      @player = c.as_dustman();
      if ( @player == null ) {
        return;
      }
    }

    if ( player.dead() ) {
      // try to prevent issues with e.g. gathering the player controllable
      return;
    }

    if ( ( time % 30 ) == 0 ) {
      // collect positional data every 30 step frames, for seeding purposes;
      // note that the SeedGenerator gets its data from the script object, hence
      // why we do it here
      store_positional_data();
    }

    time++;

    if ( time <= level_start_delay ) {
      // wait a bit to activate Chaos at level start, to try prevent immediate
      // unloads and softlocks
      return;
    }

    if ( ( time % 60 == 0 ) && !turbo_mode ) {
      // every second, there's a chance that "turbo mode" activates, which gives
      // all modes a 100 weight, and puts the cycle and queue intervals at 1
      // second
      int roll = srand_range( 1, turbo_mode_chance );
      if ( roll == 1 ) {
        turbo_mode = true;
      }
    }

    if ( !turbo_mode ) {
      if ( event_cycle.initialized ) {
        event_cycle.step( entities );
      }

      if ( event_queue.initialized ) {
        event_queue.step( entities );
      }

      if ( event_list.initialized ) {
        event_list.step( entities );
      }
    }
    else {
      if ( turbo_mode_time >= text_display_time ) {
        if ( turbo_mode_time % 1 == 0 ) {
          // activate new events every frame for now don't turn on the event
          // cycle, as it has a lot of modes that would quickly cause epilepsy,
          // if at least a good migraine; TODO: create a curated list of turbo
          // modes for the cycle and queue to pick from

          event_cycle.activate_turbo_mode();
          event_queue.activate_turbo_mode();
        }

        if ( event_cycle.initialized ) {
          event_cycle.step( entities );
        }

        if ( event_queue.initialized ) {
          event_queue.step( entities );
        }

        if ( event_list.initialized ) {
          event_list.step( entities );
        }
      }

      turbo_mode_time++;

      if ( turbo_mode_time % ( ( turbo_mode_duration * 60 ) + text_display_time ) == 0 ) {
        event_cycle.deactivate_turbo_mode();
        event_queue.deactivate_turbo_mode();
        turbo_mode_time = 0;
        turbo_mode = false;
      }
      return;
    }
  }

  void store_positional_data() {
    // collect positional data, 5 positions max, for seeding purposes
    uint x = uint( abs( player.as_entity().x() ) );
    uint y = uint( abs( player.as_entity().y() ) );
    position_history.removeAt( position_history_length-1 );
    position_history.insertAt( 0, x + y );
  }

  void draw( float sub_frame ) {
    if ( turbo_mode ) {
      if ( turbo_mode_time <= 110 ) {
        if ( turbo_mode_time % 40 < 20 ) {
          turbo_warning_symbol.draw_hud( 20, 22, "symbol_2", 0, 1, 150, 250, 0, 1.5, 1.5, 0xFFFFFFFF );
        }

        draw_text( turbo_mode_tf, 0, -100 );
      }

      if ( event_cycle.initialized ) {
        event_cycle.draw( sub_frame );
      }

      if ( event_queue.initialized ) {
        event_queue.draw( sub_frame );
      }

      if ( event_list.initialized ) {
        event_list.draw( sub_frame );
      }

      return;
    }

    if ( event_cycle.initialized ) {
      event_cycle.draw( sub_frame );
    }

    if ( event_queue.initialized ) {
      event_queue.draw( sub_frame );
    }

    if ( event_list.initialized ) {
      event_list.draw( sub_frame );
    }
  }
}
