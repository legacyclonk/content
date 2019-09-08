/*--- Luftabwehrgesch¸tz (angeschraubt) ---*/

#strict

#include CTW2

protected func RotationSpeed() { return(6); }
protected func CannonMobileID() { return(CTW7); }

/* Munition aufnehmen */

public func ComEnter(object pAmmo)
{
  // Gepackte Muni
  if(pAmmo->~IsArrowPack() || GetID(pAmmo) == ADM1)
  {
    // Aufnehmen
    Sound("Connect");
    if (GetID(pAmmo) == ADM1) pAmmo->~Unpack(this()); else Enter(this(), pAmmo);
  }
  // Einzelne Munitionskomponenten
  else if(pAmmo->~IsArrow() || GetID(pAmmo) == ADB1)
  {
    Sound("Click");
    Enter(this(), pAmmo);
  }
}

public func ComFire(object pClonk)
{
  Sound("Click");
  SetAction("Attaching", GetActionTarget() );
  SetRDir(0);
    
  // Bis zu 2 Projektile verschieﬂen
  for(var i = 0; i < 2; ++ i)
  {
    // Projektil suchen
    var c = ContentsCount() - 1, obj, ammo;
    while(obj = Contents(c--) )
    {
      if (obj->~IsArrowPack())
      {
        ammo = obj->~GetItem();
        if (!ammo)
          {
          // Alter Pfeilpaket: Erstmal nur aufteilen; sp‰ter wieder versuchen (Rekursionsschutz gegen neues Zusammenpacken)
          Sound("Connect");
          Split2Components(obj);
          break;
          }
      }
      else if(obj->~IsArrow() || GetID(obj) == ADB1)
      {
        ammo = obj;
        break;
      }
    }
  
    // Keins da? :(
    if(!ammo)
    {
      if(!i) Sound("Error");
      else Sound("Click");
      return(0);
    }

    // Sound    
    if(!i) Sound("Arrow");
    
    
    var speed = 15;
    if(GetID(ammo) == ADB1) speed = 22;
    
    // Rotationsgeschwindigkeit
    var rdir = 4;
    if(GetR() < 0) rdir = -4;
    if(Inside(GetR(), -10, 10) ) rdir = 0;
    
    // Controller setzen
    ammo->SetController(pClonk->GetController());
    
    // Ausstoﬂ
    Exit(ammo, Sin(GetR() - 45 + 90 * i, 9), -Cos(GetR() - 45 + 90 * i, 9), GetR(), Sin(GetR(), speed), -Cos(GetR(), speed), rdir);
    SetPlrView(GetOwner(pClonk), ammo);
    ammo->~Launch();
  }
}

