/*-- Hydroclonk --*/

#strict

#include ACLK

func NoFullBreath() {
    return (GetBreath() < GetPhysical ("Breath") / 1000);
}
public func ContextHome(pCaller)
{
  [$CtxHomeDesc$|Image=TRTS|Condition=NoFullBreath]
  SetCommand (this(), "Enter", FindObject (0, 0,0,-1,-1, OCF_Entrance));
  // Hier noch Code, um auch "Luftlöcher" zu nutzen
  return(1);
}

protected func ControlSpecial() 
{ 
  [$CtrlInventoryDesc$|Image=CXIV]
  // Inventory verschieben 
  return(ShiftContents(0,0,0,1) ); 
}

/* Damit der Clonk auch Zeugs ausm Wasser holt... */

func CheckObjectAvailability (obj)
{
    // Object is near enough
    if (!Inside (GetX () - GetX (obj), -500, +500)) return ();
    if (!Inside (GetY () - GetY (obj), -250, +250)) return ();
    // Object is not connected to a pipe (for line construction kits)
    if (FindObject (SPIP, 0, 0, 0, 0, 0, "Connect", obj)) return ();
    if (FindObject (DPIP, 0, 0, 0, 0, 0, "Connect", obj)) return ();
    // Must be complete
    if (~GetOCF (obj) & OCF_Fullcon) return ();
    // Doesn't burn
    if (OnFire (obj)) return ();
    // Go!
    return (obj);
}

func GetAvailableObject (def, xobj)
{
  var obj;
  // Next closest normal available or in liquid
  while (obj = FindObject (def, 0, 0, -1, -1, OCF_Available | OCF_InLiquid, 0, 0, 0, obj))
  {
    if (ObjectDistance (obj) > 559) { obj = 0; break; }
    if (!CheckObjectAvailability (obj)) continue;
    // Not contained in xobj
    if (!(Contained (obj) == xobj) || !xobj)
      // We found one
      break;
  }
  if (obj) return (obj);
  // Next closest in container (because containers are sunk here, nothing in them is "available"...)
  while (obj = FindObject (def, 0, 0, -1, -1, OCF_Fullcon, 0, 0, AnyContainer(), obj))
  //while (obj = FindObject (def, 0, 0, -1, -1, OCF_Fullcon(), 0, 0, 0, obj))
  {
    if (ObjectDistance (obj) > 559) { obj = 0; break; }
    if (!CheckObjectAvailability (obj)) continue;
    // Not contained in xobj
    if (Contained (obj) == xobj) continue;
    // The Container must be accessible (Same as for OCF_Available)
    if (GetDefCoreVal ("GrabPutGet", 0, GetID (Contained(obj))) & C4D_GrabGet || GetOCF (Contained(obj)) & OCF_Entrance)
    // We found one
      break;
  }
  return (obj);
}

public func ControlCommandAcquire(target, x, y, target2, def)
{
  //Falls das Teil rumliegt nur aufsammeln
  var obj = GetAvailableObject (def, target2);
  if (obj) return (1, AddCommand (this(), "Get", obj,0,0,0,40));
  return (inherited (target,x,y,target2,def));
}

/* Itemlimit */
public func MaxContentsCount() { return(3); }

/* Status */

public func IsHydroClonk() { return true; }
