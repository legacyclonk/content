/*-- Bombe (Zapper kann nicht coden) --*/

#strict

local startX,startY;
local pLight;
local timer, holder;

public func RejectShift()
{
	return true;
}

public func IsAlwaysCollectable()
{
	return true;
}

// Timer zurückliefern
public func GetBombTimer()
{
  return(timer);
}

public func IsAtHome() {
	if(Abs(GetX()-startX) < 20)
		if(Abs(GetY()-startY) < 20)
			if(GetOwner() == -1)
				return(true);
	return(false);
}

public func Init(iX,iY)
{
	startX=iX;
	startY=iY;
	Exit();
	SetPosition(startX,startY);
	SetOwner(NO_OWNER);
	holder = 0;
	AddEffect("BombFlight",this(),20,1,this(),GetID());
	// eventuelle Bombeneffekte entfernen
	var effect = GetEffect("BombTimer",this());
	if(effect)
		RemoveEffect(0,this(),effect);
}

// Anzeige in der Hand
public func IsDrawable() { return(true); }
public func HandSize() { return(1050); }
public func HandX() { return(1000); }
public func HandY() { return(1000); }

public func IsBouncy() { return(true); }
public func CanAim() { return(1); }

public func ControlThrow() {
	// bereit zum Schießen
	if(!(Contained()->ReadyToFire())) return(false); 

	LaunchBomb(6,Contained()->~AimAngle(0));
	return(true);
}

// verschießt die Bombe
// speed: Geschwindigkeit
// angle: Winkel
public func LaunchBomb(int speed, int angle) {
	var user = Contained();
	var dir = GetDir(user)*2-1;

	// Adjust angle
	angle = BoundBy(angle+GetYDir(user)*dir,-360,360);
	// calculate speed
	var xdir = Sin(angle,speed);
	var ydir = -Cos(angle,speed);

	var x,y;
	user->WeaponEnd(x,y);

  // effect
  Sound("BombShot");

	for(var cnt=0;cnt<5;cnt++)
	  CreateParticle("Thrust",x,y,GetXDir(user)+RandomX(-2,2),GetYDir(user)+RandomX(-2,2),100,RGBa(10,10,255,0),0,0);

	for(var i=0; i<0; ++i) {
		CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
						GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
						RandomX(80,140),RGBa(100,100,255,0),0,0);
	}

  var no = AddEffect("CollectionException", this(), 1, 5);
  EffectVar(0, this(), no) = Contained();
	Exit(this(), x+xdir/10, y+ydir/10,xdir,xdir, ydir,0);
}

protected func Destruction()
{
	var pBomb=CreateObject(GetID(),0,0,-1);
	pBomb->Init(startX,startY);
	GameCallEx("BombReturned");
	Log("$Returned$");
	Sound("BombReset",1);
}

protected func Departure() {
	GameCallEx("BombLoose");
}

protected func ReturnToSpawn()
{
	var eff;
	while(eff = GetEffect("BombFlight",this()))
		RemoveEffect(0, this(),eff);
	SetSpeed();
	Init(startX, startY);
	GameCallEx("BombScored");
}

protected func Entrance(pTo)
{
  // Wer sie sammelt, besitzt. Quasi.
	if(GetOwner(pTo) != NO_OWNER)
		SetOwner(GetOwner(pTo));

	var eff;
	while(eff = GetEffect("BombFlight",this()))
		RemoveEffect(0, this(),eff);
	
  // Timer starten
  if(!(GetOCF(pTo) & OCF_CrewMember)) return(1);
  if(pTo == holder)
    AddEffect("BombTimer", this(), 1, 1, this(),0, pTo, timer);
  else
    AddEffect("BombTimer", this(), 1, 1, this(),0, pTo, 1000);
	
  GameCallEx("BombTaken");
}

protected func Hit(xDir,yDir)
{
	if(GBackSolid(-6,0) || GBackSolid(6,0) || GBackSolid(-2,4) || GBackSolid(2,4)) SetXDir(-(xDir/20)); 
	if(GBackSolid(-4,3) || GBackSolid(4,3) || GBackSolid(0,6) || GBackSolid(0,-6)) SetYDir(-(yDir/20));
	return(_inherited());
}

/*---- Effekt für den Flug der Bombe ----*/

protected func FxBombFlightStart(pTarget)
{
	// Licht erstellen
	if(!pLight) pLight=AddLight(200,HSL(0,255,255),pTarget);
	return(true);
}

