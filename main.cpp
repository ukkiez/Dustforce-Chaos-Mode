#include "./lib/Random.cpp";

#include "./modes/Mode.cpp";

#include "./modes/Audioizer.cpp";
#include "./modes/Big.cpp";
#include "./modes/CharacterSwap.cpp";
#include "./modes/DisableAttack.cpp";
#include "./modes/DisableDash.cpp";
#include "./modes/DisableJump.cpp";
#include "./modes/Disco.cpp";

#include "./modes/Enemizer/EnemyBarrelizer.cpp";
#include "./modes/Enemizer/EnemyScaler.cpp";
#include "./modes/Enemizer/EnemyTimeWarp.cpp";

#include "./modes/InfiniteAircharges.cpp";
#include "./modes/Letterbox.cpp";
#include "./modes/MaxSuper.cpp";
#include "./modes/MinecraftMode.cpp";
#include "./modes/NoFriction.cpp";
#include "./modes/PropSwap/index.cpp";
#include "./modes/PolishDriver.cpp";
#include "./modes/SpawnApples.cpp";
#include "./modes/SussyPorcupine.cpp";

#include "./modes/Tilerizer/SpawnBlock.cpp";
#include "./modes/Tilerizer/SpawnZip.cpp";
#include "./modes/Tilerizer/SwapSprites.cpp";

#include "./modes/TauntAndDie.cpp";
#include "./modes/TimeWarp.cpp";
#include "./modes/Tiny.cpp";

#include "./modes/Unplayable/DashMacro.cpp";
#include "./modes/Unplayable/Rotator.cpp";

// Disco
const string EMBED_funkin = "./chaos/audio/ibumsfunkin.ogg";

// SussyPorcupine
const string EMBED_sussy = "./chaos/audio/amongus.ogg";

// PolishDriver
const string EMBED_carPassingRtoL = "./chaos/audio/car-passing-R-to-L.ogg";
const string EMBED_carPassingLtoR = "./chaos/audio/car-passing-L-to-R.ogg";
const string EMBED_carCrashRtoL = "./chaos/audio/car-crash-R-to-L.ogg";
const string EMBED_carCrashLtoR = "./chaos/audio/car-crash-L-to-R.ogg";
const string EMBED_carhornleft = "./chaos/audio/car-horn-left.ogg";
const string EMBED_carhornright = "./chaos/audio/car-horn-right.ogg";

// class ActiveMode {
//   uint index;
//   uint duration;
//   ActiveMode( uint index, uint duration ) {
//     this.index = index;
//     this.duration = duration;
//   }
// }

class script : script_base, Random {
  scene@ g;
  dustman@ player;

  uint time = 0;
  // keep track of which Mode iteration we're on currently, in case a Mode needs
  // to access this in order to properly deal with e.g. checkpoint
  // reinitialization
  uint iteration = 0;

  // duration that current modes are active (in step() frames)
  uint duration = 0;

  bool chaos_active = false;
  bool checkpoint_loaded = false;

  array<uint> active_mode_indexes;
  // array<ActiveMode@> active_modes;

  array<ModeConfig> mode_configs = {};
  array<textfield@> mode_textfields;
  array<textfield@> mode_subtextfields;

  array<Mode@> @modes = {
    Audioizer(),
    Big(),
    CharacterSwap(),
    DisableAttack(),
    DisableDash(),
    DisableJump(),
    Disco(),

    // Enemizer
    // EnemyBarrelizer(),
    EnemyScaler(),
    EnemyTimeWarp(),

    InfiniteAircharges(),
    Letterbox(),
    MaxSuper(),
    MinecraftMode(),
    NoFriction(),
    PolishDriver(),
    PropSwap(),
    SpawnApples(),
    SussyPorcupine(),

    // Tilerizer
    SpawnBlock(),
    SpawnZip(),
    SwapSprites(),

    TauntAndDie(),
    TimeWarp(),
    Tiny(),

    // Unplayable
    DashMacro(),
    Rotator(),
  };

  array<Mode@> CHECKPOINT_modes;
  array<uint> CHECKPOINT_active_mode_indexes;
  uint CHECKPOINT_current_num_active_modes;
  uint CHECKPOINT_iteration;

  // is automatically set to true if any modes are added to the below
  // DEBUG_modes_override array
  bool DEBUG_MODE = false;
  // putting modes in here will automatically give them a 100 weight,
  // guaranteeing them to be available to be picked every round (though still
  // not twice in a row, and retaining the existing limit of concurrent number
  // of modes)
  array<Mode@> DEBUG_modes_override = {
    MinecraftMode(),
  };

  array<uint> position_history( 5 );

  // interval in seconds
  uint interval;
  uint current_num_active_modes;

