/*-- Ausrüstung --*/

#strict

// actually, only the first four are used...
static const GEAR_Armor  = 1;
static const GEAR_Shield = 2;
static const GEAR_Light = 3;
static const GEAR_Backpack = 4;
static const GEAR_Boots = 5;
static const GEAR_Helmet = 6;
// ...feel free to add some

/* Status */

public func IsHazardGear() { return(true); }

/* Buckle */
public func Activate(object pClonk)
{
  [$CtrlActivateDesc$]
  // Can the clonk use this?
  if (!(pClonk->~CanUse(GetID()))) return(0);
  // Clonk doesn't already use gear of that type 
  if (pClonk->~HasGear(this()->GetGearType())) {
    PlayerMessage(GetOwner(pClonk),"$AlreadyWears$",pClonk,GetName(pClonk));
    return(0);
  }
  // Buckle
  if(pClonk->~EquipGear(this()))
    GearBuckle(pClonk);
  return(1);
}

public func GearBuckle(object pClonk)
{
  // Nachricht für alle, die neu hier sind
  HelpMessage(pClonk->GetOwner(), "$Buckled$", pClonk, GetName());
  // verstecken aber nicht löschen
  Exit();
  SetCategory(1);
  SetPosition(1,1);
  SetVisibility(VIS_None);

  // Callback
  GearEffect(pClonk);
}

public func GearUnbuckle(object pClonk)
{
  // Nachricht für alle, die neu hier sind
  HelpMessage(pClonk->GetOwner(), "$Unbuckled$", pClonk, GetName());
  // zurück zum Hazard
  SetPosition(GetX(pClonk),GetY(pClonk));
  SetCategory(16);
  SetVisibility(VIS_All);

  GearEffectReset(pClonk);
}

/* Callbacks */

public func GearEffect(object pClonk)		{ }
public func GearEffectReset(object pClonk)	{ }
