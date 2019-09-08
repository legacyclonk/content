/*-- Gemeinschaftskonto --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  // Spieler ohne Konto?
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    CreateObject(ACNT,0, 0, GetPlayerByIndex(i) );
  }
}

public func InitializePlayer(iPlr)
{
  // Neuer Spieler, neues Konto
  var old; if (old=FindObjectOwner(ACNT, iPlr)) RemoveObject(old);
  CreateObject(ACNT, 0,0, iPlr);
}

protected func Activate(iPlr)
{
  var ms=ObjectCall(FindObjectOwner(ACNT, iPlr), "GetTeam");
  if (ms) ms=Format("Du hast ein Gemeinschaftskonto mit: %s",ms);
  if (!ms) ms="Du hast kein Gemeinschaftskonto.";
  MessageWindow(ms, iPlr);
}

protected func Destruction()
{
  // Keine Teamaccounts, keine Konten
  // fixme: Sollte Gemeinschaftskonten vorher auflösen...
  if (ObjectCount(GetID())<=1) RemoveAll(ACNT);
  return(1);
}
