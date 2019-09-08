/*-- Verbranntes Hauptquartier --*/

#strict

#include BAS9

private func Construction()
{
  _inherited();
  if(basement) SetPosition(GetX(basement)+1,GetY(basement),basement);
}

public func IncinerationEx() { return(Incineration()); }

public func Incineration()
{
  SetObjDrawTransform(1000,0,0,0,1000,0,this());
  return(1);
}