protected func FxBombFlightTimer(pTarget,iEffectNumber,iEffectTime)
{
  // Licht effektiern
	if(pLight)
		pLight->ChangeColor(HSL(BoundBy(Abs(Sin(iEffectTime*2,120)),0,120),128,200));
	if(!Contained())
	{
		if((GetOwner() != NO_OWNER)) // Irgendwer hat sie irgendwo hingeschmissen.
		{
			// Zeit
			if((++EffectVar(0, pTarget, iEffectNumber)) > 32*30)
			{
				pTarget -> Explode(25); // Draußen-Zeit erhöhen, schauen ob zu lang, evntl. bumm
				return();
			}
			// Effekte
			if( (36*30 % (36*30 - EffectVar(0, pTarget, iEffectNumber))) == 0)
			{
				CreateParticle("PSpark", 0, 0, 0, 0, 20, RGB(255,0,0), pTarget);
				Sound("Bip", 0, pTarget, 20);
			}
		}
		if(!pLight)
		{
			pLight=AddLight(200,HSL(0,255,255),pTarget);
			pLight->SetPosition(pTarget->GetX(), pTarget->GetY());
		}
	}
  else
	{
		if(EffectVar(0, pTarget, iEffectNumber)) // Nicht draußen? Zeit zurücksetzen!
		  EffectVar(0, pTarget, iEffectNumber) = 0;
		if(pLight) pLight->RemoveObject();
	}

	return(true); 
}

protected func FxBombFlightStop()
{
	if(pLight) RemoveObject(pLight); // Das Licht wegmachen.
}

/*---- Effekt für den Timer der Bombe ----*/

protected func FxBombTimerStart(object pTarget, int iNo, int iTemp, object pHolder, int iTimer)
{
  if(iTemp) return();
  if(!Contained()) return(-1);
  holder = pHolder;
  timer = iTimer;
}

protected func FxBombTimerTimer()
{
  if(!Contained()) return(-1);
  timer--;
  if(!timer) return(Explode(25));
  var angle = 90 * timer / 1000;
  PlayerMessage(Contained()->GetOwner(), "<c %x>%d</c>", Contained(), RGB(Cos(angle, 255), Sin(angle, 255)), timer);
}

public func AI_Inventory(object clonk)
{
  if(clonk->Contents() != this) return false;

  // nächstes, feindliches Tor suchen
  var nextding, distance = -1;
  for(var pGate in FindObjects(Find_ID(GBRG)))
  {
    if(pGate->GetGoalTeam() == GetPlayerTeam(clonk->GetOwner())) continue;
    if(distance == -1)
    {
      nextding = pGate;
      distance = ObjectDistance(pGate);
    }
    else
    {
      if(ObjectDistance(pGate) < distance)
      {
        nextding = pGate;
        distance = ObjectDistance(pGate);
      }
    }
  }
  // Tor ist zu weit weg?
  if(distance > 200)
  {
    distance = -1;
    nextding = false;
  }
  // Freunde suchen, wenn der Timer abläuft
  if(!nextding && GetBombTimer() <= 100)
  {
    for(var pFriend in FindObjects(Find_OCF(OCF_CrewMember), Find_Allied(clonk->GetOwner()), Find_Exclude(clonk)))
    {
      if(distance == -1)
      {
        nextding = pFriend;
        distance = ObjectDistance(pFriend);
      }
      else
      {
        if(ObjectDistance(pFriend) < distance)
        {
          nextding = pFriend;
          distance = ObjectDistance(pFriend);
        }
      }
    }
  }
  // Ziel?
  if(nextding)
  {

    // Ziel ist in Reichweite?
    if(distance <= 200 && PathFree(GetX(), GetY(), nextding->GetX(), nextding->GetY()))
    {
      // Winkel bestimmen
      var angle = Angle(GetX(), GetY(), nextding->GetX(), nextding->GetY());

      // Winkel stimmt!
      if(Inside(angle, 70, 110) || Inside(angle, 250, 290))
      {
        if(clonk->ReadyToFire())
        {
          if(GetX() < nextding->GetX())
            clonk->SetDir(DIR_Right);
          else
            clonk->SetDir(DIR_Left);
          // Feuer frei!
          ControlThrow();
        }
      }
      else
      {
        // Zielen beginnen
        if(!clonk->IsAiming())
        {
          if(clonk->ReadyToSquatAim())
            clonk->StartSquatAiming();
        }
        else
        {
          clonk->DoMouseAiming(nextding->GetX(), nextding->GetY());
        }
      }
    }
  }
  
  return true;
}
