/*-- Spielzielsteuerung --*/

#strict 2

// Bei allen C4D_Goal-Objekten zu importieren
// Überladbare Funktionen:
// int IsFullfilled(); - Ist das Ziel erfüllt?
// Statt Initialize InitGoal benutzen

local curr_goal; // Aktives GameGoal-Prüfobjekt (Früher Local(0))

local missionPassword; // Missionspasswort, das beim Erfüllen des Spielziels gesetzt wird

// Initialisierung

protected func Initialize()
{
  // Bei GOAL selber nur Warteaktion
  if(GetID()==GOAL) return SetAction("Init");
  // Objekt mit Timer erstellen
  if (!FindObject(GOAL)) CreateObject(GOAL,0,0,NO_OWNER);
  // Fertig
  return 1;
}

// Nachinitialisierung für GOAL

protected func PostInit()
{
  // Ein CheckFullfilled-Broadcast an alle
  for(var obj in FindObjects(Find_Category(C4D_Goal)))
    if (obj)
      obj->~IsFulfilled();
  // In Ligaspielen öfters prüfen
  UpdateTransferZone();
  return 1;
}

protected func UpdateTransferZone()
{
  // In Ligaspielen öfters prüfen
  if(GetLeague() && !GetEffect("IntLeagueCheck", this) && GetID() == GOAL)
    AddEffect("IntLeagueCheck", this, 1, 5, this);
  return 1;
}

protected func FxIntLeagueCheckTimer(object target, int number)
{
  CheckTime();
}

// ActMap-Callback zum Beenden der Runde

protected func RoundOver()
{
  // Das wars ;(
  GameOver();
  return RemoveObject();
}

// Timer

protected func CheckTime()
{
  // Zielprüfung aktiv?
  if(!ActIdle()) return 1;
  if (ObjectCount(RVLR))
  {
    if (CheckRivalry()) return 1;
  } 
  else
  {
    if (CheckCooperative()) return 1;
  }
  // Ziel erfüllt: Vom Szenario abgefangen?
  if (GameCall("OnGoalsFulfilled")) return RemoveObject();
  // Tja, jetzt ist das Spiel vorbei. Erstmal die Belohnung
  Sound("Trumpet", 1);
  // Jetzt ein wenig warten, und dann GameOver
  SetAction("Wait4End");
  return 1;
}

private func CheckCooperative()
{
  // Momentanes Zielobjekt prüfen
  if (curr_goal) 
    if (!curr_goal->~IsFulfilled()) 
      return 1;
  // Jetzt in der Schleife suchen
  curr_goal=0;
  var iCount = 0;
  for(curr_goal in FindObjects(Find_Category(C4D_Goal)))
  {
    iCount++;
    if (!curr_goal->~IsFulfilled()) return 1;
    // Spielziel erfüllt: Missionspasswort setzen
    if (curr_goal->LocalN("missionPassword"))
      GainMissionAccess(curr_goal->LocalN("missionPassword"));
  }
  // Kein Zielobjekt? Wir sind überflüssig :(
  if (!iCount) return RemoveObject();
  // Spiel zuende
  return 0;
}

private func CheckRivalry()
{
  var goal_count = 0, bits;
  //Erstmal annehmen, dass alle Spieler erfolgreich waren
  var i = GetPlayerCount();
  while (i--) bits = SetBit(bits, i, 1);
  // Jetzt in der Schleife suchen
  curr_goal = 0;
  for(curr_goal in FindObjects(Find_Category(C4D_Goal)))
  {
    ++goal_count;
    // Prüfen, ob das Ziel nicht erfüllt wurde
    i = GetPlayerCount();
    while (i--) 
      if (curr_goal)
        if (!curr_goal->~IsFulfilledforPlr(GetPlayerByIndex(i)))
          bits = SetBit(bits, i, 0);
    // Wenn jeder Spieler ein Ziel noch nicht erfüllt hat, aufhören
    if (!bits) return 1;
    // Spielziel erfüllt: Missionspasswort setzen
    if (curr_goal)
      if (curr_goal->LocalN("missionPassword"))
        GainMissionAccess(curr_goal->LocalN("missionPassword"));
  }
  // Kein Zielobjekt? Wir sind überflüssig :(
  if (!goal_count) 
    return(RemoveObject());
  // Die Verlierer eleminieren
  i = GetPlayerCount();
  while (i--) 
    if (!GetBit(bits, i))
      EliminatePlayer(GetPlayerByIndex(i));
  // Spiel zuende
  return 0;
}

// Setzt das Missionspasswort, welches von diesem Spielziel bei Erfüllung beim Spieler eingetragen wird.

public func SetMissionAccess(string strPassword)
{
  missionPassword = strPassword;  
}

// Basis-Implementationen - diese sollten in abgeleiteten Spielziel-Objekten überladen werden

public func IsFulfilled() { return 1; }

public func IsFulfilledforPlr(plr) { return IsFulfilled(); }

protected func Activate(iPlr)
{
  if (IsFulfilled()) return(MessageWindow("$MsgGoalFulfilled$",iPlr));
  return MessageWindow(GetDesc(),iPlr);
}
