#strict

static g_BaseAcquired;

protected func Initialize()
{
  g_BaseAcquired = CreateArray(2);  
  // Melee
  if (!ObjectCount(MELE)) CreateObject(MELE);
  // Team accounts
  if (!ObjectCount(TACC)) CreateObject(TACC);
  // Fahrstuhlsch‰chte
  FindObject(CPEL, 1824, 300)->CreateShaft(350);
  FindObject(CPEL, 2260, 300)->CreateShaft(280);
  return(1);
}

protected func InitializePlayer(iPlayer)
{
  // Basis in Besitz nehmen
  var iTeam = GetPlayerTeam(iPlayer) - 1;
  if (!g_BaseAcquired[iTeam])
  {   
    // Besitznahme
    AcquireBase(iPlayer, 1200*iTeam,0,1200,1500);
    // Flagge erzeugen
    CreateContents(FLAG, GetHiRank(iPlayer));
    // Nur einmal akquirieren
    g_BaseAcquired[iTeam] = true;
  }

  // Baupl‰ne setzen
  DefinitionCall(CPPL, "SetKnowledge", iPlayer);
  DefinitionCall(WPPL, "SetKnowledge", iPlayer);

  // Fertig!
  return(1);
}
  
  
public func AcquireBase(iPlayer, iPosx, iPosy, iWidth, iHeight)
{
 var obj;

 // Alles in Besitz nehmen
 while (obj = FindObject(0, iPosx, iPosy, iWidth, iHeight, 0, 0,0, 0, obj))
  if (GetOwner(obj) == -1)
    if (obj->GetID() != FISH) // Auﬂer die Fiche!!
      SetOwner(iPlayer, obj);

 // Fertig!
 return(1);
}
