/*-- Nordlicht --*/

#strict

local iSpeed;

protected func Initialize() {
  SetAction(Format("Glow%d",Random(4)));
  DoCon(Random(80)*(-Random(3)));
  iSpeed=Random(7)-3;
}

private func Moving() {
  SetPosition(GetX()+iSpeed,GetY());
  if (GBackSemiSolid(-GetCon(),+30) || GBackSemiSolid(GetCon(),+30))
    Remove();
}

private func Remove() { RemoveObject(); }