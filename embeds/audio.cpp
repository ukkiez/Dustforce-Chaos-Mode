// Disco
const string EMBED_funkin = "./chaos/assets/audio/ibumsfunkin.ogg";

// DiscordPing
const string EMBED_discordping = "./chaos/assets/audio/discord-ping.ogg";

// PolishDriver
const string EMBED_carPassingRtoL = "./chaos/assets/audio/car-pass-RL.ogg";
const string EMBED_carPassingLtoR = "./chaos/assets/audio/car-pass-LR.ogg";
const string EMBED_carCrashRtoL = "./chaos/assets/audio/car-crash-RL.ogg";
const string EMBED_carCrashLtoR = "./chaos/assets/audio/car-crash-LR.ogg";
const string EMBED_carhornleft = "./chaos/assets/audio/car-horn-left.ogg";
const string EMBED_carhornright = "./chaos/assets/audio/car-horn-right.ogg";

// SussyPorcupine
const string EMBED_sussy = "./chaos/assets/audio/amongus.ogg";

void build_audio_embeds( message@ msg ) {
  msg.set_string( "funkin", "funkin" );

  msg.set_string( "discord_ping", "discordping" );

  msg.set_string( "car_passing_r_to_l", "carPassingRtoL" );
  msg.set_string( "car_passing_l_to_r", "carPassingLtoR" );
  msg.set_string( "car_crash_r_to_l", "carCrashRtoL" );
  msg.set_string( "car_crash_l_to_r", "carCrashLtoR" );
  msg.set_string( "car_horn_left", "carhornleft" );
  msg.set_string( "car_horn_right", "carhornright" );

  msg.set_string( "sussy", "sussy" );
}
