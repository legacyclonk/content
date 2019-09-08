/*-- Verbranntes Waffenlager --*/

#strict

#include BAS9

local noExplode;

private func Construction()
{
  _inherited();
  if(basement) SetPosition(GetX(basement)+1,GetY(basement),basement);
}

protected func Initialize()
{
  // Kein Explodieren wenn einfach so platziert
  noExplode = 1;
}

protected func Incineration()
{
  SetObjDrawTransform(1000,0,0,0,1000,0,this());
  if(noExplode) return();
  var i, iObj;
  // Fässer fliegen lassen
  for(i = 0; i < 3; i++) {
    iObj = CreateObject(WDBR,0,0,-1);
    // Fässer nicht geladen o.ä.
    if(!iObj) break;
    Incinerate(iObj);
    SetSpeed(RandomX(-50,50),-35-Random(30),iObj);
  }
  // Kugeln
  for(i = 0; i < 5; i++) {
    iObj = CreateObject(CART, 0, 0, -1);
    // Kugel nicht geladen o.ä.
    if(!iObj) break;
    iObj->~Launch(-1,7+Random(6));
    SetSpeed(RandomX(-100,100),RandomX(-100,10),iObj);
  }
  // Geräusch
  Sound("Blast2");
  // Nicht nochmal
  noExplode = 1;
}

public func IncinerationEx()
{
  SetObjDrawTransform(1000,0,0,0,1000,0,this());
  return(1);
}