#strict
#include ROCK

local iLastDamageFrame; // FrameCounter beim letzten Aufprall des Steines
local iLastDamageAmount; // Schadensmenge fürs letzte Aufprallen

public func Activate(object pController)
{
  [$DescGetOut$]
  // Träger aktiviert den Stein: Clonk herausschmeißen
  return(DoEnd(true));
}

protected func ContainedDigDouble()
{
  [$DescGetOut$]
  if (Contained()) return(Sound("Error"));
  return(DoEnd());
}

protected func ContainedLeft(pClonk)
{
  [$DescMessage$]
  if(GetContact(this(), -1, 10) & 10)
    {
    SetXDir(-7);
    SetYDir(-11);
    // Richtigen Controller setzen
    SetController(GetController(pClonk),this());
    }
  return(1);
}

protected func ContainedDown()
{
  return(1);
}

protected func ContainedRight(pClonk)
{
  [$DescMessage$]
  if(GetContact(this(), -1, 9) & 9)
    {
    SetXDir(7);
    SetYDir(-11);
    // Richtigen Controller setzen
    SetController(GetController(pClonk),this());
    }
  return(1);
}

protected func ContainedUp(pClonk)
{
  [$DescMessage$]
  if(GetContact(this(), -1, 8) & 8)
    {
    SetYDir(-17);
    // Richtigen Controller setzen
    SetController(GetController(pClonk),this());
    }
  return(1);
}

protected func Completion()
{
  SetAction("Wait4Ending");
  return(1);
}

protected func End()
{
  return(DoEnd());
}

private func DoEnd(bool fForced)
  {
  CastObjects(MSTB,6,35);
  CastObjects(SPRK, 15, 15);
  Sound("DeEnergize");
  var i = ContentsCount(), pCont;
  while (i--) if (pCont = Contents(i))
    if (Exit(pCont) && fForced)
      pCont->SetAction("Tumble"); // Fehlschlag bei Schild, etc. - nicht schlimm
  RemoveObject(this(), 1);
  return(true);
}

protected func Damage()
{
  // Bei zu viel Schaden Stein zerstören
  if (GetDamage() >= 37)
    DoEnd(true);
}

protected func Hit2(int xDir, int yDir)
{
  // Harter Aufschlag: Enthaltenen Lebewesen Schaden geben
  // Schaden nach Geschwindigkeit berechnen:
  // 1 Px/Frame (Clonk-Laufgeschwindigkeit) entspricht 2% Schaden für voll trainierten Clonk
  // Maximal 50% Schaden auf voll trainierte Clonks (3.2 Px/Frame - extrem schnell!)
  var iDamage = BoundBy(Distance(xDir, yDir)/50, 1, 50);
  // Schaden maximal alle 25 Frames, damit z.B. Windstoß nicht gleich tötet
  //  Wenn der letzte Schaden aber kleiner war als der jetzige, soll der
  //  Restschaden trotzdem aufaddiert werden
  var iNow = FrameCounter() + 30;
  if (iLastDamageFrame + 25 > iNow)
    if (iDamage < iLastDamageAmount)
      return();
    else
      iDamage -= iLastDamageAmount;
  else
    iLastDamageAmount = 0;
  iLastDamageAmount += iDamage;
  iLastDamageFrame = iNow;
  // Schaden an alles, was im Stein lebt
  var i = ContentsCount(), pCont;
  while (i--) if (pCont = Contents(i))
    if (GetAlive(pCont))
      // Caller von Punch ist this, damit der Controller des Steins für den Schaden verantwortlich ist!
      Punch(pCont, iDamage, true);
  // Stein nimmt ebenfalls Schaden - bei zu viel Schaden zerbricht er
  DoDamage(iDamage);
  return(true);
}

public func IsCamouflageContainer() { return true; }
