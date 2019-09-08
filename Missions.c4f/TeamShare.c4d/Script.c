/*-- Gemeinschaft --*/

#strict 2

/* Initialisierung */

protected func Initialize()
{
  // Spieler ohne Konto?
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    CreateObject(ACNT,0, 0, GetPlayerByIndex(i));
  }
}

public func InitializePlayer(iPlr)
{
  // Neuer Spieler, neues Konto
  var pOldAccount = FindObject2(Find_ID(ACNT), Find_Owner(iPlr));
  if (pOldAccount) RemoveObject(pOldAccount);
  CreateObject(ACNT, 0, 0, iPlr);
}

public func RemovePlayer(iPlr)
{
  // Beim Tod das Konto entfernen
  var pAccount = FindObject2(Find_ID(ACNT), Find_Owner(iPlr));
  if(pAccount)
    RemoveObject(pAccount);
}

protected func Activate(iPlr)
{
  var szMessage = FindObject2(Find_ID(ACNT), Find_Owner(iPlr))->GetTeam();
  MessageWindow(szMessage, iPlr);
}

protected func Destruction()
{
  // Keine Teamaccounts, keine Konten
  // fixme: Sollte Gemeinschaftskonten vorher auflösen...
  if (ObjectCount(GetID())<=1) RemoveAll(ACNT);
  return 1;
}

protected func Timer()
{
  // Alle Spieler behandeln
  for(var i = 0; i < GetPlayerCount(); i++)
    HandlePlayer(GetPlayerByIndex(i));
}

private func HandlePlayer(iPlr)
{
  var idPlan, i;
  // Baupläne abgleichen
  if(!GameCall("TACC_BlockTeamExchange", TACC_TypePlans))
    while (idPlan = GetPlrKnowledge(iPlr, 0, i++, C4D_All))
      TransferToAllies(iPlr, idPlan);
  // Zauber übergeben
  if(!GameCall("TACC_BlockTeamExchange", TACC_TypeSpells))
    while (idPlan = GetPlrMagic(iPlr, 0, i++))
      TransferToAllies(iPlr, idPlan, 1); 
}

private func TransferToAllies(iPlr, idPlan, fSpell)
{
  var iToPlr;
  // Alle Spieler durchgehen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    iToPlr = GetPlayerByIndex(i);
    // Welche sind Freunde?
    if(iToPlr != iPlr && !Hostile(iToPlr, iPlr))
    {
      // Wer hat den Plan oder Zauber noch nicht?
      if(!fSpell && !GetPlrKnowledge(iToPlr, idPlan))
        SetPlrKnowledge(iToPlr, idPlan);
      if(fSpell  && !GetPlrMagic(iToPlr, idPlan))
        SetPlrMagic(iToPlr, idPlan);
    }
  }
}

static const TACC_TypeValue   = 1;
static const TACC_TypeScore   = 2;
static const TACC_TypeBaseMat = 3;
static const TACC_TypePlans   = 4;
static const TACC_TypeSpells  = 5;