  script() {
    @g = get_scene();

    if ( DEBUG_modes_override.length > 0 ) {
      // only pick DEBUG modes when provided
      modes = DEBUG_modes_override;
      DEBUG_MODE = true;
    }

    for ( uint i = 0; i < modes.length; i++ ) {
      ModeConfig config = modes[ i ].get_mode_config();

      if ( DEBUG_MODE ) {
        config.weight = 100;
      }

      // fill up the array with all mode configurations up front, so they will
      // be readily available for the script
      mode_configs.insertLast( config );
    }
  }

  void build_sounds( message @msg ) {
    msg.set_string( "funkin", "funkin" );

    msg.set_string( "sussy", "sussy" );

    msg.set_string( "car_passing_r_to_l", "carPassingRtoL" );
    msg.set_string( "car_passing_l_to_r", "carPassingLtoR" );
    msg.set_string( "car_crash_r_to_l", "carCrashRtoL" );
    msg.set_string( "car_crash_l_to_r", "carCrashLtoR" );
    msg.set_string( "car_horn_left", "carhornleft" );
    msg.set_string( "car_horn_right", "carhornright" );
  }

  void on_level_start() {
    controllable@ c = controller_controllable( 0 );
    if ( @c == null ) {
      return;
    }

    @player = c.as_dustman();

    for ( uint i = 0; i <= 6; i++ ) {
      textfield@ mode_text = create_textfield();
      mode_text.set_font( "Caracteres", 40 );
      mode_text.align_horizontal( -1 );
      mode_text.align_vertical( 0 );
      mode_textfields.insertAt( i, mode_text );

      textfield@ mode_subtext = create_textfield();
      mode_subtext.set_font( "Caracteres", 26 );
      mode_subtext.align_horizontal( -1 );
      mode_subtext.align_vertical( 0 );
      mode_subtextfields.insertAt( i, mode_subtext );
    }

    interval = srand_range( 3, 12 );
  }

  void checkpoint_save() {
    // keep track of script state across checkpoints, so we can go back to it
    // when a checkpoint gets loaded to be in line with the game state at that
    // point; to prevent e.g. saving a checkpoint while attack is disabled, and
    // a minute later the checkpoint loads and the mode will no longer be active
    // and able to be deactivated
    CHECKPOINT_modes = modes;
    CHECKPOINT_active_mode_indexes = active_mode_indexes;
    CHECKPOINT_current_num_active_modes = current_num_active_modes;
    CHECKPOINT_iteration = iteration;
  }

  void checkpoint_load() {
    controllable@ c = controller_controllable( 0 );
    if ( @c == null ) {
      return;
    }

    @player = c.as_dustman();

    checkpoint_loaded = true;
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
      // try to prevent issues with gathering the player controllable
      return;
    }

    if ( ( time % 60 ) == 0 ) {
      // collect positional data every 60 step frames, 5 positions max, for
      // seeding purposes; note that the SeedGenerator gets its data from the
      // script object, hence why we do it here
      store_positional_data();
    }

    // activate new modes every X seconds
    if ( duration % ( interval * 60 ) == 0 ) {
      duration = 0;
      iteration++;

      // choose after how many seconds we will pick the next set of Modes after
      // this one
      interval = srand_range( 3, 12 );

      // randomly filter out available modes to be activated next, based on
      // their weight and whether they were activated the previous time
      array<uint> filtered_pool = filter_modes();

      if ( active_mode_indexes.length > 0 ) {
        // empty out the active mode index list
        active_mode_indexes.removeRange( 0, active_mode_indexes.length );
      }

      // pick a range for the number of active modes that is more weighted
      // towards 3 and 4 than any other
      current_num_active_modes = pick_number_of_modes();
      int range = srand_range( 2, 12 );

      current_num_active_modes = srand_range( 2, 6 );
      if ( current_num_active_modes > filtered_pool.length ) {
        current_num_active_modes = filtered_pool.length;
      }

      for ( uint i = 0; i < current_num_active_modes; i++ ) {
        // randomly pick new modes to activate
        uint index = filtered_pool[ srand_range( 0, filtered_pool.length - 1 ) ];

        while ( active_mode_indexes.find( index ) >= 0 ) {
          // don't pick duplicate modes
          index = filtered_pool[ srand_range( 0, filtered_pool.length - 1 ) ];
        }

        active_mode_indexes.insertLast( index );

        modes[ index ].initialize();

        ModeConfig config = mode_configs[ index ];
        mode_textfields[ i ].text( config.name );
        mode_textfields[ i ].colour( config.colour );
        mode_subtextfields[ i ].text( config.subtext );
        mode_subtextfields[ i ].colour( config.colour );
      }

      if ( current_num_active_modes == 0 ) {
        chaos_active = false;
        // never stay without any Modes active for longer than 5 seconds
        interval = ( interval < 5 ) ? 5 : interval;
      }
      else {
        chaos_active = true;
      }
    }

