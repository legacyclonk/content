/*-- Aquacit --*/

#strict

local prey;

func IsAlien() { return(true); }
func IsBulletTarget() { return(true); }

func IsRecovering() { return(GetEffect("Recovering",this())); }
func IsSwimming()   { return(GetAction() S= "Swim" || GetAction() S= "Turn"); }
func IsAttacking()  { return(GetEffect("KameHameHa",this())); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Swim");
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,0);
}

/* TimerCall mit KI-Steuerung */

private func Activity() 
{
  if (GetAction() S= "Walk") WalkDir();
    
	if(prey)
		return(Attack(prey));
	else {
	  for(var p in FindTargets(this,150)) {
		prey = p;
		break;
	  }
	  
	}


  // Schwimmverhalten
  if (!GBackLiquid(0, -8) && GetAction() ne "Walk") return(SetComDir(COMD_Down()));
  if (Random(2)) return(1);
  if (GetAction()ne"Swim") return(1);
  if (!Random(5)) SetComDir(COMD_Up());

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

private func WalkDir()
{
  SetComDir(COMD_Left());
  if (Random(2)) SetComDir(COMD_Right());
  return(1);
}

/* Kontakte */

protected func ContactLeft()
{
  return(TurnRight());
}

protected func ContactRight()
{
  return(TurnLeft());
}

protected func ContactTop()
{
  SetComDir(COMD_Down());
  return(1);
}

protected func ContactBottom()
{
  if (GetAction() ne "Walk") SetComDir(COMD_Up());
  if (Random(10)) SetComDir(COMD_Right());
  if (Random(10)) SetComDir(COMD_Left());
  return(1);
}

/* Aktionen */

func Attack(object target)
{
  if(IsAttacking())
    return();

  if(target != prey)
    prey = target;

  if(ObjectDistance(target) > 40 && ObjectDistance(target) < 100)
    if(KameHameHa())
      return();
  
  if(ObjectDistance(target) < 10)
    Bite();
  
  if(!GetCommand())
    SetCommand(this(),"Follow",target);
}

func KameHameHa()
{
  // Attacke erst ausgeführt
  if(IsRecovering())
    return();
  // Schwimmt nicht!
  if(!IsSwimming())
    return();
  // Sicht frei?
  if(!PathFree(GetX(),GetY(),GetX(prey),GetY(prey)))
    return();
  // Kein Wasser am Ende?
  if(!GBackLiquid(prey->GetX()-GetX(), prey->GetY()-GetY()))
    return();
    
  AddEffect("KameHameHa",this(),150,1,this(),ALN6);
  
  return(true);
}

func FxKameHameHaStart(object target, int num, bool temp)
{
  if(temp)
    return();
}

func FxKameHameHaTimer(object target, int num)
{  
  var angle = Angle(GetX(),GetY(),prey->GetX(),prey->GetY());
  
  // Kamehameha!
  SetGraphics(0,0,GetID(),1,6);
  SetObjectBlitMode(1,0,1);
  
  var dist,x,y;
  for(dist = 0; dist < 100; dist++)
  {
    x = Sin(angle, dist);
    y = -Cos(angle,dist);
    if(GBackSolid(x, y))
      break;
  }
  
  // größenversetzte Partikellinie!
  var step = Max(dist/20,1);
  for(var i; i < 20; i++)
  {
    CreateParticle("PSpark",Sin(angle,step*i),-Cos(angle,step*i),0,0,i*4+20,RGB(200,200,255));
  }
  
  SetPosition(GetX()+x, GetY()+y);
  
  for(var victim in FindTargets(this,150))
    target->DoDmg(10,DMG_Energy,victim);
  
  return(-1);
}

func FxKameHameHaStop(object target, int num, bool temp)
{
  if(temp)
    return();
  
  Recover(200);
  AddEffect("UnGlow",this(),111,1,this(),ALN6);
}

func FxUnGlowTimer(object obj, int num)
{
  SetClrModulation(RGBa(255,255,255,EffectVar(0,obj,num)),0,1);
  EffectVar(0,obj,num) += 10;
  if(EffectVar(0,obj,num) > 255)
  {
    SetGraphics(0,0,0,1);
    return(-1);
  }
}

func Bite()
{
  // Nur im Wasser
  if(!IsSwimming())
    return();
  
  Sound("JumperBite");
  DoDmg(4, DMG_Melee,prey);
  DoDmg(4, DMG_Bio,prey);
  Recover(30);
}

private func TurnRight()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right());
  return(1);
}

private func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left());
  return(1);
}

protected func Death(int iKilledBy)
{
  //Splooortsch
  Sparks(10, RGBa(25, 155, 185, 20));
  GameCallEx("OnAlienDeath", this, iKilledBy);
  RemoveObject();
}

func Recover(int time)
{
  AddEffect("Recovering",this(),1,time);
}

/* Schaden */

public func OnDmg(int iDmg, int iType) {
  //Melee
  if(iType == DMG_Melee)
    return(0);
  //Projectile
  if(iType == DMG_Projectile)
    return(10);
  //Flame
  if(iType == DMG_Fire) {
    return(-100);
  }
  //Explosion
  if(iType == DMG_Explosion)
    return(-50);
  //Energy
  if(iType == DMG_Energy)
    return(90);
  //Bio
  if(iType == DMG_Bio) {
    return(80);
  }
}

public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }