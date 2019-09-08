/*-- Hauptquartier --*/

#strict

#include BAS9
#include DOOR

private func Construction()
{
  _inherited();
  if(basement) SetPosition(GetX(basement)+1,GetY(basement),basement);
}

private func Initialize()
{
  SetShape(-50,-10,101,42);
  SetObjDrawTransform(1000,0,0,0,1000,-1000*11,this());
  return(1);
}