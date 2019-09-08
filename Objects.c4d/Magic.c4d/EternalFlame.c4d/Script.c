/* Ewige Flammen */

#strict

public func Activate(pCaster)
  {
  Sound("Magic1");
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("EtrnlFlamesNSpell", 0, 75)) return(iResult!=-1 && RemoveObject());
  // Zauber ausführen
  Sound("Inflame"); Sound("Fuse");
  var iPos = -60;
  if (GetDir(pCaster) == DIR_Right()) iPos = 60;;
  // Flammen erzeugen
  CastObjects(MFLM, 6, 20, iPos, 0);
  // Fertig; Zauber entfernen
  RemoveObject();
  return(1);
  }

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("455"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
public func GetResearchBase () { return (FRFS); }