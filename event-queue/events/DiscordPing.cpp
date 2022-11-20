#include "../QueueEvent.cpp";

class DiscordPing : QueueEvent {
  QueueEventConfig get_config() {
    return QueueEventConfig( 10, 0, "Message" );
  }

  scene@ g;

  bool initialized = false;

  DiscordPing() {}

  void initialize() {
    if ( initialized ) {
      return;
    }

    @g = get_scene();

    g.play_script_stream( "discord_ping", 3, 0, 0, false, 0.5 );

    initialized = true;
  }
}
