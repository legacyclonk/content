/* Wiederbelebungszauber */

#strict

static const XCRS_Priority = 50;

public func Activate(object pCaster, object pCaster2)
  {
  // Zauberer ermitteln
  if (pCaster2) pCaster = pCaster2;
  // Effekt starten (wenn nicht schon einer vorhanden und genug Energie)
  if(GetEnergy(pCaster)<=10) return(0, Message("$MsgNotEnoughEnergy$", pCaster) );
  if(AddEffect("ReincarnationPSpell", pCaster, XCRS_Priority, 0, 0, GetID()))
    {
    // Energie opfern
    DoEnergy(-10,pCaster);
    // Magie kann man hören, ganz klar ;)
    Sound("Magic*");
    // Fertig - das Zauberobjekt wird nun nicht mehr gebraucht
    RemoveObject();
    return(1);
    }
  RemoveObject();
  Sound("Error");
  Message("$MsgNoFurtherReincarnation$", pCaster);
  return(0);
  }

protected func FxReincarnationPSpellEffect(string szNewEffect)
  {
  if(szNewEffect eq "ReincarnationPSpell") return(FX_Effect_Deny);
  }

protected func FxReincarnationPSpellStop(object pTarget, int iEffectNumber, int iReason)
  {
  // Nur beim Tod des Clonks
  if (iReason != FX_Call_RemoveDeath) return(FX_OK);
  // schonmal wiederbelebt
  if (EffectVar(0,pTarget,iEffectNumber)) return(FX_OK);
  // Effekt erhalten, wenn der Clonk lebt: Wurde wohl durch einen anderen Effekt wiederbelebt :)
  if (GetAlive(pTarget)) return(FX_Stop_Deny);
  // Clonk wiederbeleben!
  SetAlive(true, pTarget);
  // Steckt im Tarnstein?
  if (Contained(pTarget)) if (Contained(pTarget)->~IsCamouflageContainer())
   Contained(pTarget)->DoDamage(100);
  // Aber erst mal "schlafen" lassen
  AddEffect("IntReincDelay", pTarget, 1, 35, 0, GetID());
  // Effekt bleibt nun da (der Clonk kann nur einmal auf diese Weise wiederbelebt werden)
  EffectVar(0,pTarget,iEffectNumber)=true;
  return(FX_Stop_Deny);
  }

protected func FxIntReincDelayStart(object pTarget, int iNumber)
{
  // alle höheren Effekte löschen
  var iEffect;
  for(var i = GetEffectCount(0, pTarget, XCRS_Priority); i > 0; --i)
    if(iEffect = GetEffect(0,pTarget,i-1, XCRS_Priority))
      if(iEffect != iNumber)
        RemoveEffect(0,pTarget,iEffect);
  // totstellen
  if (pTarget->~IsClonk())
    {
    pTarget->Sound("Die");
    pTarget->DeathAnnounce();
    }
  pTarget->SetAction("Dead", 0,0, true);
  // anzünden
  Incinerate(pTarget);
}

protected func FxIntReincDelayDamage(object pTarget, int iNumber, int iDmg, int iCause, int iCausePlayer)
{
  // Kein Schaden während Reinkarnation
  if (iCause & FX_Call_Energy) if (iDmg > 0) { DoEnergy(-iDmg); }
}

protected func FxIntReincDelayTimer(object pTarget, int iNumber, int iTime)
{
  if(!OnFire(pTarget)) Incinerate(pTarget);
  if(iTime > 1000) return(FX_Execute_Kill);
}

protected func FxIntReincDelayStop(object pTarget) { Reincarnate(pTarget); return(FX_OK); }

protected func Reincarnate(object pTarget)
  {
  // löschen
  Extinguish(pTarget);

  // Spieler schon tot
  if(GetOwner(pTarget)==-1) return(0);

  // Clonk wiederbeleben
  MakeCrewMember(pTarget, GetOwner(pTarget));
  pTarget->SetAction("FlatUp", 0, 0, true);
  SetComDir(COMD_Stop,pTarget);
  SetXDir(0,pTarget);
  // Energie geben
  DoEnergy(10, pTarget);
  DoMagicEnergy(-GetMagicEnergy(pTarget),pTarget);
  // Nachricht
  Sound("Magic*", 0, pTarget);
  PlayerMessage(GetOwner(pTarget),"$Raise$",0,GetName(pTarget));
  // OK
  return(1);
  }

/* Zaubercombo */

public func GetSpellClass() { return(FIRE); }
public func GetSpellCombo() { return ("511"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
