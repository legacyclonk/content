/* Schleuder */

#strict

/* Objekt ist eine Schleuder */
public func IsSlingshot() { return(1); }

public func ControlThrow(pClonk)            // Bedienung: Werfen (benutzen)
{
  // Nur im Laufen
  if(!WildcardMatch(GetAction(pClonk),"*Walk*") && !WildcardMatch(GetAction(pClonk),"*Jump*"))return(1);
  // nur schleudern, wenn Objekt zum Schleudern da
  if(ContentsCount(0,pClonk)<=1) return(1);
  if(ContentsCount(0,pClonk)==2) if(DefinitionCall(GetID(Contents(1,pClonk)),"IsSpear")) return(1);
  if(WildcardMatch(GetAction(pClonk),"*Jump*"))
   ObjectSetAction(pClonk,"JumpSlingSpin");
  else ObjectSetAction(pClonk,"SlingSpin");
  // eigene ID speichern und selbst löschen
  LocalN("idWeapon",pClonk)=GetID();
  RemoveObject();
  return(1);
}

public func ControlLeft(pClonk)             // Bedienung: links
{
  if(GetDir(pClonk)) SetDir(0,pClonk);
}

public func ControlRight(pClonk)            // Bedienung: rechts
{
  if(!GetDir(pClonk)) SetDir(1,pClonk);
}

public func Fire(pClonk)                // Bedienung: Feuern!
{
    // Speer nicht verschleudern
    if(DefinitionCall(GetID(Contents(0,pClonk)),"IsSpear")) return(1);
    // sonst Schleuderaktion ausführen (also feuern)
    if(!WildcardMatch(GetAction(pClonk),"*SlingThrow*"))
    {
     if(WildcardMatch(GetAction(pClonk),"*Jump*"))
      ObjectSetAction(pClonk,"JumpSlingThrow");
     else ObjectSetAction(pClonk,"SlingThrow");
    }
    return(1);
}

protected func Throwing(pClonk)         // Schuss, weitergeletet von der Schleuderaktion des Clonks
{
  var pObj, iX, iY, iR, iXDir, iYDir, iRDir, iDir;
  if(!(pObj=Contents(0,pClonk))) return();

  // Wurfparameter berechnen
  iDir = GetDir(pClonk)*2-1;
  iX = 10*iDir;
  iY = -6;
  iR = 90+60*iDir;
  iXDir = 4*iDir;
  iYDir = -3;
  iRDir = 6*iDir;
  // wech damit
  Exit(pObj, AbsX(iX+GetX(pClonk)), AbsY(iY+GetY(pClonk)), iR, iXDir, iYDir, iRDir);
  // Sound beim Clonk abspielen, da das hier nur ein DefinitionCall ist.
  pClonk->Sound("SlingshotFling");
  return(1);
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }
