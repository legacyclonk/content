/* Pferde sollen in den Aufzug passen */

#strict

#appendto ELEC

private func FitsInElevator(pObject)
{
  // Alle (ungedrehten) Vertices des Objekts prüfen
  var vertexCount = GetDefCoreVal("Vertices", "DefCore", pObject->GetID());
  for (var i = 0; i < vertexCount; i++) 
  {
    var vertexX = GetDefCoreVal("VertexX", "DefCore", pObject->GetID(), i);
    var vertexY = GetDefCoreVal("VertexY", "DefCore", pObject->GetID(), i);
    // Passt nicht
    if (!Inside(vertexX, -12, 11) || !Inside(vertexY, -20, 16)) return(0);
  }
  // Passt
  return(1);  
}

private func FindWaitingClonk()
{
  var clnk, best;
  while(clnk = FindObject(0, -30, RangeTop - GetY(), 60, 6000, OCF_CrewMember(), 0, 0, NoContainer(), clnk) )
    if(GetComDir(clnk) == COMD_Stop() || (GetAction(clnk) S= "Swim" && Inside(GetXDir(clnk), -5, 5)) ) //(GetComDir(clnk) == COMD_Down() || GetComDir(clnk) == COMD_Up() )) )
      if(GetAction(clnk) S= "Walk" || GetAction(clnk) S= "Push" || GetAction(clnk) S= "Scale" || GetAction(clnk) S= "Hangle" || GetAction(clnk) S= "Swim" || GetAction(clnk) S= "RideStill")
        if(!Hostile(GetController(clnk), GetOwner()) )
        {
          // Bei reitenden Clonks
	  if(GetAction(clnk)S="RideStill")
	  {
	    if(!(GetActionTarget(0,clnk)->~IsHorse())) continue;
            if(GetY(GetActionTarget(0,clnk))+8==GetY()) continue;
	  }
          // Nicht erreichbar?
          if(GetY(clnk) < GetY() - 7) if(!PathFree(GetX(), GetY(), GetX(), GetY(clnk)) ) continue;
          if(GetY(clnk) > GetY() + 7) if(!PathFree(GetX(), GetY() + 16, GetX(), GetY(clnk)) ) continue;
          // Priorität prüfen (Cursor hat Priorität, sonst nächster Clonk)
          if(!best)
            best = clnk;
          else if(GetCursor(GetController(clnk)) == clnk)
            best = clnk;
          else if(GetCursor(GetController(best)) != best)
            if(ObjectDistance(clnk) < ObjectDistance(best))
              best = clnk;
        }
  return(best);
}

private func MoveToWaitingClonk()
{
  // Wartender Clonk da? 
  var oClnk;
  if(!(oClnk = FindWaitingClonk()) ) return(0);

  // Bewegung nötig?
  if(Abs(GetY(oClnk) - GetY()) < 4 
     && GetAction(oClnk) ne "Swim") return(0);

  // Zielposition bestimmen
  var iTargetY = GetY(oClnk);
  if(GetAction(oClnk) S= "Swim")
    iTargetY -= 2;
  if(GetAction(oClnk) S= "RideStill")
    iTargetY += 8;
  if(GetY() > iTargetY)
    iTargetY -= 3;
  
  // Dorthin fahren
  SetMoveTo(iTargetY);
  return(1);              
}

/*
private func GrabAdjustPosition(obj) 
{
  var offset = (GetObjHeight(this()) - GetObjHeight(obj)) / 2 - 1;
  var x = GetX();

  if (GetCategory(obj) & C4D_Object()) 
  {
    if(GetOCF(obj) & OCF_HitSpeed1() ) return(0);
    offset = 8 + GetYDir() / 10;
    x = GetX(obj);
  }
  else if (GetID(obj) == LORY)
    offset = -1 + GetYDir() / 10;
  
  SetSpeed(0, GetYDir() / 20, obj); SetRDir(0, obj);
  if(GetR(obj)) SetR(0, obj);
  SetPosition(x, GetY() + offset, obj);
  obj->~EnterElevator();
}*/
