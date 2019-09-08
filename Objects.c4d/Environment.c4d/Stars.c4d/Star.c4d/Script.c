/*-- Stern --*/

#strict

local iBrightness;

// Local(0) und Local(1) werden für die Parallax-Steuerung verwendet...

protected func Initialize()
{
  iBrightness = Random(10) + 1;
  SetAction("Appear");
  MakeBackground();
  return(1);
}

private func Appearing()
{
  if (GetPhase() == iBrightness)
  {
    SetAction("Shine");
    SetPhase(iBrightness - 1);
  }
  return(0);
}
  
/* Versetzt den Stern in den Hintergrund */

private func MakeBackground()
{
  SetCategory(GetCategory() | C4D_Parallax() | C4D_Background());
  Local(0) = Local(1) = Random(20) + iBrightness * 5 - 5;
  if (Random(3)) SetClrModulation(RGB(Random(100) + 156, Random(100) + 156, Random(100) + 156));
}

/* Versetzt OldGfx-Sterne beim Laden eines Spielstands in den Hintergrund */
  
protected func UpdateTransferZone()
{
  if (~GetCategory() & C4D_Parallax()) 
    MakeBackground();
}
