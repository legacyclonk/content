/* Wiederbelebungszauber */

#strict 2

public func Activate(object pCaster, object pCaster2)
  {
  // Zauberer ermitteln
  if (pCaster2) pCaster = pCaster2;
  // Effekt starten (wenn nicht schon einer vorhanden und genug Energie)
  if(GetEnergy(pCaster)<=10) { Message("$MsgNotEnoughEnergy$", pCaster); return 0; }
  if(AddEffect("ReincarnationPSpell", pCaster, 180, 0, 0, GetID()))
    {
    // Energie opfern
    DoEnergy(-10,pCaster);
    // Magie kann man hören, ganz klar ;)
    Sound("Magic*");
    // Fertig - das Zauberobjekt wird nun nicht mehr gebraucht
    RemoveObject();
    return 1;
    }
  RemoveObject();
  Sound("Error");
  Message("$MsgNoFurtherReincarnation$", pCaster);
  return 0;
  }

protected func FxReincarnationPSpellEffect(string szNewEffect)
  {
  if(szNewEffect == "ReincarnationPSpell") return -1;
  }

protected func FxReincarnationPSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  // Nur beim Tod des Clonks
  if(iReason != 4) return(1);
  // schonmal wiederbelebt
  if(EffectVar(0,pTarget,iEffectNumber)) return(1);
  // Effekt erhalten, wenn der Clonk lebt: Wurde wohl durch einen anderen Effekt wiederbelebt :)
  if(GetAlive(pTarget)) return(-1);
  // Die Reinkarnation
  AddEffect("IntReincDelay",pTarget,1,35,0,GetID());
  for(var iObj, i = ContentsCount(0,pTarget); i > 0; i--) {
    iObj = Contents(i-1,pTarget);
    Exit(iObj); SetPosition(GetX(pTarget),GetY(pTarget),iObj);
  }
  ObjectSetAction(pTarget,"Dead");
  pTarget->~DeathAnnounce();
  Sound("Die",0,pTarget);
  SetAlive(1,pTarget);
  SetCrewEnabled(0,pTarget);
  SetObjectLayer(pTarget,pTarget);
  // Effekt bleibt nun da (der Clonk kann nur einmal auf diese Weise wiederbelebt werden)
  EffectVar(0,pTarget,iEffectNumber) = true;
  return(-1);
}

protected func FxIntReincDelayStart(object pTarget, int iNumber, int iTemp)
{
  if(iTemp) return(0);
  EffectVar(0,pTarget,iNumber) = GetObjPlrViewRange(pTarget);
  // Damit der Spieler nicht erfährt, dass dieser Clonk eine Reinkarnation hat
  Schedule("SetPlrViewRange(Max(GetObjPlrViewRange()-10,0),0,1)",1,Min(EffectVar(0,pTarget,iNumber)/10+1,999),pTarget);
//  SetPlrViewRange(0,pTarget);
  return(1);
}

protected func FxIntReincDelayDamage(object pTarget, int iNumber, int iDamage, int iCause) { return(0); }

protected func FxIntReincDelayTimer(object pTarget, int iNumber, int iTime) { if(iTime > 1000) return(-1); }

protected func FxIntReincDelayStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  SetPlrViewRange(EffectVar(0,pTarget,iNumber),pTarget);
  if(iReason == 3) {
    Kill(pTarget);
    return(1);
  }
  return(Reincarnate(pTarget));
}

protected func Reincarnate(object pTarget)
{
  // Clonk ist nicht verfügbar
  if(!pTarget) return(0);
  // Spieler ist nicht verfügbar
  if(GetOwner(pTarget) == -1) return(0);

  // Clonk wiederbeleben
  SetAlive(1,pTarget);
  SetCrewEnabled(1,pTarget);
  SetObjectLayer(0,pTarget);
  if(!GetCursor(GetOwner(pTarget))) SetCursor(GetOwner(pTarget),pTarget);
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  SetComDir(COMD_Stop,pTarget);
  SetXDir(0,pTarget);
  Extinguish(pTarget);
  DoEnergy(10-GetEnergy(pTarget), pTarget);
  DoMagicEnergy(-GetMagicEnergy(pTarget),pTarget);

  // Nachricht
  Sound("Magic*",1,pTarget,0,GetOwner(pTarget)+1);
  PlayerMessage(GetOwner(pTarget),"$Raise$",0,GetName(pTarget));
  // OK
  return(1);
}

/* Zaubercombo */

public func GetSpellClass() { return(FIRE); }
public func GetSpellCombo() { return ("511"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
