/*-- Lore --*/

#strict

/* Status */

public func IsLorry() { return(1); }

/* Steuerung */

protected func ContactLeft() {
  if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
}

protected func ContactRight() {
  if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
}

private func ControlElevator(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(0, +1,+1,0,0, OCF_Grab(), 0,0, NoContainer(), pElev))
    // Im Fahrstuhlkorb
    if(pElev->~IsElevator())
      // Steuerung an Fahrstuhl weiterleiten
      return(ObjectCall(pElev,szCommand,pObject));
  return(0);
}

private func ControlElevatorMovement(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(0, +1,+1,0,0, OCF_Grab(), 0,0, NoContainer(), pElev))
    // Fahrstuhlkorb gefunden
    if(pElev->~IsElevator())
      // Lore ist angehalten
      if (GetComDir() == COMD_Stop())
        // Steuerung an Fahrstuhl weiterleiten
        return(ObjectCall(pElev,szCommand,pObject));
  return(0);
}

private func ControlElevatorStop(string szCommand, object pObject)
{
  var pElev;
  // Objekte an dieser Position überprüfen
  while (pElev = FindObject(0, +1,+1,0,0, OCF_Grab(), 0,0, NoContainer(), pElev))
    // Fahrstuhlkorb gefunden
    if(pElev->~IsElevator())
      // Fahrstuhlkorb bewegt sich
      if (GetComDir(pElev) != COMD_Stop())
      {
        // Fahrstuhlkorb anhalten
        ObjectCall(pElev,szCommand,pObject); 
        // Noch nicht aussteigen
        SetComDir(COMD_Stop(), pObject);
        SetComDir(COMD_Stop());
        // Bewegunsbefehl abbrechen
        return(1);
      }
  return(0);
}

/* Füllmengenkontrolle */

private func MaxContents() { return(50); }

protected func RejectCollect(id idObj,object pObj)
{
  if(ContentsCount() < MaxContents()) return(0, Sound("Clonk"));
  if(Contained(pObj)) return(Message("$TxtLorryisfull$", this()));
  if(Abs(GetXDir(pObj))>6) SetYDir(-5,pObj);
  Sound("WoodHit*");
  return(1);
}


/* Automatisches Ausleeren in Gebäuden */

protected func Entrance(object pNewContainer)
  {
  // Nur in Gebäuden (auch Burgteile)
  if (GetCategory(pNewContainer) & (C4D_StaticBack | C4D_Structure))
    // Nicht, wenn es das Gebäude verbietet
    if (!pNewContainer->~NoLorryEjection(this()) && !pNewContainer->~IsStaircase())
      {
      // Lore entleeren
      pNewContainer->GrabContents(this());
      }
  }

/* Forschung */

public func IsDeepSeaResearch() { return(1); }

/* Einlade-Helfer */

protected func ContextLoadUp(object pClonk)
{
  [$TxtLoadUp$|Image=LRY1]
  // Alte Kommandos des Clonks löschen
  SetCommand(pClonk, "None");
  // Lore ist bereits voll
  if (ContentsCount() >= MaxContents())
    return(Message("$TxtLorryisfull$", this()));
  // Maximale noch mögliche Zuladung bestimmen
  var iMaxLoad = MaxContents() - ContentsCount();
  // Frei liegende Gegenstände in der Umgebung automatisch einladen
  var iRange = 60;
  var pObj, iCount;
  while (pObj = FindObject(0, -iRange, -iRange/2, iRange*2, iRange*2, OCF_Collectible, 0,0, NoContainer(), pObj))
    if (pObj->GetOCF() & OCF_Available)
    {
      // Maximale Zuladung berücksichtigen (auch die noch vom Clonk kommt)
      if (++iCount > iMaxLoad - pClonk->ContentsCount()) break;
      // Einladen
      AddCommand(pClonk, "Put", this(), 0,0, pObj);
    }
  // Der Clonk soll seine getragenen Objekte auch einladen (ansonsten legt er sie nur irgendwo ab)
  for (var i = 0; i < Min(pClonk->ContentsCount(), iMaxLoad); i++)
    AddCommand(pClonk, "Put", this(), 0,0, pClonk->Contents(i));
}
