/*-- Gemeinschafts Basismaterial --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  // Spieler ohne Konto?
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    CreateObject(BACN,0, 0, GetPlayerByIndex(i) );
  }
}

public func InitializePlayer(iPlr)
{
  // Neuer Spieler, neues Konto
  var old; if (old=FindObjectOwner(BACN, iPlr)) RemoveObject(old);
  CreateObject(BACN, 0,0, iPlr);
}

protected func Activate(iPlr)
{
  var ms=ObjectCall(FindObjectOwner(BACN, iPlr), "GetTeam");
  if (ms) ms=Format("Du teils dir dein kaufbares Material mit: %s",ms);
  if (!ms) ms="Du teils dir dein kaufbares Material nicht.";
  MessageWindow(ms, iPlr);
}

protected func Destruction()
{
  // Keine Teamaccounts, keine Konten
  // fixme: Sollte Gemeinschaftskonten vorher auflösen...
  if (ObjectCount(GetID())<=1) RemoveAll(BACN);
  return(1);
}

