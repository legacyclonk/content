/* Erdbebenzauber */

#strict

public func Activate(object pCaster, object pCaster2)
  {
  var iX=(GetDir(pCaster)*2-1)*RandomX(100,120);
  var iDY=0;
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("EarthquakeNSpell", 0, 150)) return(iResult!=-1 && RemoveObject());
  // in der Luft soll kein Erdbeben erzeugt werden
  while(!GBackSolid(iX,iDY) && iDY+GetY()<=LandscapeHeight()) iDY+=5;
  // Effekt ausführen
  LaunchEarthquake(GetX()+iX,GetY()+iDY);
  // An Zielposition nochmal extra durchschütteln
  for(var i;i<3;++i) ShakeFree(GetX()+iX+RandomX(-16,16),GetY()+iDY+RandomX(-16,16),RandomX(14,26));
  Sound("Magic1");
  RemoveObject();
  return(1);
  }


public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("666"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
