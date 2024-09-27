/* ----------------------------- Support Drone ----------------------------- */
// Newton: komplett neu gescriptet für Hazard 2.1

#strict 2

local guardedClonk;				// the clonk to follow. Can be null if the drone should be autonomous
local usedWeapon;					// weapon with which the drone is equipped
local currentTarget;			// current enemy aimed at
local currentAim;					// current aim angle
local targetAim;          // angle at which the drone wants to shoot
local targetX, targetY;		// target position where to go
local previousXDir;				// previous X speed. Used for thruster effect
local isFollowing;				// whether the drone follows the clonk
local isAutonomous;				// whether the done has a guardedClonk at all.
local ammobag;						// ammunition belt. Usually references to the clonks ammobag. Has an own ammobag if autonomous

func Initialize()
{
	AddDamageEffect(this,30);
  AddEffect("ShowWeapon", this, 1, 1, this, GetID());
  SetGraphics(0, this, GetID(), 3, 5, 0, 0, this); // für ShowWeapon
  currentAim = targetAim = 160;
  SetAction("Idle");
}

/** Heftet die Drone mit angegebener Waffe an den Clonk */
public func ArmToClonk(object clonk, object weapon)
{
  if(!clonk || !weapon) return;
  guardedClonk = clonk;
	isFollowing = true;
  GetArmed(0,weapon);
}

/** Arm with weapon, without protecting a clonk. The drone will use its own
    ammo storage then. If it is empty, it is empty. */
public func Arm(object weapon)
{
  if(!weapon) return false;
	if(!isFollowing) isAutonomous = true;
	weapon->Exit();
  weapon->Enter(this);
  SetCategory(C4D_Vehicle);
	SetAction("Fly");
	usedWeapon = weapon;
  AddEffect("Guard", this(), 1, 2, this, GetID());
	return true;
}

/** Switch following the clonk (if any) on or off */
public func FollowClonk(bool doFollow)
{
	isFollowing = doFollow;
}

/** Set position at which the drone should go. While following a clonk, this 
    value will be re-set every few frames. */
public func SetGuardPosition(int x, int y)
{
	targetX = x;
	targetY = y;
}

/** Liefert den beschützten Clonk zurück */
public func GetGuardedClonk() { return(guardedClonk); }


/* -------- callbacks für den ShowWeapon Effekt -------- */

public func WeaponAt(&x, &y, &r) {
  x = 0;
  y = 8000;
  r = 270-AimAngle();
  return true;
}

public func WeaponEnd(&x, &y) {
  var effectNumber = GetEffect("ShowWeapon",this);
  if(!effectNumber) return;
  x = EffectVar(4, this, effectNumber)/1000; // X-Ende des Laufs der Waffe
  y = EffectVar(5, this, effectNumber)/1000; // Y-Ende des Laufs der Waffe
}

public func WeaponBegin(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number) return;
  x = EffectVar(2, this(), number)/1000;
  y = EffectVar(3, this(), number)/1000;
}

public func AimAngle(int maxAngle, int range, bool mode)
{
  return(BoundBy(currentAim, 90, 270));
}

/* ------------- Schaden ------------ */

public func OnDmg(int damage, int type)
{
  if(type == DMG_Melee)      return -30;
  if(type == DMG_Projectile) return +20;
  if(type == DMG_Fire)       return +30;
  if(type == DMG_Explosion)  return -10;
  if(type == DMG_Energy)     return -40;
  if(type == DMG_Bio)        return +50;
}

public func Damage() {
	if(GetDamage() < MaxDamage()) return;

  TurnOff();
  Explode(20);
}

/** Benutzen der Drone */
public func Activate(object clonk)
{
  if(!(clonk->~CanUse(GetID()))) return true;
 
  // nur eine Drone pro Clonk
  var pDrone; 
  for(pDrone in FindObjects( Find_Func("IsDrone"), Find_Action("Fly"), Find_Owner(clonk->GetOwner())))
  {
    if(pDrone->GetGuardedClonk() != clonk) continue;
    PlayerMessage(clonk->GetOwner(), "$JustOneDrone$", clonk);
    Sound("Error");
    return true;
  }
  
  if(EMPShocked())
  {
    PlayerMessage(clonk->GetOwner(), "$Damaged$", clonk);
    Sound("Error");
    return true;
  }
  
  guardedClonk = clonk;
	isFollowing = true;
  
  CreateMenu(DRSU, clonk, this,0,"$NoWeap$");
  for(var i = 0; i < clonk->ContentsCount(); ++i)
  {
    var weapon = clonk->Contents(i);
    if(weapon->~IsWeapon())
      AddMenuItem("$ArmWith$", "GetArmed", weapon->GetID(), clonk, 0, weapon);
  } 
  
  return true;
}

