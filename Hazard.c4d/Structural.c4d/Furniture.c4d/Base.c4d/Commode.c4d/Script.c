/*-- Commode --*/

#strict

local iAction;

func Initialize() 
{
  SetAction( Format("Exist%d", iAction = Random(2) + 1)); 
  SetDir(Random(2));
}

public func OnHit(int iDmg, int iType, object pFrom) {
  if(GetDamage() > MaxDamage())
  {
    SetAction( Format("Broken%d", iAction));
    return(true);
  }
}

public func OnDmg(int iDmg, int iType)
{
  if( iType == DMG_Explosion)
    return(0);
  else
    return(100);
}

public func MaxDamage() { return(30); }

public func CanBeRecycled() { return(2); }

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetAction(\"%s\")",GetAction());
	extra[GetLength(extra)] = Format("SetDir(%d)",GetDir());
}
