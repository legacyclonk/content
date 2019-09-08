/*-- Glitzern --*/

#strict

local initialized;
local placement_factor;
local sparkle_amount;

protected func Initialize() {
  initialized = 0;
  placement_factor = 0;
  sparkle_amount = 0;
  SetMatAccessTable();
  return(1);
}

private func SetMatAccessTable()
  {
  // Schnellzugriffstabelle auf Materialien, die glitzern
  Local(Material("Gold")) = 1; Local(Material("Crystal")) = 2; Local(Material("Ice")) = 3;
  }

protected func UpdateTransferZone()
  {
  // Für Spielstände
  SetMatAccessTable();
  }

private func Initialize2() {
  // Wir sitzen oben links in der Ecke.
  SetPosition(0,0);

  // Umweltobjekte dieses Typs zählen
  placement_factor = RemoveAll(GetID())+1;

  // Wie oft soll in einem Frame nach Glitzermaterial gesucht werden?
  // Abhängig von der Kartengröße und dem Plazierungsfaktor des Umweltobjekts
  sparkle_amount = placement_factor*LandscapeWidth()*LandscapeHeight()/250000;
  //Log("Glitzerfaktor: %d", sparkle_amount);

  // Initialisierung fertig
  initialized = 1;
  return(1);
}

private func Twinkle() {
  // Sind wir noch nicht vorbereitet?
  if(!initialized) return(Initialize2());

  // sparkle_amount mal nach Glitzermaterial suchen
  // Gibt ggf. Zugriff auf Local(-1), was aber einfach 0 zurückgibt
  var x,y,c,m;
  for(var i = 0; i < sparkle_amount; i++)
    if (Local(m = GetMaterial(x = Random(LandscapeWidth()), y = Random(LandscapeHeight()))))
      {
      if(Local(m) == 1) // Gold
        Sparkle(x,y, 255,255,128+Random(128), 1);
      else if(Local(m) == 2) // Crystal
        Sparkle(x,y, c=128+Random(128),c,255, 1);
      else if(Local(m) == 3) // Ice
        Sparkle(x,y, 128+Random(128),255,255, 1);
      }
  
  // mehrmals nach einem möglichen Glitzerobjekt suchen
  if (!Random(15))
    for (var obj in FindObjects(Find_OCF(OCF_Collectible), Find_NoContainer(), Find_Or(Find_ID(GOLD), Find_ID(CRYS), Find_ID(ICE1)), Find_Func("Stuck")))
      if (!Random(10)) if (!Random(11-placement_factor))
        if (!GetXDir(obj) && !GetYDir(obj))
          // So, alle Tests sind überstanden, jetzt wird geglänzt.
          Sparkle(GetX(obj)-3+Random(7),GetY(obj)-3+Random(7), 255,255,255);
}

private func Sparkle(int x, int y, int r, int g, int b, bool allow_big_sparkles) {
  // Entweder einen kleinen, starken Blitzer...
  if(Random(3) || !allow_big_sparkles) {
    CreateParticle("MaterialSpark", x,y, 0,0, 5*5+Random(5*5), RGBa(r,g,b,0)); 
  }
  // ...oder ein größeres, schwaches Leuchten
  else {
    CreateParticle("MaterialSpark", x,y, 0,0, 10*5+Random(40*5), RGBa(r,g,b,192+Random(64)));
  }
  return(1);
}
