/*-- Gravitation rauf --*/

#strict

public func Activate(caster, real_caster) {
  // Zaubernden Clonk ermitteln
  var clonk = caster;
  if (real_caster) clonk = real_caster;
  // Kombiniert mit Beton -> Clonk in den Boden stecken
  var obj;
  if (!Contained(clonk) && (obj = FindContents(CNCR, clonk))) {
    var height = GetDefCoreVal("Height", "DefCore", GetID(clonk));
    var radius = RandomX(25, 35);
    var x = GetX(clonk);
    var y = GetY(clonk);
    // Unter dem Clonk freischütteln und buddeln
    DigFree(x, y+height+radius, height/2);
    ShakeFree(x, y+height+(radius/2), radius);
    // Tiefstes freigeschüttelte Y-Position ermitteln
    while (!GBackSolid(AbsX(x), AbsY(y++ + height))) {}
    // Effekte!
    DrawParticleLine("MSpark",
      0, 0, 0, AbsY(y),
      height*3/2, height*5/2,
      RGBa(255-Random(50),255-Random(50), 0,128),
      RGBa(155-Random(50),155-Random(50), 0,128),
      -10);
    // Zauberer an ermittelte Stelle versetzen
    SetPosition(x, y, clonk);
    // Objekt entfernen
    RemoveObject(obj);
  }
  // Sonst normale Wirkung (Gravitation erhöhen)
  else {
    // Gravitation bereits auf Maximum?
    if (GetGravity() >= 290)
      return(0, Message("$MsgAlreadyMaxed$", caster), RemoveObject());
    // Sonst erhöhen
    // Effekt global starten
    if (!AddEffect("GravChangeUSpell", 0, 150, 80, 0, MGDW, 20))
      return(0, RemoveObject());
    Message("$MsgRaised$", caster, GetGravity());
  }
  Sound("Magic1");
  RemoveObject();
  return(1);
}

// Zauberklassen für den Magen
public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("255"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
