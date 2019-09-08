/* Untoten erwecken */

#strict

public func Activate(object pCaster, object pRealcaster) {

  var pDead, pClonk=pRealcaster, iRange=300;
  if(!pClonk) pClonk=pCaster;

  // Auswählen wenn möglich
  if (pClonk->~DoSpellSelect(this(),iRange/2, pCaster)) return(1);

  // Clonk kann nicht auswählen: nächsten toten Clonk erwecken
  while(pDead=FindObject(0,-iRange/2,-iRange/2,iRange,iRange,OCF_Living,0,0,NoContainer(),pDead))
    if(pDead->~IsClonk())
      if(!GetAlive(pDead))
        if(!GetEffectCount("*",pDead))
          if(AddEffect("UndeadSpell",pDead,82,10,0,GetID(),GetOwner(pCaster)))
            return(1,RemoveObject());

  pClonk->Sound("Error");
  return(0,Message("$MsgNoUndeads$", pClonk), RemoveObject());
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Typ heilen
  AddEffect("UndeadSpell",pTarget,82,1,0,GetID(),GetOwner(pCaller));
  // Objekt löschen
  return(RemoveObject());
}

public func SelectorTarget(object pTarget)
{
  // muss ein toter Clonk sein
  if(GetOCF(pTarget) & OCF_Living) 
    if(pTarget->~IsClonk())
      if(!GetAlive(pTarget))
        if(!GetEffectCount("*",pTarget))
          return(1);
  return(0);
}

private func DoPhysical(string szPhysical, int iInc, int iMode, object pObject)
{
  SetPhysical(szPhysical,Min(GetPhysical(szPhysical,0,pObject)+iInc,100000),iMode,pObject);
}

protected func FxUndeadSpellStart(object pTarget, int iEffectNumber, int iTemp, int iController) {
  var iColor;
  // temporäre aufrufe ignorieren
  if(iTemp) return(0);
  // wiedererwecken
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  SetAlive(1,pTarget);

  iColor = GetColorDw(pTarget);
  // Zeichenart ändern
  SetObjectBlitMode(4,pTarget);
  // Hautfarbe färben :)
  SetClrModulation(RGB(128,255,128),pTarget);
  // Farbe so lassen: die Farbänderung durch SetOwner muss rückgängig gemacht werden
  SetOwner(iController,pTarget);
  SetColorDw(iColor,pTarget);
  // neue Physicals (temporär natürlich):
  // Geister können nicht hangeln und nicht klettern
  ResetPhysical(pTarget);
  SetPhysical("CanScale",0,PHYS_Temporary,pTarget);
  SetPhysical("CanHangle",0,PHYS_Temporary,pTarget);

  // Laufgeschwindigkeit, Schwimmgeschwindigkeit und Sprunghöhe werden reduziert
  DoPhysical("Jump",-10000,PHYS_Temporary,pTarget);
  DoPhysical("Walk",-20000,PHYS_Temporary,pTarget);
  DoPhysical("Swim",-10000,PHYS_Temporary,pTarget);
  DoEnergy(+100,pTarget);
  DoBreath(+100,pTarget);

  // Ein Untoter kann sehr gut töten
  DoPhysical("Fight",+60000,PHYS_Temporary,pTarget);

  // zum Manschaftsmitglied machen
  // MakeCrewMember(pTarget,iController);
  // Fertig
  return(1);
}

protected func FxUndeadSpellTimer(object pTarget, int iEffectNumber, int iTime) {
  DoEnergy(-(!Random(40)),pTarget);

  // Selbes Verhalten wie bei Replikationszauber!

  // Habe schon was zu tun: Beenden
  if(GetCommand(pTarget) && Random(10)) return();
  // Hochintellente Ki
  var pObj;
  // Gegnersuche
  while(pObj=FindObject(0, GetX(pTarget), GetY(pTarget), -1, -1, OCF_CrewMember(), 0, 0, NoContainer(), pObj))
  {
    if(ObjectDistance(pTarget, pObj) > 800) continue;
    if(!Hostile(GetOwner(pObj), GetOwner(pTarget))) continue;
    var pEnemy=pObj;
    break;
  }
  //Attacke
  if(pEnemy) SetCommand(pTarget, "Attack", pEnemy);

}

protected func FxUndeadSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Wenn Untoter Clonk stirbt, nicht entfernen um andere Untoteneffekte zu sperren
  // (Untote können nicht wiedererweckt werden)
  if(iReason==4) return(-1);
}

protected func FxUndeadSpellEffect(string szNewEffectName)
{
  // keine weiteren Untoteneffekte zulassen (s.o.)
  if(szNewEffectName eq "UndeadSpell") return(-1);
}

protected func FxUndeadSpellInfo() { return("{{RUND}} $UndeadInfo$"); }

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("622"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
