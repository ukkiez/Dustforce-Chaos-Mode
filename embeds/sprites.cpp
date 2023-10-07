// FrankerZMode
const string EMBED_frankerz = "./chaos/assets/sprites/frankerz.png";

// ExplosionMode
const string EMBED_explosion1 = "./chaos/assets/sprites/explosion/explosion-1.png";
const string EMBED_explosion2 = "./chaos/assets/sprites/explosion/explosion-2.png";
const string EMBED_explosion3 = "./chaos/assets/sprites/explosion/explosion-3.png";
const string EMBED_explosion4 = "./chaos/assets/sprites/explosion/explosion-4.png";
const string EMBED_explosion5 = "./chaos/assets/sprites/explosion/explosion-5.png";
const string EMBED_explosion6 = "./chaos/assets/sprites/explosion/explosion-6.png";
const string EMBED_explosion7 = "./chaos/assets/sprites/explosion/explosion-7.png";
const string EMBED_explosion8 = "./chaos/assets/sprites/explosion/explosion-8.png";
const string EMBED_explosion9 = "./chaos/assets/sprites/explosion/explosion-9.png";
const string EMBED_explosion10 = "./chaos/assets/sprites/explosion/explosion-10.png";
const string EMBED_explosion11 = "./chaos/assets/sprites/explosion/explosion-11.png";
const string EMBED_explosion12 = "./chaos/assets/sprites/explosion/explosion-12.png";
const string EMBED_explosion13 = "./chaos/assets/sprites/explosion/explosion-13.png";
const string EMBED_explosion14 = "./chaos/assets/sprites/explosion/explosion-14.png";
const string EMBED_explosion15 = "./chaos/assets/sprites/explosion/explosion-15.png";
const string EMBED_explosion16 = "./chaos/assets/sprites/explosion/explosion-16.png";
const string EMBED_explosion17 = "./chaos/assets/sprites/explosion/explosion-17.png";

// LaserMode
const string EMBED_empty = "./chaos/assets/sprites/empty.png";

void build_sprite_embeds( message@ msg ) {
  msg.set_string( "FrankerZ", "frankerz" );

  msg.set_string( "explosion1", "explosion1" );
  msg.set_string( "explosion2", "explosion2" );
  msg.set_string( "explosion3", "explosion3" );
  msg.set_string( "explosion4", "explosion4" );
  msg.set_string( "explosion5", "explosion5" );
  msg.set_string( "explosion6", "explosion6" );
  msg.set_string( "explosion7", "explosion7" );
  msg.set_string( "explosion8", "explosion8" );
  msg.set_string( "explosion9", "explosion9" );
  msg.set_string( "explosion10", "explosion10" );
  msg.set_string( "explosion11", "explosion11" );
  msg.set_string( "explosion12", "explosion12" );
  msg.set_string( "explosion13", "explosion13" );
  msg.set_string( "explosion14", "explosion14" );
  msg.set_string( "explosion15", "explosion15" );
  msg.set_string( "explosion16", "explosion16" );
  msg.set_string( "explosion17", "explosion17" );

  const array<string> prefixes = { "dm", "dg", "dk", "do", "sb", "ls", "tk", "dw" };
  for ( uint i = 0; i < prefixes.size(); ++i ) {
    const string prefix = prefixes[ i ];
    msg.set_string( prefix + "airheavyd", "empty" );
    msg.set_string( prefix + "heavyd", "empty" );
    msg.set_string( prefix + "heavyf", "empty" );
    msg.set_string( prefix + "heavyu", "empty" );
    msg.set_string( prefix + "hitfx", "empty" );
  }
}
