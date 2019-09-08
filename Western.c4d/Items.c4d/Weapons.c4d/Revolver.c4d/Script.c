/*-- Revolver --*/

#strict

local iAmmoCount;
/* Steuerung */

public func ControlThrow (pClonk) {
  // Wieder bereit zum Feuern?
  if(GetEffect("ReloadGun", this())) return(1);
  // Keine Munition mehr
  if(!iAmmoCount) return(CheckAmmo());
  // Clonk soll die Waffe ziehen und abfeuern
  if(!pClonk->~FireGun()) return(0);
  // Okay
  return(1);
}

/* Manuelles Nachladen */
public func Activate (pClonk)
{
  // Noch genug Muni vorhanden?
  if(iAmmoCount==6) return(Sound("RevolverNoAmmo", 0, pClonk));
  // Clonk hat keine Muni mehr?
  if(!pClonk->~GetCartridgeCount()) return(Sound("RevolverNoAmmo", 0, pClonk));
  // Inventar wieder auf Revolver verschieben
  ShiftContents(pClonk, 1, GetID());
  // Nachladen
  pClonk->~LoadGun();
  return(1);
}

/* Feuern */

public func Fire () {
  // Richtung des Trägers abfragen
  var dir = -10;  if(GetDir(Contained()) == DIR_Right()) dir = 10;
  // Ammo suchen
  var ammo = Contents();
  iAmmoCount--;
  // Besitzer des Projektils setzen
  SetOwner(GetOwner(Contained()),ammo); 
  // Ammo abfeuern
  Exit(ammo, dir, -2 + Random(5), 0, 0, 0, 0 );
  // Ein Captain trifft schmerzhafter
  if(Contained()->~IsCaptain()) ammo->Launch(GetDir(Contained()), 10+Random(8));
  else ammo->Launch(GetDir(Contained()), 7+Random(8));
  // Mündungsfeuer
  CreateParticle("MuzzleFlash", 10*(dir/10), -2, dir, 0, 35, RGBa(255,255,255,0), Contained());
  // Rauch
  Smoke(dir, -2 + Random(5), 2);
  Smoke(dir, -2 + Random(5), 3);
  // Sound
  Sound("RevolverShot");
  // Wartezeit zwischen den Schüssen
  AddEffect("ReloadGun", this(), 101, 20);
  return(1);
}

/* TimerCall (entfernt): Bei Bedarf Träger nach passender Munition durchsuchen */
// Neu: Spieler drückt Werfen zum Laden der Waffe

protected func CheckAmmo() {
  // Waffe wird nicht getragen
  if (!Contained()) return(0);
  if (! (GetOCF(Contained()) & OCF_CrewMember()) ) return(0);
  // Clonk bereit?
  if(GetAction(Contained()) ne "Walk" && GetAction(Contained()) ne "Ride" && GetAction(Contained()) ne "RideStill") return(1);
/*  // Patronenhülsen rauswerfen (3x auch wenn 6 drin sind), allerdings nicht beim ersten Laden
  if(loaded)
    {
    CreateParticle("Casing", dir/2, -2 + Random(5), -(dir/10)*RandomX(1, 5), -RandomX(3, 7), 15, RGBa(250, 140, 80, 0));
    CreateParticle("Casing", dir/2, -2 + Random(5), -(dir/10)*RandomX(1, 5), -RandomX(3, 7), 15, RGBa(250, 140, 80, 0));
    CreateParticle("Casing", dir/2, -2 + Random(5), -(dir/10)*RandomX(1, 5), -RandomX(3, 7), 15, RGBa(250, 140, 80, 0));
    // Patronen sind raus
    loaded = 0;
    }*/
  // Hat der Träger noch Munition?
  if(! Contained()->~GetCartridgeCount()) return(Sound("RevolverNoAmmo"));

  // Inventar wieder auf Revolver verschieben
  ShiftContents(Contained(), 1, GetID());

  // Dann soll der Träger hübsch nachladen
  Contained()->~LoadGun();
  return(1);
}

protected func LoadAmmo() {
  // Munition einladen bis Maximum erreich oder keine mehr verfügbar
  var obj;
  while( iAmmoCount < 6  &&  Contained()->~GetCartridgeCount() )
    {
    obj = Contained()->~GetCartridge();
    iAmmoCount++;
    Enter(this(), obj);
    }
  // Inventar wieder auf Revolver verschieben
  ShiftContents(Contained(), 1, GetID());
  // Sound
  Sound("RevolverLoad");
  return(1);
}

/* Objekt ist eine Waffe */
public func IsWeapon () { return(1); }

/* Objekt ist eine Handfeuerwaffe */
public func IsGun () { return(1); }

/* Objekt ist kein Gewehr */
public func IsRifle () { return(0); }

/* Objekt wurde ausgewählt */
public func Selection () {
  Sound("RevolverDraw");
}

/* Ammo IDs */
private func GetAmmoID () { return(CSHO); }

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }
