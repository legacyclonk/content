/* Blasrohr */

#strict

public func IsBlowtube() { return(1); }

public func ControlThrow(pClonk)            // Bedienung: Werfen (benutzen)
{
  // Nur im Laufen
  if(!WildcardMatch(GetAction(pClonk),"*Walk*"))return(1);
  // Nur wenn Pfeile vorhanden sind
  if(!pClonk->GetArrowCount())return(1);
  ObjectSetAction(pClonk,"ReloadBlowtube");
  LocalN("iAimPhase",pClonk)=6;
  // eigene ID speichern und selbst löschen
  LocalN("idWeapon",pClonk)=GetID();
  RemoveObject();
  return(1);
}

public func ControlConf(int conf, object controller)
{
  if(controller->GetAction() ne "AimBlowtube")
   if(controller->GetAction() ne "ReloadBlowtube")
    return(1);
   else
   {
    var oldAngle=LocalN("iAimPhase",controller);
    LocalN("iAimPhase",controller)=BoundBy(LocalN("iAimPhase",controller)+conf, 0, GetActMapVal("Length", "AimBlowtube", GetID(controller))-1);
    return LocalN("iAimPhase",controller) != oldAngle;;
   }
  controller->AimStdConf(conf);
  return(1);
}

public func ControlUp(pClonk)               // hoch zielen
{
  // GetID() geht nicht, weil wir keinen Objektkontext haben
  AimUp(pClonk, 4, "ControlConf", BLTB);
}

public func ControlDown(pClonk)             // runter zielen
{
  // GetID() geht nicht, weil wir keinen Objektkontext haben
  AimDown(pClonk, 4, "ControlConf", BLTB);
}

public func ControlUpdate(pClonk, int comdir, bool dig, bool throw)
{
  // GetID() geht nicht, weil wir keinen Objektkontext haben
  AimUpdate(pClonk, comdir, 4, "ControlConf", BLTB);
}

public func ControlLeft(pClonk)             // nach links drehen
{
  if(GetDir(pClonk))
    SetDir(0,pClonk);
  return(1);
}

public func ControlRight(pClonk)            // nach rechts drehen
{
  if(!GetDir(pClonk))
    SetDir(1,pClonk);
  return(1);
}

public func ControlCommand(pClonk,iX,iY)
{
  // lädt noch nach
  if(GetAction(pClonk) ne "AimBlowtube") return(1);

  // Zielwinkel
  var iAngle = Angle(GetX(pClonk),GetY(pClonk),iX,iY);
  // Bei größerer Distanz höher zielen
  if (Inside(iX - GetX(),+1,+300))
    iAngle -= Abs(iX - GetX()) / 8;
  if (Inside(iX - GetX(),-300,-1))
    iAngle += Abs(iX - GetX()) / 8;
    
  // zur richtigen Seite hin schießen
  if(iAngle < 180) {
    SetDir(1,pClonk);
    SetPhase(BoundBy(iAngle,0,135)/15,pClonk);
  }
  else {
    SetDir(0,pClonk);
    SetPhase((360-BoundBy(iAngle,225,360))/15,pClonk);
  }

  BlowtubeShot(pClonk);

  return(1);
}

public func Fire(pClonk)                // Bedienung: Feuern!
{
    // verschießen
    if(GetAction(pClonk) eq "AimBlowtube") BlowtubeShot(pClonk);
    return(1);
}

protected func BlowtubeShot(pClonk)         // Schuss mit dem Blasrohr
{
  var pObj, pObj2, iX, iY, iR, iXDir, iYDir, iRDir, iDir, iPhase;
  if(!(pObj=(pClonk->~GetArrow()) )) return(0);
  // Wurfparameter berechnen
  iDir = GetDir(pClonk)*2-1;
  iPhase = GetPhase(pClonk);
  iX = +Sin(iPhase*15,14)*iDir;
  iY = -Cos(iPhase*15,14)-2;
  iR = iPhase*15*iDir;
  iXDir = (Sin(iPhase*15+RandomX(-6,+6),8)+1)*iDir;
  iYDir = -Cos(iPhase*15+RandomX(-6,+6),8);
  iRDir = 3*iDir;
  // wech damit
  Exit(pObj, AbsX(iX+GetX(pClonk)), AbsY(iY+GetY(pClonk)), iR, iXDir, iYDir, iRDir);
  pObj->~Launch();
  // Sound beim Clonk, da das hier nur ein DefinitionCall ist.
  pClonk->Sound("Arrow");
  // Reload-Aktion, Phase merken
  LocalN("iAimPhase",pClonk)=GetPhase(pClonk);
  ObjectSetAction(pClonk,"ReloadBlowtube");
  // Sicht verfolgen
  if(GetPlrViewMode(GetOwner(pClonk))!=2)
    SetPlrView(GetOwner(pClonk),pObj);
  return(1);
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }
