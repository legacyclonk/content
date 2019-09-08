/*-- Ritterlager --*/

#strict

static g_fLeftBaseAcquired, g_fRightBaseAcquired;

protected func Initialize()
{
  // Team accounts
  if (!ObjectCount(TACC)) CreateObject(TACC);
  // Böden der Burgen glätten
  DigFreeRect(0,214,500,15);
  DigFreeRect(1240,214,500,15);
  // Fertig!
  return(1);
}

protected func InitializePlayer(iPlayer)
{
  // Waffenbaupläne
  DefinitionCall(WPPL, "SetKnowledge", iPlayer);
  
  // Flagge und Zeltpack erzeugen
  CreateContents(FLAG, GetHiRank(iPlayer));
  CreateContents(TENP, GetHiRank(iPlayer));
  
  // Basen (nur einmal) in Besitz nehmen
  if (GetPlayerTeam(iPlayer)==1 && !g_fLeftBaseAcquired)
    { AcquireBase(iPlayer,0,100, 500,130); g_fLeftBaseAcquired=true; }
  if (GetPlayerTeam(iPlayer)==2 && !g_fRightBaseAcquired)
    { AcquireBase(iPlayer,1230,100, 500,130); g_fRightBaseAcquired=true; }
  
  // Fertig!
  return(1);
}

public func AcquireBase(iPlayer, iPosx, iPosy, iWidth, iHeight)
{
  var obj;
  
  // Alles in Besitz nehmen
  while (obj = FindObject(0, iPosx, iPosy, iWidth, iHeight, 0, 0,0, 0, obj))
    // Aber keine CrewMember...
    if (~obj->GetOCF() & OCF_CrewMember)
      SetOwner(iPlayer, obj);
    
  // Fertig!
  return(1);
}