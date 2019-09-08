/*-- Drain --*/

#strict

local amount;
local drain;
local iMaterial;
protected func Initialize()
{
  SetAction("Drain");
  amount = 10;
  iMaterial = Material("Water"); 
  return(1);
}

private func Draining()
{
  if(GBackSolid()) SetPosition(GetX(), GetY()-1);
  if(GetY()<419)
  {
    ExtractMaterialAmount(0, 0, iMaterial, amount);
    Message("?", this());
  }
  else if(GetY()>421)  
  {
    var i=amount;
    while(i--) InsertMaterial(iMaterial);
    Message("!", this());
  }
  return(1);
}
