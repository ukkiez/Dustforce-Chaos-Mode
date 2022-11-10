// Disco
const string EMBED_funkin = "./chaos/audio/ibumsfunkin.ogg";

// PolishDriver
const string EMBED_carPassingRtoL = "./chaos/audio/car-passing-R-to-L.ogg";
const string EMBED_carPassingLtoR = "./chaos/audio/car-passing-L-to-R.ogg";
const string EMBED_carCrashRtoL = "./chaos/audio/car-crash-R-to-L.ogg";
const string EMBED_carCrashLtoR = "./chaos/audio/car-crash-L-to-R.ogg";
const string EMBED_carhornleft = "./chaos/audio/car-horn-left.ogg";
const string EMBED_carhornright = "./chaos/audio/car-horn-right.ogg";

// SussyPorcupine
const string EMBED_sussy = "./chaos/audio/amongus.ogg";

void _build_sounds( message@ msg ) {
  msg.set_string( "funkin", "funkin" );

  msg.set_string( "car_passing_r_to_l", "carPassingRtoL" );
  msg.set_string( "car_passing_l_to_r", "carPassingLtoR" );
  msg.set_string( "car_crash_r_to_l", "carCrashRtoL" );
  msg.set_string( "car_crash_l_to_r", "carCrashLtoR" );
  msg.set_string( "car_horn_left", "carhornleft" );
  msg.set_string( "car_horn_right", "carhornright" );

  msg.set_string( "sussy", "sussy" );
}
