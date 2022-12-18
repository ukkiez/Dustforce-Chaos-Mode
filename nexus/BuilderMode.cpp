class NexusBuilderMode {
  scene@ g;
  dustman@ player;

  sprites@ spr;
  string sprite_name = "tile3";

  textfield@ instructions_builder_mode;

  string enter_builder_mode_text = "Press TAUNT to enter\nBuilder Mode";
  string build_instructions_text = "Light attack = place\nHeavy attack = remove\nTaunt = exit";

  float tile_offset_x = tile( 2 );
  float tile_offset_x_left = tile( -3 );
  float tile_offset_y = tile( -1 );

  bool build_mode = false;

  // use this to delay placing/removing a tile by 1 frame, as
  // player.attack_face() takes a frame to change after pressing an attack
  bool placing_tile = false;
  bool removing_tile = false;

  int facing = 1;

  bool light_intent_zeroed = false;
  bool heavy_intent_zeroed = false;

  bool initialized = false;

  NexusBuilderMode() {}

  void init() {
    @g = get_scene();

    controllable@ c = controller_controllable( 0 );
    if ( @c == null ) {
      return;
    }

    @player = c.as_dustman();
    if ( @player == null ) {
      return;
    }

    @spr = create_sprites();
    spr.add_sprite_set( "tile1" );

    @instructions_builder_mode = create_textfield();
    instructions_builder_mode.set_font( "Caracteres", 36 );
    if ( build_mode ) {
      instructions_builder_mode.text( build_instructions_text );
    }
    else {
      instructions_builder_mode.text( enter_builder_mode_text );
    }
    instructions_builder_mode.colour( 0xBBFFFFFF );

    initialized = true;
  }

  void step( int entities ) {
    if ( player.taunt_intent() == 1 ) {
      build_mode = !build_mode;

      if ( build_mode ) {
        instructions_builder_mode.text( build_instructions_text );
      }
      else {
        instructions_builder_mode.text( enter_builder_mode_text );
      }

      player.taunt_intent( 0 );
    }

    if ( build_mode ) {
      facing = player.face();
      if ( player.attack_state() == 1 || player.attack_state() == 2 ) {
        facing = player.attack_face();
      }

      if ( placing_tile ) {
        place_tile();
        placing_tile = false;
      }
      else if ( removing_tile ) {
        remove_tile();
        removing_tile = false;
      }

      // place a tile on light attack
      if ( light_intent_zeroed && player.light_intent() == 10 ) {
        light_intent_zeroed = false;
        placing_tile = true;

        player.light_intent( 0 );
      }
      if ( !light_intent_zeroed && player.light_intent() != 10 ) {
        light_intent_zeroed = true;
      }

      // place a tile on light attack
      if ( heavy_intent_zeroed && player.heavy_intent() == 10 ) {
        heavy_intent_zeroed = false;
        removing_tile = true;
        player.heavy_intent( 0 );
      }
      if ( !heavy_intent_zeroed && player.heavy_intent() != 10 ) {
        heavy_intent_zeroed = true;
      }
    }
  }

  void draw( float sub_frame ) {
    if ( build_mode ) {
      spr.draw_world(
        20,
        22,
        sprite_name,
        0,
        1,
        tile( tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) ) ),
        tile( tile_coord( player.y() + tile_offset_y ) ),
        0,
        1,
        1,
        0x99FFFFFF
      );
    }

    draw_text( instructions_builder_mode, 0, -350 );
  }

  void place_tile() {
    // top left
    g.set_tile(
      tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) ),
      tile_coord( player.y() + tile_offset_y ),
      19,
      true,
      0,
      1,
      1,
      1
    );
    // top right
    g.set_tile(
      tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) + tile( 1 ) ),
      tile_coord( player.y() + tile_offset_y ),
      19,
      true,
      0,
      1,
      1,
      1
    );
    // bottom left
    g.set_tile(
      tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) ),
      tile_coord( player.y() + tile_offset_y + tile( 1 ) ),
      19,
      true,
      0,
      1,
      1,
      1
    );
    // bottom right
    g.set_tile(
      tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) + tile( 1 ) ),
      tile_coord( player.y() + tile_offset_y + tile( 1 ) ),
      19,
      true,
      0,
      1,
      1,
      1
    );
  }

  void remove_tile() {
    float x = player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left );
    float y = player.y() + tile_offset_y;
    set_tile( g, tile_coord( x ), tile_coord( y ), false );
    remove_tile_filth( g, tile_coord( x ), tile_coord( y ) );

    set_tile( g, tile_coord( x + tile( 1 ) ), tile_coord( y ), false );
    remove_tile_filth( g, tile_coord( x + tile( 1 ) ), tile_coord( y ) );

    set_tile( g, tile_coord( x ), tile_coord( y + tile( 1 ) ), false );
    remove_tile_filth( g, tile_coord( x ), tile_coord( y + tile( 1 ) ) );

    set_tile( g, tile_coord( x + tile( 1 ) ), tile_coord( y + tile( 1 ) ), false );
    remove_tile_filth( g, tile_coord( x + tile( 1 ) ), tile_coord( y + tile( 1 ) ) );
  }
}