protected func GetArmed(id item, object weapon)
{
  if(!Arm(weapon)) return false;
  HelpMessage(guardedClonk->GetOwner(),"$ArmedWith$",guardedClonk,GetName(weapon));
  return true;
}

public func FxGuardStart(object target, int effectNumber, int temp)
{
  if(temp) return 0;
  Exit();
	
  //Schauen, ob Munition in der Waffe ist. Sonst nachladen.
  var ammoid = usedWeapon->GetFMData(FM_AmmoID);
  var ammoload = usedWeapon->GetFMData(FM_AmmoLoad);
  
  if(!CheckAmmo(ammoid, ammoload, usedWeapon))
    usedWeapon->Reload();

	if(guardedClonk)
	{
		SetController(guardedClonk->GetController());
		SetOwner(guardedClonk->GetOwner());
	  targetX = guardedClonk->GetX();
		targetY = guardedClonk->GetY();
	}
	else
	{
	  targetX = GetX();
		targetY = GetY();
	}
}

public func FxGuardStop(object target, int effectNumber)
{ 
  SetAction("Idle");
  SetCategory(C4D_Object);
  
  if(usedWeapon)
  {
      usedWeapon->Exit();
  }
	usedWeapon = guardedClonk = currentTarget = 0;
	targetX = targetY = previousXDir = 0;
	isAutonomous = isFollowing = 0;
	if(ammobag) RemoveObject(ammobag);
	currentAim = targetAim = 160;
}

public func FxGuardTimer(object target, int effectNumber, int time)
{ 
  if(!usedWeapon) return -1;

  var xDiffDir = GetXDir()-previousXDir;
  var thrustStrength = Min(Abs(xDiffDir)*50,200);
  var thrustX = xDiffDir/Abs(xDiffDir);
  if(thrustX)
    CreateParticle("MuzzleFlash2",-10*thrustX,-3,-thrustX*500,0,thrustStrength,RGBa(64,128,255,0),this, true);

  CreateParticle("PSpark", RandomX(-3,3), 5, 0, RandomX(2,5), 220+Random(70), RGBa(128,196,255,160), this, true);
  
  previousXDir = GetXDir();
  
	// adjust aim angle
	var diffAngle = Normalize(targetAim - currentAim, -180);
	var rotationSpeed = 4;
	if(diffAngle > 0) currentAim += Min(diffAngle,rotationSpeed);
	if(diffAngle < 0) currentAim += Max(diffAngle,-rotationSpeed);
	
  // other stuff only every 6th frame
  if(time % 6) return 0;
    
  if(usedWeapon->IsReloading() && time % 30 == 0)
    CreateParticle("PSpark", 0, 0, 0, 0, 40, RGB(255,0,0), this);
  
  //Lebt der Clonk noch? Oder noch da? :o
  if(!isAutonomous && (!guardedClonk || !guardedClonk->GetAlive()))
  {
    TurnOff();
    return -1;
  }
  	
	if(isFollowing && guardedClonk)
	{
	  targetX = guardedClonk->GetX();
		targetY = guardedClonk->GetY();
	}
		
  // Wenn Abstand zu gross müssen wir uns zum Beschützendem beamen
  if(Distance(GetX(),GetY(), targetX, targetY) > GetBeamToClonkDistance())
  {
     currentTarget = 0;
     Beam(this, 0, targetX, targetY);
     return 0;
  }
  
  // Ziel suchen
  currentTarget = AcquireTarget();
  
  if(currentTarget)
  {
		targetAim = Angle(GetX(), GetY(), currentTarget->GetX(), currentTarget->GetY());
		var halfAimDelta = Abs(AngleDiff(targetAim,currentAim))/2;
		var targetDistance = Distance(GetX(), GetY(), currentTarget->GetX(), currentTarget->GetY());
		var targetSize = Sqrt(Pow(currentTarget->GetDefHeight(),2) + Pow(currentTarget->GetDefWidth(),2));
		
		if(1000 * targetSize/2 >= targetDistance * Sin(halfAimDelta, 1000))
		{
			if(!usedWeapon->IsShooting()) usedWeapon->~ControlThrow(this);
		}
  }
  else
  {
    if(usedWeapon->IsShooting()) usedWeapon->StopAutoFire();
  }
  
  // Drone versucht wenn möglich über dem Clonk zu schweben
  var x = targetX;
  var y = targetY;
  var yoffset = GetFlightHeight();
  for(var i = 0 ; i < yoffset; ++i)
  {
    if(GBackSolid(AbsX(x),AbsY(y-i))) break;
  }
  y = y-i+25;
  
  if(!GetCommand(0) || time % 60 == 0)
  {
    SetCommand(0, "MoveTo", 0, x, y);
  }
}

