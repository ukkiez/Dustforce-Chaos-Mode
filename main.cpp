#include "./lib/std.cpp";
#include "./lib/Random.cpp";
#include "./lib/data/characters.cpp";

#include "./embeds/audio.cpp";

#include "./event-cycle/Cycle.cpp";
#include "./event-queue/Queue.cpp";

class script : script_base, Random {
  scene@ g;
  dustman@ player;

  Cycle@ event_cycle;
  Queue@ event_queue;

  float level_start_coord_x = 0;
  float level_start_coord_y = 0;

  uint time = 0;
  uint level_start_delay = 0;

  bool chaos_active = false;
  bool checkpoint_loaded = false;

  array<uint> position_history( 5 );

  script() {
    @g = get_scene();

    @event_cycle = Cycle();
    @event_queue = Queue();
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

    level_start_coord_x = player.x();
    level_start_coord_y = player.y();

    event_cycle.init();
    event_queue.init();
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
      // collect positional data every 30 step frames, 5 positions max, for
      // seeding purposes; note that the SeedGenerator gets its data from the
      // script object, hence why we do it here
      store_positional_data();
    }

    time++;

    if ( time <= level_start_delay ) {
      // wait a bit to activate Chaos at level start, to try prevent immediate
      // unloads and softlocks
      return;
    }

    // if ( event_cycle.initialized ) {
    //   event_cycle.step( entities );
    // }

    if ( event_queue.initialized ) {
      event_queue.step( entities );
    }
  }

  void store_positional_data() {
    // collect positional data, 5 positions max, for seeding purposes
    uint x = uint( abs( player.as_entity().x() ) );
    uint y = uint( abs( player.as_entity().y() ) );
    position_history.removeAt( 4 );
    position_history.insertAt( 0, x + y );
  }

  void draw( float sub_frame ) {
    if ( event_cycle.initialized ) {
      event_cycle.draw( sub_frame );
    }

    if ( event_queue.initialized ) {
      event_queue.draw( sub_frame );
    }
  }
}
