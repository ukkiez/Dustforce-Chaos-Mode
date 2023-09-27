#include "../CycleEvent.cpp";

class BuilderMode : CycleEvent {
  CycleEventConfig get_config() {
    return CycleEventConfig( 250, "Builder Mode", "Impromptu Ramen Map" );
  }

  scene@ g;
  dustman@ player;

  sprites@ spr;
  string sprite_name = "tile3";

  float tile_offset_x = tile( 3 );
  float tile_offset_x_left = tile( -4 );
  float tile_offset_y = tile( -1 );

  // use this to delay placing/removing a tile by 1 frame, as
  // player.attack_face() takes a frame to change after pressing an attack
  bool placing_tile = false;
  bool removing_tile = false;

  int facing = 1;

  bool light_intent_zeroed = false;
  bool heavy_intent_zeroed = false;

  bool initialized = false;

  BuilderMode() {}

  void step( int entities ) {
    facing = player.face();
    if ( player.x_intent() != 0 ) {
      facing = player.x_intent();
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
    }
    if ( !light_intent_zeroed && player.light_intent() != 10 ) {
      light_intent_zeroed = true;
    }

    // place a tile on light attack
    if ( heavy_intent_zeroed && player.heavy_intent() == 10 ) {
      heavy_intent_zeroed = false;
      removing_tile = true;
    }
    if ( !heavy_intent_zeroed && player.heavy_intent() != 10 ) {
      heavy_intent_zeroed = true;
    }
  }

  void draw( float sub_frame ) {
    spr.draw_world(
      19,
      10,
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

  void place_tile() {
    // top left
    g.set_tile( tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) ), tile_coord( player.y() + tile_offset_y ), 19, true, 0, 1, 1, 1 );
    // top right
    g.set_tile( tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) + tile( 1 ) ), tile_coord( player.y() + tile_offset_y ), 19, true, 0, 1, 1, 1 );
    // bottom left
    g.set_tile( tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) ), tile_coord( player.y() + tile_offset_y + tile( 1 ) ), 19, true, 0, 1, 1, 1 );
    // bottom right
    g.set_tile( tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) + tile( 1 ) ), tile_coord( player.y() + tile_offset_y + tile( 1 ) ), 19, true, 0, 1, 1, 1 );
  }

  void remove_tile() {
    int x = tile_coord( player.x() + ( facing == 1 ? tile_offset_x : tile_offset_x_left ) );
    int y = tile_coord( player.y() + tile_offset_y );
    set_tile( g, x, y, false );
    remove_tile_filth( g, x, y );

    set_tile( g, x + 1, y, false );
    remove_tile_filth( g, x + 1, y );

    set_tile( g, x, y + 1, false );
    remove_tile_filth( g, x, y + 1 );

    set_tile( g, x + 1, y + 1, false );
    remove_tile_filth( g, x + 1, y + 1 );
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

    @spr = create_sprites();
    spr.add_sprite_set( "tile1" );

    initialized = true;
  }

  void deactivate() {
    if ( !initialized ) {
      return;
    }

    initialized = false;
  }
}
