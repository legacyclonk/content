/* Elexir 2 */

#strict

func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  // Auswählen wenn möglich
  if (pClonk->~DoSpellSelect(this(),300,pCaster)) return(1);

  // Clonk kann nicht auswählen: Clonk heilen wenn negativer Effekt, Feuer oder Heilung nötig
  if (SelectorTarget(pClonk))
   return(ActivateTarget(pClonk, pClonk));

  // ansonsten geth nich
  RemoveObject();
  return(0);
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Typ heilen
  AddEffect("CurePlusPSpell",pTarget,250,4,0,GetID());
  // Objekt löschen
  return(RemoveObject());
}

public func SelectorTarget(object pTarget)
{
  // nur außerhalb von Gebäuden
  if(Contained(pTarget)) return();
  // muss ein Mannschaftsmitglied sein
  if(!(GetOCF(pTarget)&OCF_CrewMember)) return();
  // darf nicht feindlich sein oder NTMG muss aktiviert sein
  if(Hostile(GetOwner(),GetOwner(pTarget)) && !ObjectCount(NTMG)) return();
  // hat schon volle Energie, brennt nicht und hat keine negativen Effekte auf sich
  if(GetEnergy(pTarget)==GetPhysical("Energy",0,pTarget)/1000
     && !GetEffect("*NSpell",pTarget)
     && !OnFire(pTarget)) return();
  // Wird noch nicht durch Genesung geheilt
  if (GetEffect("CurePlusPSpell",pTarget)) return();
  // sichtbar
  if(!Visible(GetOwner(),pTarget)) return();
  return(1);
}

func FxCurePlusPSpellStart(pClonk, iEffectNumber, iTemp)
{
  // negative Effekte beseitigen
  var iEffect;
  for(var i = GetEffectCount(); i > 0; --i) {
    iEffect = GetEffect("*NSpell",pClonk,i);
    if(iEffect) RemoveEffect(0,pClonk,iEffect);
  }
  // Feuer löschen
  Extinguish(pClonk);
  // Auftauen
  Unfreeze(pClonk);
  // Effekt
  CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),0,0,150,RGB(64,255,64));
}

func FxCurePlusPSpellTimer(pClonk, iEffectNumber, iEffectTime)
{
 // Clonk heilen
  var energy_before = GetEnergy(pClonk);
  DoEnergy(+1,pClonk);
  var energy_after = GetEnergy(pClonk);
  // Ende der Heilung? Spezial: Wenn die Heilung tatsächlich geschadet hat (Antiheilfluch), sollte der Zauber beim fast toten Clonk abbrechen.
  if (energy_after == GetPhysical("Energy", 0, pClonk)/1000 || (energy_after < energy_before && energy_after <= 1)) return (FX_Execute_Kill);
  // Partikeleffekte für den betroffenen
  CreateParticle("MSpark",GetX(pClonk)+RandomX(-8,8),GetY(pClonk)+8,GetXDir(pClonk)/2,RandomX(-10,-20)+GetYDir(pClonk)/2,RandomX(40,60),RGB(127,127,255));
  CreateParticle("MSpark",GetX(pClonk)+RandomX(-8,8),GetY(pClonk)+8,GetXDir(pClonk)/2,RandomX(-10,-20)+GetYDir(pClonk)/2,RandomX(20,50),RGB(127,255,127));

}

protected func FxCurePlusPSpellInfo() { return("{{ELX2}} $SpellInfo$"); }

func NoTowerMagic() { return(true); }

/* Zaubercombo */

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return ("566"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
