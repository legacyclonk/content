/* Schutzzaps */

#strict

func Activate(pCaster,pRealcaster)
  {
  var pClonk;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;
  
  // Kombo: Mit Gold gibt Zapnest
  var pCombo;
  if (pCombo = FindContents(GOLD, pClonk))
    {
    ChangeDef(ZAPN, pCombo);
    return(RemoveObject());
    }

  // Auswählen wenn möglich
  if (pClonk->~DoSpellSelect(this(),350,pCaster)) return(1);

  CreateZaps(pClonk);
  RemoveObject();
  return(1);
}

private func CreateZaps(object pTarget) {
  var iX,iY;
  for(var i=6;i>0;--i)
    {
    iX=Random(11)-5;
    iY=Random(11)-5;
    if(!GBackSolid(iX,iY)) LocalN("target",CreateObject(ZAP2,iX,iY,-1))=pTarget;
    }
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Typ heilen
  CreateZaps(pTarget);
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
  // sichtbar
  if(!Visible(GetOwner(),pTarget)) return();
  return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("666"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
