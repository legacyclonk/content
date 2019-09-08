/* Kleines Kraftfeld (Zauber) */

#strict 2

public func Activate(pCaster, pCaster2)
{
  // Zauberer ermitteln
  if (pCaster2) pCaster = pCaster2;
  if (Contained(pCaster))
    if (Contained(pCaster)->~IsGolem())
      pCaster = Contained(pCaster);
  // Effekt prüfen
  var iChkEff;
  if (iChkEff = CheckEffect("FFBridgePSpell", pCaster, 150)) { RemoveObject(); return (iChkEff!=-1); }
  // Kraftfeld erstellen
  ObjectSetAction(CreateObject(FRCS,0,50,GetOwner()),"Field",pCaster);
  CastParticles("MSpark", 5, 50, 0,0, 100, 200, RGBa(100,100,255,128), RGBa(0,0,255,0));
  CreateParticle("MSpark", 0,0, 1000, RGBa(50,50,255,150));
  RemoveObject();
  return true;
}

public func GetSpellClass(object pMage) { return WATR; }
public func GetSpellCombo(pMage) { return "266"; } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
