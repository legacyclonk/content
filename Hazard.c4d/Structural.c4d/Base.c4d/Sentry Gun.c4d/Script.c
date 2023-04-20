/*-- Sentry Gun --*/

#strict

local cur_Attachment;
local aim_angle;
local target_angle;
local Shooting;
local iPat_Dir;
local Active;
local GotTarget;

/* Aufrufe */

public func TurnOn()
{
  Active = true;
}

public func TurnOff()
{
  Active = false;
}

public func Arm(id idWeapon)
{
  // Crash-Vermeidung
  if(!idWeapon) return();
  if(!GetName(0, idWeapon)) return();

  //Ausrüsten mit idWeapon
  var pWeapon = CreateObject(idWeapon, 0, 0, GetOwner());
  Enter(this(),pWeapon);
  //Ordnern
  SetObjectOrder(this(), pWeapon, 1);
  aim_angle = 180;
  iPat_Dir = -1+ Random(2)*2;
  cur_Attachment = pWeapon;
  LocalN("controller", pWeapon) = this();
  Reload();
}

/* Script */

public func Initialize() 
{
  //Ausrichten
  FitToTop();
  AddEffect("ShowWeapon",this(),1,1,this(),GetID());
  //lieber Overlay 3 nehmen. Falls wir mal 2-overlayige waffen einbaun :]
  SetGraphics(0,this(),GetID(),3,5,0,0,this());
}

public func WeaponAt(&x, &y, &r) {
  x = 0;
  y = -3000;
  r = aim_angle+270;
  return(1);
}

public func WeaponBegin(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(2, this(), number)/1000;
  y = EffectVar(3, this(), number)/1000;
}

public func WeaponEnd(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(4, this(), number)/1000;
  y = EffectVar(5, this(), number)/1000;
}

public func GetWeaponR() {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  return(EffectVar(1, this(), number));
}

public func FitToTop()
{
  //Das Gestell passt sich selbst mal etwas besser zur Decke an.
  var x = GetX(), y = GetY() - GetObjHeight() / 2;
  
	SetPosition(GetX(),GetY()+GetObjHeight()/2);
	var i = 0;
	while(++i < GetObjHeight() && !Stuck()) SetPosition(GetX(),GetY()-1);
	SetPosition(GetX(),GetY()-1);
	Message("@%d",this,i);

}

public func Activity()
{
  var iHeight, iWidth, iAngle;
  //Wuah, haben wir eine Waffe?
  if(! GetAttWeapon()) return();
  //Sind wir im Eimer?
  if( EMPShocked()) return();
  //Sind wir aktiv?
  if(! Active) return();
  //Wenn nicht schon gesetzt: Turn-Action
  if(GetAction() ne "Turn")
    SetAction("Turn");

  /*Patroullie fahren*/
  
  // alle 5 Frames
  if(!(GetActTime()%5)) {
	  //Zu weit links?
	  if( aim_angle <= MaxRotLeft() )
	  {
	    //Wir fahren zurück
	    iPat_Dir = 1;
	    target_angle = MaxRotRight();
	    GotTarget = 0;
	  }
	  //Oder zu weit rechts?
	  else if( aim_angle >= MaxRotRight() )
	  {
	    //Hinfahren
	    iPat_Dir = -1;
	    target_angle = MaxRotLeft();
	    GotTarget = 0;
	  }
	if(!GotTarget)
		aim_angle += iPat_Dir;
  }
  
  //Das Fahren selber ;)
  if(GotTarget)
  	aim_angle += BoundBy(target_angle-aim_angle,-2,2);
  
       
  /* Feinde suchen */
  
  if(!GotTarget)
  {
  	if(Shooting)
  	{
	    Shooting = false;
   		GetAttWeapon()->StopAutoFire();
  	}
  		
	  GotTarget = Search();
	}
	else
	{
		target_angle = Angle(GetX(), GetY() + 7, GotTarget->GetX(), GotTarget->GetY());
		if(Abs(aim_angle - target_angle) < 15)
		{
		    Shooting = true;
		    if(!GetAttWeapon()->IsShooting())
			    GetAttWeapon()->ControlThrow(this());
	  	    if(GetAmmo(GetAttWeapon()->GetFMData(FM_AmmoID), GetAttWeapon()) < GetAttWeapon()->GetFMData(FM_AmmoUsage))
				Reload();
		}
		else
		{
	    Shooting = false;
   		GetAttWeapon()->StopAutoFire();
		}
		
		if(!CheckTarget(GotTarget,this()))
			GotTarget = 0;
		else if(!PathFree(GetX(),GetY()+7,GotTarget->GetX(), GotTarget->GetY()))
			GotTarget = 0;
		else if(ObjectDistance(GotTarget,this()) > SearchLength())
			GotTarget = 0;
	}
	 
  /*
  //Berechnung
  iAngle = aim_angle;
  iWidth =  Sin(iAngle, SearchLength());
  iHeight = -Cos(iAngle, SearchLength());

  if(iWidth < 0)
    Search(iWidth, -iWidth, iHeight);
  else
    Search(0, iWidth, iHeight);
  */
}

