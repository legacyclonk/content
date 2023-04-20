#strict 2

global func Arena_ChooserFinished()
{
  // Bei InstaGIB werden alle Spawnpunkte entfernt und die Waffen aus den Spielern entfernt
  if(FindObject(IGIB))
  {
    for(var spwn in FindObjects(Find_ID(SPNP)))
      RemoveObject(spwn);
    for(var clonk in FindObjects(Find_OCF(OCF_CrewMember)))
      while(Contents(0, clonk))
        RemoveObject(Contents(0, clonk));
    for(var bonus in FindObjects(Find_ID(BSPN)))
    {
      // Und den Bonusspawnpunkt anpassen
      bonus->~RemoveBonus(AMPB); // Ammopack weg
      bonus->~RemoveBonus(BSKB); // Berserker weg
      bonus->~RemoveBonus(HELB); // Heal weg
    }
  }
  // Bei Keine-Munition werden Munispawns entfernt
  if(FindObject(NOAM))
  {
    for(var spwn in FindObjects(Find_ID(SPNP)))
      if(Contents(0, spwn)->~IsAmmoPacket())
        RemoveObject(spwn);
    // Und den Bonusspawnpunkt anpassen
    for(var bonus in FindObjects(Find_ID(BSPN)))
      bonus->~RemoveBonus(AMPB); // Ammopack weg
  }

    // Bei Waffenwahl werden alle Nicht-Munispawns entfernt
  var wp = FindObject(WPCH);
  if(wp)
  {
    for(var spwn in FindObjects(Find_ID(SPNP)))
      if(!(Contents(0, spwn)->~IsAmmoPacket()))
        RemoveObject(spwn);
    for(var bonus in FindObjects(Find_ID(BSPN)))
      bonus->~AddBonus(MNYB); // Money dazu
  }
}

global func Arena_RelaunchPlayer(int iPlr, object pCrew, int iKiller, int iTeam)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER)
    return;
  // Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  // Clonk tot?
  if(!GetAlive(pCrew))
    pCrew = Arena_RelaunchClonk(iPlr, pCrew);
  // Zufallsposition
  var pos = GameCall("RelaunchPosition",iTeam);
	if(!pos) return;
  var iX = pos[0];
  var iY = pos[1];
  // Lecker Waffen
  if(!FindObject(IGIB))
  {
    var pistol = CreateContents(PIWP, pCrew);
    pistol->DoAmmo(pistol->GetFMData(FM_AmmoID), pistol->GetFMData(FM_AmmoLoad));
    DoAmmo(pistol->GetFMData(FM_AmmoID), pistol->GetFMData(FM_AmmoLoad), pCrew);
    DoAmmo(GRAM, 2, pCrew);
  }
   if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);
  // Fertig gerelauncht
}

global func Arena_RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk = CreateObject(HZCK, 10, 10, iPlr);
  if(pCursor)
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+150, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);

  // Geldspritze
  if(FindObject(WPCH))
  {
    DoWealth(iPlr, +25);
  }

  // Wegstecken
  var tim = CreateObject(TIM2, 10, 10, -1);
  pClonk->Enter(tim);
  PlayerMessage(iPlr, Format("@%s", GetName(pClonk)), tim);

  return pClonk;
}