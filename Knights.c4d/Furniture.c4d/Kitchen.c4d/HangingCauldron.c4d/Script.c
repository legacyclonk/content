/*-- Hängender Kochtopf --*/

#strict

/* TimerCall */

private func CheckContents()
{
  var stove = FindObject(STOV, -50, 0, 100, 100, 0, "Burning");
  if(!stove) return(0);

  var i, obj;
  while(obj = Contents(i ++) )
    if(obj->~Cook() )
      Sound("Cook");
}

/* Aufnahme */

protected func Collection()
{
  Sound("Clonk");
}
  
/* Aufschlag */  
  
protected func Hit()
{
  Sound("ClonkHit*");
}

/* CastleCheck */

public func CastleChange()
{
  if(!FindObject(CPB3,1)) return(1);
  while(Contents())
    Exit(Contents());
  Split2Components();
}