public func Search(int iX, int iWidth, int iHeight)
{
  var pAim;
  
  /*DrawParticleLine("PSpark", 0, 0, -70 + Sin(aim_angle, SearchLength()), SearchLength(), 10, 80, RGB(255, 0, 0));
  DrawParticleLine("PSpark", 0, 0, 70 + Sin(aim_angle, SearchLength()), SearchLength(), 10, 80, RGB(255, 0, 0));*/ 

  var w,h;
  
  var Targets = FindTargets(this(), SearchLength()); //FindObjects(Find_Distance(SearchLength()),Find_PathFree(),Find_NoContainer());
  for(pAim in Targets)
  {
    if(GetOwner() != NO_OWNER)
      if(pAim->GetOwner() == GetOwner() || !Hostile(pAim->GetOwner(), GetOwner()))
        continue;

    if(!CheckTarget(pAim,this()))
      continue;

    //Winkel zum Ziel
    target_angle = Angle(GetX(), GetY() + 7, pAim->GetX(), pAim->GetY());

    target_angle = Normalize(target_angle, 0);
    if(target_angle < MaxRotLeft() || target_angle > MaxRotRight())
      continue;

    return pAim;
  }
  
  if(Shooting && !pAim)
  {
    Shooting = false;
    GetAttWeapon()->StopAutoFire();
  }
  
  if(!pAim)
    GotTarget = true;
}

private func Reload()
{
  // Munitionsart
  var AmmoID = GetAttWeapon()->~GetFMData(FM_AmmoID);
  // Erzeugen
  Local(0, CreateContents(AmmoID)) = GetAttWeapon()->~GetFMData(FM_AmmoLoad);
  // Waffe soll nachladen
  GetAttWeapon()->~Reloaded(this());
  GetAttWeapon()->~Recharge();
  GetAttWeapon()->~StopAutoFire();
}

public func MaxDamage() {
	return( 100 );
}

public func Damage()
{
  if(GetDamage() > MaxDamage())
    Explode(20);
}

public func EMPShock()
{
    //EMP Granate!1 :O
	EMPShockEffect(20*35);
	if(GetAttWeapon())
		GetAttWeapon()->StopAutoFire();
    return(1);
}

/* Konsolensteuerung */

public func ConsoleControl(int i)
{
    if(i == 1)
    {
      if(Active) return("$TurnOff$");
      else
          return("$TurnOn$");
    }
    if(i == 2 && EMPShocked())
        return("$Repair$");
}

public func ConsoleControlled(int i)
{
    if(i == 1)
    {
        if(Active) Active = 0;
        else
        {
            Active = 1;
        }
    }
    if(i == 3)
    {
        //Temporär mal noch nichts.
    }
}

//Für das Abfragen
public func GetAttWeapon() { return(cur_Attachment); } //Waffe
public func MaxRotLeft()   { return(110); } //Maximaler Winkel links
public func MaxRotRight()  { return(250); } //Maximaler Winkel rechts
public func SearchLength() { return(250); } //Suchlänge
public func AimAngle()     { return(aim_angle); } //Winkel auf Ziel
public func ReadyToFire()  { return(1); } //Allzeit bereit
public func IsMachine()    { return(true); } //Ist eine Elektrische Anlage
public func IsBulletTarget() { return(true); } //Kugelziel
public func IsAiming()     { return(true); } // Die Sentry Gun "zielt" immer

/* Serialisierung */

public func RejectContainedSerialize(object foo) { return !false; } // weg mit den alten Waffen

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetTeam(%d)", GetTeam());
	if (Active)
		extra[GetLength(extra)] = "TurnOn()";
	if (cur_Attachment) {
		extra[GetLength(extra)] = Format("Arm(%i)", GetID(cur_Attachment));
		extra[GetLength(extra)] = Format("LocalN(\"aim_angle\")=%d", aim_angle);
		extra[GetLength(extra)] = Format("LocalN(\"iPat_Dir\")=%d", iPat_Dir);
	}
}

public func IsThreat() { return(Active); }