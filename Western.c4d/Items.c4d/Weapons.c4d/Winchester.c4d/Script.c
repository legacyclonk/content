/*-- Winchester --*/

#strict

/* Steuerung */

public func ControlThrow (pClonk)            // Bedienung: Werfen (benutzen)
{
  if(!(pClonk->~FireRifle()))
  {
    if(!GetPlrDownDouble(pClonk->GetOwner()))
      return(1);
    else
      return(0);
  }
  SetPhase(6, pClonk);
  // Fadenkreuz zum besseren Zielen erzeugen
  var pCross = CreateObject(WCHR, 0, 0, GetOwner(pClonk)); pCross->SetAction("Crosshair", pClonk);
  Local(0,GetCrosshair(pClonk)) = 84;
  WINC->ActualizePhase(pClonk);
  // dem Clonk übermitteln, wie viel Ammo da ist
  LocalN("iRifleAmmo", pClonk) = ContentsCount();
  while(Contents()) Enter(pCross, Contents());
  // eigene ID speichern
  LocalN("idWeapon",pClonk)=GetID();
  // Schnell noch schauen, ob der Clonk auch Munition hat!
  if(!LocalN("iRifleAmmo", pClonk)) DefinitionCall(GetID(), "CheckAmmo", pClonk);
    // ...und selbst löschen
  RemoveObject();
  return(1);
}

public func Activate (pClonk)
{
  // Schon/Noch am Laden?
  if(WildcardMatch(GetAction(pClonk), "*Load*")) return(1); 
  // Noch genug Muni vorhanden?
  if(ContentsCount()==6) return(Sound("RevolverNoAmmo", 0, pClonk));
  // Clonk hat keine Muni mehr?
  if(!pClonk->~GetCartridgeCount()) return(Sound("RevolverNoAmmo", 0, pClonk));
  // Inventar wieder auf Waffe verschieben
  ShiftContents(pClonk, 1, GetID());
  // Nachladen
  pClonk->~ControlThrow(pClonk);
  pClonk->~LoadRifle();
  // Kein Zielen nach dem Laden
  AddEffect("NoAim", pClonk, 1, 1, 0, GetID());
  // Dabei kein Fadenkreuz anzeigen
  var obj;
  if(obj = FindObject(WCHR, 0, 0, 0, 0, 0, "Crosshair", pClonk)) SetClrModulation(RGBa(255,255,255,255), obj);
  //RemoveObject(FindObject(WCHR, 0, 0, 0, 0, 0, "Crosshair", pClonk));
  return(1);
}

public func ControlDig(pClonk)
{
  if(WildcardMatch(GetAction(pClonk), "*Aim*"))
  {
    // Noch genug Muni vorhanden?
    if(LocalN("iRifleAmmo", pClonk)==6) return(Sound("RevolverNoAmmo", 0, pClonk));
    // Clonk hat keine Muni mehr?
    if(!pClonk->~GetCartridgeCount()) return(Sound("RevolverNoAmmo", 0, pClonk));
    LocalN("iAimPhase", pClonk)=GetPhase(pClonk);
    pClonk->~LoadRifle();
    return(1);
  }
}

public func FxNoAimTimer(object pTarget)
{
  var szAction = GetAction(pTarget);
  // Lädt der Clonk noch?
  if(WildcardMatch(szAction,"*Load*")) return(1);
  // Wenn er zielt abbrechen, sonst nichts machen (z.B. bei Tumble)
  // Hierbei sollte der Clonk gegebenfalls weiterreiten
       if(WildcardMatch(szAction,"*Ride*")) ObjectSetAction(pTarget,"Ride");
  else if(WildcardMatch(szAction, "*Aim*")) ObjectSetAction(pTarget,"Walk");
  return(-1);
}

public func ControlUp (pClonk)               // hoch zielen
{
  AimUp(pClonk, 1, "ControlConf", WINC);
}

public func ControlDown(pClonk)             // runter zielen
{
  AimDown(pClonk, 1, "ControlConf", WINC);
}

public func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  AimUpdate(clonk, comdir, 1, "ControlConf", WINC);
}

public func ControlConf(int conf, object clonk)
{
  if(clonk->GetAction() ne "AimRifle" && clonk->GetAction() ne "RideAimRifle")
  {
    AimCancel(clonk);
  }
  else
  {
    // Feinere Abstufung mit JnR möglich
    var angle_change;
    if(GetPlrJumpAndRunControl(clonk->GetController()))
      angle_change = conf * 2;
    else
      angle_change = conf * 6;

    Local(0, GetCrosshair(clonk)) = BoundBy(Local(0, GetCrosshair(clonk)) + angle_change, 0, 140);
    WINC->ActualizePhase(clonk);
  }
}