private func WayFree(object enemy)
{
  //Schauen, ob das Schussfeld frei (von befreundeten Einheiten) ist.
  if(FindObject2(
    Find_OnLine(0,0,AbsX(enemy->GetX()),AbsY(enemy->GetY())),
    Find_OCF( OCF_CrewMember),
    Find_Not(Find_Hostile(GetOwner()))))
      return false;
  
  return true;
}

private func AcquireTarget()
{
  var range = Min(GetMaxTargetDistance(),usedWeapon->GetBotData(BOT_Range));
  var enemy;
  for(enemy in FindTargets(this, range, 90, 180))
  {
    if(enemy == guardedClonk) continue;
    if(WayFree(enemy))
    {

      return enemy;
    }
  }
}

public func TurnOff()
{
  if(GetEffect("Guard", this)) RemoveEffect("Guard", this);
}

public func AmmoStoring()
{
	if(guardedClonk) return guardedClonk->~AmmoStoring();
	if(!ammobag)
	{
		ammobag = CreateObject(ABAG);
  }
  return ammobag;
}

public func Destruction()
{
	if(ammobag) RemoveObject(ammobag);
}

public func EMPShock()
{
  TurnOff();
  EMPShockEffect(15*35);
  return true;
}

public func RejectEntrance()
{
  return !!GetEffect("Guard", this);
}

public func ReadyToFire()
{
  return !!GetEffect("Guard", this);
}

public func IsBulletTarget()
{
  return !!GetEffect("Guard", this);
}

public func IsAiming() { return true; } // can use weapons which have to aim
public func MaxDamage() { return 80; }
public func IsDrone() { return true; }
public func IsEquipment() { return true; }
public func IsMachine() { return true; }
public func IgnoreFriendlyFire() { return true; }

public func GetArenaRemoveTime() { return 300; }
public func GetBeamToClonkDistance() { return 400; }
public func GetMaxTargetDistance() { return 250; }
public func GetFlightHeight() { return 80; }

// KI-Behandlung
public func AI_Inventory(object clonk)
{

  //Checken, ob der Clonk nicht schon eine hat. :Ooo
  var drone;
  
  // Kann Ding nicht benutzen...
  if(!(clonk->~CanUse(GetID()))) return false;
  // Nur eine
  for(var drone in FindObjects( Find_Func("IsDrone"), Find_Action("Fly")))
    if(clonk->GetOwner() == drone->GetOwner())
      if(drone->GetGuardedClonk() == clonk)
        return false;
  // Nicht, wenn kaputt
  if(EMPShocked()) return false;
  // Drohne ist zulässig. Nur einsetzen, wenn der Clonk mehr als 1 Waffe hat
  if(clonk->CustomContentsCount("IsWeapon") < 2) return true;
  // Waffen für Drohne ok? (FM_Aim und so)
  var wpn = clonk->GetRandomDroneWeapon();
  if(!wpn) return true;
  // Waffe nehmen (verzögert)
  ScheduleCall(this, "ArmToClonk", 1, 0, clonk, wpn);
  return true;
}
