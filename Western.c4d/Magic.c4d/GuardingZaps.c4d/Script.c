/* Schutzzaps */

#strict

public func Activate(pCaster,pRealcaster)
{
  if(pRealcaster) pCaster = pRealcaster;
  
  var pCombo;

  // Kombo: Mit Gold gibt Zapnest
  if(pCombo = FindContents(GOLD, pCaster)) {
    ChangeDef(ZAPN,pCombo);
    Sound("Magic1");
    RemoveObject();
    return(1);
  }

  // Auswählen wenn möglich
  if(pCaster->~DoSpellSelect(this(),350)) return(1);

  if(GetZapCount(pCaster) > 4) {
    PlayerMessage(GetOwner(pCaster),"$MsgHasZaps$",pCaster,GetName(pCaster));
    Sound("Error",0,0,0,GetOwner(pCaster)+1);
    RemoveObject();
    return(0);
  }

  if(!CreateZaps(pCaster)) return(0,RemoveObject());
  Sound("Magic1");
  RemoveObject();
  return(1);
}

private func CreateZaps(object pTarget) {
  var iX,iY,iTimes,iCount;
  iCount = BoundBy(6-GetZapCount(pTarget),3,6);
  for(iTimes = 0; iTimes < 30; iTimes++) {
    if(!iCount) break;
    iX = RandomX(-5,5); iY = RandomX(-5);
    if(!GBackSolid(iX,iY)) {
      LocalN("target",CreateObject(ZAP2,iX,iY,-1)) = pTarget;
      iCount--;
    }
  }
  return(6-iCount);
}

public func GetZapCount(pTarget) { return(ObjectCount2(Find_ID(ZAP2),Find_Local("target",pTarget))); }

public func ActivateTarget(object pCaller, object pTarget)
{
  // Zaps erstellen
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