    if ( checkpoint_loaded && ( CHECKPOINT_iteration != iteration ) ) {
      // reinitialize and deactivate all modes that were active at the last
      // saved checkpoint, if we're past the iteration from that time;
      // reinitialization is necessary since script variables may have been
      // reset/lost
      for ( uint i = 0; i < CHECKPOINT_current_num_active_modes; i++ ) {
        CHECKPOINT_modes[ CHECKPOINT_active_mode_indexes[ i ] ].initialize();
        CHECKPOINT_modes[ CHECKPOINT_active_mode_indexes[ i ] ].deactivate();
      }
    }

    if ( chaos_active ) {
      for ( uint i = 0; i < current_num_active_modes; i++ ) {
        if ( checkpoint_loaded ) {
          // reinitialize the mode after we loaded from a checkpoint, to make
          // sure the script variables aren't messed up
          modes[ active_mode_indexes[ i ] ].deactivate();
          modes[ active_mode_indexes[ i ] ].initialize();
        }

        modes[ active_mode_indexes[ i ] ].step( entities );
      }

    }

    if ( checkpoint_loaded ) {
      checkpoint_loaded = false;
    }

    // always increment the duration, even if no Mode is active, because that
    // would just mean that last time we didn't have any Modes available and
    // we're just playing vanilla for <interval> seconds
    duration++;
    time++;
  }

  array<uint> filter_modes() {
    // generate a range to exclude modes based on their weight, e.g. a rolled
    // range of 50 means all weights below 50 should not be included in the pool
    uint range = srand_range( 1, 100 );

    array<uint> pool = {};

    for ( uint i = 0; i < modes.length; i++ ) {
      if ( active_mode_indexes.find( i ) >= 0 ) {
        // this mode was already activated the previous time, so don't add it to
        // the pool; also deactivate this last mode and remove it from the ones
        // marked as active
        modes[ i ].deactivate();
        continue;
      }

      if ( mode_configs[ i ].weight >= range ) {
        pool.insertLast( i );
      }
    }

    return pool;
  }

  uint pick_number_of_modes() {
    // pick a range for the number of active modes that is slightly more
    // weighted towards 3 and 4 than any other number; min active modes is 2,
    // max is 6

    int range = srand_range( 2, 12 );
    if ( range == 2 ) {
      return 2;
    }
    else if ( range <= 5 ) {
      return 3;
    }
    else if ( range <= 9 ) {
      return 4;
    }
    else if ( range <= 11 ) {
      return 5;
    }
    else if ( range <= 12 ) {
      return 6;
    }
    else {
      return 4;
    }
  }

  void deactive_mode( Mode@ m ) {
    ModeConfig config = m.get_mode_config();
  }

  void store_positional_data() {
    // collect positional data every 60 step frames, 5 positions max; for
    // seeding purposes
    uint x = uint( abs( player.as_entity().x() ) );
    uint y = uint( abs( player.as_entity().y() ) );
    position_history.removeAt( 4 );
    position_history.insertAt( 0, x + y );
  }

  void draw( float sub_frame ) {
    if ( !chaos_active ) {
      return;
    }

    int default_x = -750;
    int y_offset = 0;
    int subtext_y_offset = 40;

    if ( duration < 7 ) {
      default_x -= uint( 300 - ( duration * 30 ) );
    }

    int y_gap = 100;
    int starting_point = ( -( y_gap / 2 ) * ( current_num_active_modes ) );

    for ( uint i = 0; i < current_num_active_modes; i++ ) {
      // call the mode's draw method itself
      modes[ active_mode_indexes[ i ] ].draw( sub_frame );

      int x = default_x;
      int y = ( starting_point + ( y_gap * i ) );

      // attempt to dynamically spread out the mode text across the screen, so
      // that they don't overlap, depending on how many are active
      draw_text( mode_textfields[ i ], x, y, 1, 1, 0 );
      if ( mode_subtextfields[ i ].text() != "" ) {
        draw_text( mode_subtextfields[ i ], x, y + subtext_y_offset, 1, 1, 0 );
      }
    }
  }

  void draw_text( textfield@ text, float x, float y, float scale_x = 1.0, float scale_y = 1.0, float rotation = 0.0 ) {
    if ( text.text() != "" ) {
      // draw the text on layer 0, sublayer 0
      text.draw_hud( 22, 22, x, y, scale_x, scale_y, rotation % 360 );
    }
  }
}