public func ActualizePhase(pClonk)
{
  var iDir = GetDir(pClonk)*2-1;
  var iAngle = Local(0,GetCrosshair(pClonk));
  var pObj = FindObject(WCHR,0,0,0,0,0,"Crosshair",pClonk); 
  SetVertexXY(0,-Sin(iAngle,40)*iDir,Cos(iAngle,40),pObj);
  if(!WildcardMatch(GetAction(pClonk),"*Aim*")) return(1);
       if(iAngle<  6) SetPhase(0,pClonk);
  else if(iAngle< 24) SetPhase(1,pClonk);
  else if(iAngle< 35) SetPhase(2,pClonk);
  else if(iAngle< 48) SetPhase(3,pClonk);
  else if(iAngle< 54) SetPhase(4,pClonk);
  else if(iAngle< 72) SetPhase(5,pClonk);
  else if(iAngle< 90) SetPhase(6,pClonk);
  else if(iAngle<108) SetPhase(7,pClonk);
  else if(iAngle<128) SetPhase(8,pClonk);
  else		      SetPhase(9,pClonk);
  return(1);
}

public func ControlLeft(pClonk)             // nach links drehen
{
  if(!WildcardMatch(GetAction(pClonk), "*Rifle*")) return();
  if(pClonk->~IsRiding())
    {
    GetActionTarget(0,pClonk)->~ControlLeft(pClonk);
    return(1);
    }
  if(GetDir(pClonk))
    SetDir(0,pClonk);
  WINC->ActualizePhase(pClonk);
  return(1);
}

public func ControlRight(pClonk)            // nach rechts drehen
{
  if(!WildcardMatch(GetAction(pClonk), "*Rifle*")) return();
  if(pClonk->~IsRiding())
    {
    GetActionTarget(0,pClonk)->~ControlRight(pClonk);
    return(1);
    }
  if(!GetDir(pClonk))
    SetDir(1,pClonk);
  WINC->ActualizePhase(pClonk);
  return(1);
}

public func ControlCommand(pClonk,iX,iY,iDevitation)
{
  if(!WildcardMatch(GetAction(pClonk), "*Rifle*")) return();
  // lädt noch nach
  if(GetAction(pClonk) ne "AimRifle" && GetAction(pClonk) ne "RideAimRifle") return(1);

  // Zielwinkel

  var iAngle = Angle(GetX(pClonk),GetY(pClonk),iX,iY);

  // Bei größerer Distanz höher zielen

  if (Inside(iX - GetX(pClonk),+1,+300))

    iAngle -= Abs(iX - GetX(pClonk)) / 50;

  if (Inside(iX - GetX(pClonk),-300,-1))

    iAngle += Abs(iX - GetX(pClonk)) / 50;

  // zur richtigen Seite hin schießen
  if(iAngle < 180) {
    SetDir(1,pClonk);
    SetPhase(BoundBy(iAngle,0,135)/15,pClonk);
    if(GetAction(pClonk) eq "RideAimRifle" && GetDir(GetActionTarget(0,pClonk)) == DIR_Left()) GetActionTarget(0,pClonk)->~TurnRight();
  }
  else {
    SetDir(0,pClonk);
    SetPhase((360-BoundBy(iAngle,225,360))/15,pClonk);
    if(GetAction(pClonk) eq "RideAimRifle" && GetDir(GetActionTarget(0,pClonk)) == DIR_Right()) GetActionTarget(0,pClonk)->~TurnLeft();
  }
  if(iDevitation) iAngle += RandomX(-iDevitation/2, iDevitation/2);
  if(iAngle>180) iAngle=-iAngle+360;
  Local(0, GetCrosshair(pClonk)) = iAngle;
  WINC->ActualizePhase(pClonk);
  RifleShot(pClonk, iAngle);


  return(1);
}


public func Fire(pClonk)                // Bedienung: Feuern!
{
    // verschießen
    if(GetAction(pClonk) eq "AimRifle" || GetAction(pClonk) eq "RideAimRifle") RifleShot(pClonk);
    return(1);
}

protected func RifleShot(pClonk, iAngle)         // Schuss mit dem Gewehr
{
  var pObj, pAmmo, iX, iY, iR, iXDir, iYDir, iDir, iPhase, i, i2;

  // Noch Ammo da?
  if(!LocalN("iRifleAmmo", pClonk)) return(DefinitionCall(LocalN("idWeapon", pClonk), "CheckAmmo", pClonk));
  // Clonk lädt keine neue Kugel rein?
  if(GetEffect("*ReloadRifle*", pClonk)) return(0);

  // Wallhack-Check
  if(FindObject(NW4V))
    if(IsWallhack(pClonk,WINC)) {
      PlayerMessage(GetOwner(pClonk), "$TxtNoShoot$", pClonk);
      return(0);
    }

  // Ammo ist noch vorhanden, also Patrone her
  pAmmo = Contents(0, GetCrosshair(pClonk));
  LocalN("iRifleAmmo", pClonk)--;

  // Austrittsparameter
  iDir = GetDir(pClonk)*2-1;
  iPhase = GetPhase(pClonk);
  if(!iAngle) iAngle = Local(0,GetCrosshair(pClonk));
  i  = [-11,1,15,28,43,62,76,95,115,135][iPhase];
  iX = +Sin(i,14)*iDir;
  iY = -Cos(i,14)-2;
  iR = iAngle*iDir + 90;
  iXDir = (Sin(iAngle,2200)+100)*iDir;
  iYDir = -Cos(iAngle,2200)+100;

  // Besitzer des Projektils setzen
  SetOwner(GetOwner(pClonk), pAmmo);

  // Abfeuern
//  Exit(pAmmo, AbsX(iX+GetX(pClonk)), AbsY(iY+GetY(pClonk)), iR, iXDir, iYDir);
  Exit(pAmmo, AbsX(iX+GetX(pClonk)), AbsY(iY+GetY(pClonk)), iR);
  SetXDir(iXDir, pAmmo, 100);
  SetYDir(iYDir, pAmmo, 100);
  // Ein Captain trifft schmerzhafter
  if(pClonk->~IsCaptain()) pAmmo->~Launch(-1, 14+Random(10));
  else pAmmo->~Launch(-1, 10+Random(10));
  
  // Mündungsfeuer
  i  = [-6,5,17,30,41,56,71,88,108,133][iPhase];
  i2 = Inside(iPhase,2,5)*2;
  if(iPhase == 2) i2--;
  iX = +Sin(i,17+i2)*iDir;
  iY = -Cos(i,17+i2)-1+GetDir(pClonk);
  CreateParticle("MuzzleFlash", AbsX(iX+GetX(pClonk)), AbsY(iY+GetY(pClonk)), iXDir, iYDir, 35, RGBa(255,255,255,0), pClonk);
  // Sound
  Sound("RifleShot", 0, pClonk);
  // Rauch
  Smoke(iX, iY, 2);
  Smoke(iX, iY + Random(2), 3);
  // Patronenhülse fliegt raus
  CreateParticle("Casing",AbsX(iX/2+GetX(pClonk)),AbsY(iY/2+GetY(pClonk)),-iDir*RandomX(1,5),-RandomX(3,7),15,RGBa(250,140,80,0));
  // Der Clonk muss eine Kugel einladen
  AddEffect("ReloadRifle", pClonk, 101, 1);
  return(1);
}

public func IsWallhack(pClonk, iID) { return(_inherited(pClonk, iID)); }

/* Neue Kugel einladen */
// Dies hier wird gebraucht, damit der Clonk kein Dauerfeuer schießen kann

global func FxReloadRifleTimer (objTarget, iNumber, iTime)
{
  if(iTime > 30) return(-1);
}
  

/* TimerCall (um Gottes Willen...!): Bei Bedarf Träger nach passender Munition durchsuchen */
// Anders, ganz ganz anders!

protected func CheckAmmo (pClonk)
{
  // Clonk hat gar kein Gewehr?!
  if((GetAction(pClonk) ne "AimRifle") && (GetAction(pClonk) ne "RideAimRifle")) return(0);
  // Clonk hat keine Muni mehr?
  if(!pClonk->~GetCartridgeCount()) return(Sound("RevolverNoAmmo", 0, pClonk));

  // Clonk hat das Gewehr und noch Munition. Gut, dann soll er nachladen!
  LocalN("iAimPhase", pClonk)=GetPhase(pClonk);
  pClonk->~LoadRifle();
}

/* Fadenkreuz finden */
public func GetCrosshair(pClonk)
{
  return(FindObject(WCHR, 0, 0, 0, 0, 0, "Crosshair", pClonk));	
}

/* Objekt ist eine Waffe */
public func IsWeapon () { return(1); }
  
/* Objekt ist keine Handfeuerwaffe */
public func IsGun () { return(0); }
  
/* Objekt ist ein Gewehr */
public func IsRifle () { return(1); }

/* Objekt wurde ausgewählt */
public func Selection ()
{
  Sound("RevolverDraw");
}

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }
