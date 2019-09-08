/* Magische Brücke */

#strict

public func Activate(caster, real_caster) {
  // Zaubernden Clonk ermitteln
  var clonk = caster;
  if (real_caster) clonk = real_caster;
  // Effekt prüfen
  var iChkEff;
  if (iChkEff = CheckEffect("FFBridgePSpell", 0, 150)) return (iChkEff!=-1 && RemoveObject());
  // ID des zu erzeugenden Brückenobjekts ermitteln
  var id = FBRG;
  var obj;
  if (obj = FindContents(METL, clonk)) {
    id = BRDG;
    RemoveObject(obj);
  }
  // Ziel-X der Brücke ermitteln
  // Für schwebende/springende Clonks die Brücke auch unterhalb des Clonks erstellen
  var x = GetDefCoreVal("Width", "DefCore", id)*55/100;
  if (!Contained(clonk) && !GetContact(clonk, -1, 8)) // CNAT_Bottom
    x -= x/10;
  if (GetDir(caster) == DIR_Left())
    x *= -1;
  // Ziel-Y der Brücke ermitteln
  var y = GetDefCoreVal("Height", "DefCore", id)*2;
  // Brücke erzeugen
  var bridge = CreateObject(id, x, y, GetOwner());
  // Sonderbehandlung nach IDs
  if (id == FBRG) {
    SetPosition(GetX(bridge),GetY(bridge)+2,bridge);
    ObjectSetAction(bridge, "Field", caster);
  }
  if (id == BRDG)
    PrivateCall(bridge, "Lock", 1, 1);
  // Effekte und so
  Sound("Magic1");
  CastParticles("MSpark", 5, 50, 0,0, 100, 200, RGBa(100,100,255,128), RGBa(0,0,255,0));
  CreateParticle("MSpark", 0,0, 1000, RGBa(50,50,255,150));
  // Fertig!
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("233"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
public func GetResearchBase () { return (MFFS); }
