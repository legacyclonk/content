/* Luftschutzkraftfeld */

#strict 2

protected func GetForceFieldTime() { return 11500; }

/* Initialisierung */

protected func Initialize()
{
  Sound("Energize");
  SetOwner(NO_OWNER);
  SetAlive(false);
  CheckStuck();
  AddEffect("ForceFieldPSpell", this, 150, 5, this);
}

private func CheckStuck()
{
  var pClonk,iAngle,iCos,iSin,iX,iY;
  
  // Alle feststeckenden Clonks in der Nähe suchen
  for (pClonk in FindObjects(Find_InRect(-248,-110,500,216), Find_OCF(OCF_CrewMember), Find_NoContainer()))
    if(Stuck(pClonk))
    {
      // nur wenn sie im Kraftfeld feststecken
      iX=GetX(pClonk);
      iY=GetY(pClonk);
      iAngle=Angle(GetX(),GetY()+100,iX,iY)-90;
      iSin=Sin(iAngle,190)+100;
      iCos=Cos(iAngle,230);
      if(Distance(iX-GetX(),iY-GetY(),iCos,iSin)<25)
        // aus dem Kraftfeld rausschieben (nach innen)
        while(Inside(GetX(pClonk)-iX,-20,20) && Inside(GetY(pClonk)-iY,-20,20) && Stuck(pClonk))
          SetPosition(-iCos/40+GetX(pClonk),-iSin/40+GetY(pClonk),pClonk);
      // verschieben fehlgeschlagen: rückgängig machen
      if(Stuck(pClonk)) SetPosition(iX,iY,pClonk);
    }
}

/* Das Kraftfeld ausfaden */

protected func FxForceFieldPSpellTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if (ActIdle()) return FX_Execute_Kill;
  var iTime = EffectVar(0, pTarget, iEffectNumber) += 5;
  
  // Das Kraftfeld langsam ausfaden
  var iAlpha = 70 * iTime / GetForceFieldTime();
  SetClrModulation(RGBa(215 + Cos(iTime, 40),
						215 + Cos(iTime, 40),
						215 + Sin(iTime, 40),
						Sqrt(Sqrt(iAlpha**5)) ));
  
  // Nach Ablauf der Zeit wird der Effekt gelöscht
  if (iTime >= GetForceFieldTime()) return FX_Execute_Kill;
}

protected func FxForceFieldPSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Keine temporären Aufrufe
  if (fTemp) return;
  
  // Beim Ablauf der Zeit wird das Kraftfeld gelöscht
  Destroy();
}

private func Destroy()
{
  Sound("DeEnergize");
  RemoveObject();
}

/* Blitzeinschlag */

public func GetLightningDamage() { return 0; } // Luftschutzkraftfeld ist immun

public func LightningStrike(object lightning)
{
  // Beschaedigung durch Blitz
  var dmg = GetLightningDamage();
  if (dmg)
    return DoDamage(GetLightningDamage(), this, FX_Call_DmgScript, GetController(lightning)+1);
  else
    return false;
}

/* Eigenschaften */
public func IsForceField() { return true; }