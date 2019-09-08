/* Elexir */

#strict

func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  // Auswählen wenn möglich
  if (pClonk->~DoSpellSelect(this(),200,pCaster)) return(1);

  // Clonk kann nicht auswählen: Clonk heilen wenn negativer Effekt oder Heilung nötig
  if (SelectorTarget(pClonk))
   return(ActivateTarget(pClonk, pClonk));

  // ansonsten geth nich
  RemoveObject();
  return(0);
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Typ heilen
  AddEffect("CurePSpell",pTarget,250,4,0,GetID());
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
  // hat schon volle Energie und hat keine negativen Effekte auf sich
  if(GetEnergy(pTarget)==GetPhysical("Energy",0,pTarget)/1000 && !GetEffect("*NSpell",pTarget)) return();
  // Wird noch nicht durch Elixier geheilt
  if (GetEffect("CurePSpell",pTarget)) return();
  // sichtbar
  if(!Visible(GetOwner(),pTarget)) return();
  return(1);
}

func FxCurePSpellStart(pClonk, iEffectNumber, iTemp)
{
  // negative Effekte beseitigen
  var iEffect;
  for(var i = GetEffectCount(); i > 0; --i) {
    iEffect = GetEffect("*NSpell",pClonk,i);
    if(iEffect) RemoveEffect(0,pClonk,iEffect);
  }
  // Effekt
  CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),0,0,150,RGB(64,64,255));
}

func FxCurePSpellTimer(pClonk, iEffectNumber, iEffectTime)
{
  if(iEffectTime >= 80) return(-1);

  // Abbruch wenn voll geheilt
  if(GetEnergy(pClonk)==GetPhysical("Energy",0,pClonk)/1000) return(-1);

  // Partikeleffekte für den betroffenen
  CreateParticle("MSpark",GetX(pClonk)+RandomX(-8,8),GetY(pClonk)+8,GetXDir(pClonk)/2,RandomX(-10,-20)+GetYDir(pClonk)/2,RandomX(40,70),RGB(127,127,255));
  // Clonk heilen
  DoEnergy(+1,pClonk);
  return(1);
}

protected func FxCurePSpellInfo() { return("{{ELX1}} $SpellInfo$"); }

// Versagt im Zauberturm
func NoTowerMagic() { return(true); }

/* Zaubercombo */

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return ("544"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
