/*-- Das große Finale --*/

#strict

// Local 0: Ablauf

func Initialize() { Local(0)=0; }

/* Ablauf */

private func Proceed () {
  ObjectCall(this(),Format("Process%d",Local(0)));
  ++Local(0);
}
  
/* Los geht's */  
  
public func Process0 () {
  Sound("InukConjuration",1);
  SetWind(0);
}
  
/* Es werde Licht */  
    
public func Process2 () {
  // Weg mit der Polarnacht
  RemoveAll (STRS);
  RemoveObject(FindObject(NRTS));
  // Sterne entfernen
  RemoveAll(STAR);
  // Roter Himmel (deaktiviert auch die Tageszeiten)
  FindObject(TIME) -> StartSkyFade (RGBa(255,0,0,64),RGB(255,255,0));
}
  
/* Alten Meeresspiegel entfernen */

public func Process9 () {
  RemoveAll(DRAI);
}
  
/* Wassermassen entfernen */  
  
public func Process10 () {
  CreateObject(_OCA, -GetX(), 420 - GetY(), -1);
}
  
/* Neuen Meeresspiegel bestimmen */

public func Process36 () {
  CreateObject(SELV,0,0,-1);
}
   
/* Klima ändern */

public func Process40 () {
  // Warm & Sommer
  SetClimate(50);
  SetSeason(50);
  SetTemperature(100);
}
  
/* Blauer Himmel */

public func Process45 () {
  FindObject(TIME) -> StartSkyFade (RGBa(0,0,192,128),RGB(128,128,255));
}
  
/* Vegetation */

public func Process50 ()  { Vegetate(); }
public func Process150 () { Vegetate(); }
public func Process200 () { Vegetate(); }
public func Process250 () { Vegetate(); }
  
private func Vegetate () {
  for(var i=0;i<3;++i) {
    PlaceVegetation(PLM1,0,0,LandscapeWidth(),LandscapeHeight(),Random(80000));  
    PlaceVegetation(PLM2,0,0,LandscapeWidth(),LandscapeHeight(),Random(80000));  
    PlaceVegetation(BUSH,0,0,LandscapeWidth(),LandscapeHeight(),Random(80000));  
    PlaceVegetation(TRE2,0,0,LandscapeWidth(),LandscapeHeight(),Random(80000));  
    PlaceVegetation(TRE3,0,0,LandscapeWidth(),LandscapeHeight(),Random(80000));
  }
}
  
/* Siedlungsgrundlage */

public func Process80 () {
  for(var i,j=GetPlayerCount();j;++i) if(GetPlayerName(i)) {
    GainSettlementBasics(GetPlayerByIndex(i));
    --j;
  }
}
  
private func GainSettlementBasics (int iPlr) {
  // Bauwissen
  SetPlrKnowledge(iPlr,HUT1);
  SetPlrKnowledge(iPlr,FNDR);
  SetPlrKnowledge(iPlr,RSRC);
  // Startmaterial
  CreateContents(FLAG,GetCursor(iPlr));
}
  
/* Vogelzwitschern */
  
public func Process180 () {
  CreateObject(BRDS,0,0,-1);
}

