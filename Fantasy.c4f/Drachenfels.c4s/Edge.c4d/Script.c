/*-- Ecke --*/

#strict

protected func Initialize() {
  Snap();
  return(1);
}

public func Snap(int o,int p)
  {
  SetPosition((GetX()+5)/10*10-5+o, (GetY()+5)/10*10-5+p);
  }

public func SetP(int p)
  {
  SetAction("Edge"); SetPhase(p);
  SetSolidMask(p*10,0,10,10);
  }
  