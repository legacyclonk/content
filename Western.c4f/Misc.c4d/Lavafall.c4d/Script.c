/*-- Lavafall --*/

#strict

local amount;
local drain;
protected func Initialize()
{
  SetAction("Drain");
  amount = 2;
  return(1);
}

private func Draining()
{
  if(GBackSolid()) SetPosition(GetX(), GetY()-1);   
  var i;
  var mat;
  mat = Material("DuroLava");
  if(!drain)
  {
    i=amount;
    while(i--) InsertMaterial(mat);
    return();
  }
  //mat = GetMaterial(GetX(drain)-GetX(), GetX(drain)-GetX());
  i = drain->ExtractMaterialAmount(0, 0, drain->GetMaterial(), amount);
  Message("%d %s", this(), i, MaterialName(mat));
  while(i--) InsertMaterial(mat);
  return(1);
}
