/*-- Kopf --*/

#strict

local base;

public func IsBulletTarget() { return(true); }
public func IsAlien() { return(false); }

public func Init(object by)
{
  base = by;
  SetAction("Be");
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,0);
}

public func Death(iByPlayer)
{
  SetClrModulation(RGBa(0,0,0,255));
  base->Die();
  GameCallEx("OnAlienDeath", this, iByPlayer);
}

public func OnDmg(int iDmg, int iType) {
  if(iType == DMG_Bio) //Bioresistent.
    return(90);
  if(iType == DMG_Energy) //fast Energieresistent
    return(50);
  if(iType == DMG_Fire)
    return(-50);
}

public func OnHit(int dmg, int type, object byObj)
{
  AlienBlood(40,0,0,0,this);
  base->OnHit(dmg, type, byObj);
}

protected func CheckLiquid()	// Startcall von Swim
{
  if(!GBackLiquid(0,-3))
    return();
  if(GetMaterial(0,-3) == Material("Water"))
  {
    Death(-1);
  }
}

func Open()
{
  if(GetAction() S= "Closing")
  {
    var p = 0-GetPhase();
    SetAction("Opening");
    SetPhase(p);
  }
  if(GetAction() S= "Be")
    SetAction("Opening");
  if(GetAction() S= "DuroOpen")
    SetAction("Open");
}

func Close()
{
  if(GetAction() S= "Opening" || GetAction() S= "Opening2")
  {
    var p = 0-GetPhase();
    SetAction("Closing");
    SetPhase(p);
  }
  if(GetAction() S= "DuroOpen")
    SetAction("Closing");
  if(GetAction() S= "Open")
    SetAction("Closing");
}

func Move(int x, int y, bool override, int speed)
{
  var ef;
  if(ef = GetEffect("Move",this()))
  {
    if(override)
      RemoveEffect(0,this(), ef);
    else
      return(false);
  }
  
  AddEffect("Move",this(),100,1,this(),GetID(),x,y,speed);
  return(true);
}

private func FxMoveStart(object target, int num, bool temp, int x, int y, int speed)
{
  if(temp)
    return();

  EffectVar(0,target,num) = x;
  EffectVar(1,target,num) = y;
  EffectVar(2,target,num) = speed;
}

private func FxMoveTimer(object target, int num, int time)
{
  if(GBackSolid())
  {
    DoEnergy(-1);
    SetPosition(GetX()+RandomX(-1,1),GetY()+RandomX(-1,1));
  }

  var x,y;
  x = EffectVar(0,target,num);
  y = EffectVar(1,target,num);
  
  var d = Distance(target->GetX(),target->GetY(),x,y);
  var r = Angle(target->GetX(),target->GetY(),x,y);
  
  //Log("%d / %d",EffectVar(0,target,num),EffectVar(1,target,num));

  if(d <= 1)
  {
    return(-1);
  }
  
  if(EffectVar(2,target,num))
    if(Distance(target->GetX(),target->GetY(),x,y) < 10)
      return(-1);
  
//  if(Distance(target->GetX(),target->GetY(),base->GetX(),base->GetY()-(WORM_Part-WORM_Cutoff)*WORM_Count) > (WORM_Part-WORM_Cutoff)*WORM_Count)
//  {
//    SetPosition(GetX()-GetXDir(), GetY()-GetYDir());
//    return(-1);
//  }
  
  var speed = Max(1,EffectVar(2,target,num)*d);
  
  SetXDir(+Sin(r,speed,1),target,100);
  SetYDir(-Cos(r,speed,1),target,100);

  var x = +Sin(r,35);
  var y = -Cos(r,35);

  if(GBackSolid(x,y))
  {
    return(-1);
  }
  
  base->AdjustParts();
}

func FxMoveStop(object target, int num, int reason, bool tmp)
{
  if(tmp)
    return();
    
  SetXDir(0);
  SetYDir(0);
  
  //if(GetY() >= base->GetY()-5)
  //  SetPosition(GetX(),base->GetY()-6);
  base->FinishedMoving();
}

func Rotate(int angle, bool override, int speed)
{
  var ef;
  if(ef = GetEffect("Rotate",this()))
  {
    if(override)
      RemoveEffect(0,this(),ef);
    else
      return();
  }
  
  AddEffect("Rotate",this(),99,1,this(),GetID(),angle,speed);
}

private func FxRotateStart(object target, int num, bool temp, int r, int speed)
{
  if(temp)
    return();
  
  if(r < 0)
    r += 360;
  EffectVar(0,target,num) = r;
  EffectVar(1,target,num) = speed;
}

private func FxRotateTimer(object target, int num, int time)
{
  var angle = EffectVar(0,target,num);
  var speed = EffectVar(1,target,num);
  var r = Normalize(GetR());
  angle = Normalize(angle);

  if(r == angle)
    return(-1);

  var dif = Abs(angle - r);
  if(dif > 180) dif = 360-dif;

  var dir = BoundBy(Normalize(angle-r,-180),-Max(1,speed*dif/30),Max(1,speed*dif/30));
    
  // anpassen
  SetR(r+dir);
  
  base->AdjustParts();
}

public func NoWarp() { return(1); }
public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }
