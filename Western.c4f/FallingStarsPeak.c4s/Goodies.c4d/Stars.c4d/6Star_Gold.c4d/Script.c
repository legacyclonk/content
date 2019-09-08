/*-- Goldener Stern --*/

#strict

#include _STY

public func GetColor() { return(RGB(200,200,100)); }

public func GetPoints() { return(10); }

public func GetTime() { return(30); }

public func GetSpeedBoost() { return(25); }

public func Entrance(pContainer)
{
  if(GetID(pContainer)==SHOF)
  {
    AddEffect("Sheriff", GetCursor(GetOwner()), 200, 35*30, 0, GetID());
    Sound("Trumpet");
    RemoveObject();
  }
  SetOwner(GetOwner(pContainer));
}

public func ShotHit(iPlr)
{
  if(iPlr<0) return();
  if(FindContents(GetID(), GetCursor(iPlr)))
  {
    DoPoints(GetPoints(), iPlr);
    Sound("Crystal*");
    Hit();
    return();
  }
//  DoTime(GetTime(), iPlr);
//  DoPoints(GetPoints(), iPlr);
//  AddEffect("Sheriff", GetCursor(iPlr), 200, 35*15, 0, GetID());
//  if(Random(100)<=5) CreateObject(MBAG);
  Enter(GetCursor(iPlr));
  SetOwner(iPlr);
  Sound("Crystal*");
//  Hit();
}

func FxSheriffStart(object pTarget, int iNumber, bool fTmp)
{
  SetGraphics(0, pTarget, SHRF);
  // Portrait speichern
  EffectVar(1, pTarget, iNumber) = GetPortrait(pTarget);
  EffectVar(2, pTarget, iNumber) = GetPortrait(pTarget, 1);
  // Portrait anpassen
  SetPortrait("1", pTarget, SHRF);
}

func FxSheriffStop(object pTarget, int iNumber, int iReason, bool fTmp)
{
  // Grafik zurücksetzen
  SetGraphics(0, pTarget, GetID(pTarget));
  // Portrait zurücksetzen
  SetPortrait(EffectVar(1, pTarget, iNumber), pTarget, EffectVar(2, pTarget, iNumber));
  if(!fTmp) { CastObjects(SPRK,15,30,GetX(pTarget),GetY(pTarget)); Sound("DeEnergize", pTarget);}
}
