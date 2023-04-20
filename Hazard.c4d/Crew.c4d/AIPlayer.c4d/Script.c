/*-- KI --*/

#strict

// Variablen
local plrID; // Team!

/* Globale Funktionen */

// Bisher nicht benutzt
global func IsAlienPlayer(int iPlr)
{
  if(GetPlayerType(iPlr) == C4PT_Script)
    if(!GetCrew(iPlr))
      return(true);
}

/* Steuerungsfunktionen */

// Fügt eine neue KI ins Spiel ein
global func AddAI(int iTeam)
{
  var ai = CreateObject(__AI, AbsX(0),AbsY(0), -1);
  // ID zusammensetzen
  var newID = Format("%d", ObjectCount(__AI)), oneID;
  while(GetLength(newID) < 4)
    newID = Format("0%s", newID);
  oneID = C4Id(newID);
  ai->Init(oneID);

  var name = Format("$Team$", iTeam);
  if(!iTeam) name = Format("$NoTeam$", GetPlayerCount(C4PT_Script));

  var color = HSL(Random(16)*16,200+Random(56),100+Random(50));
  if(GetTeamColor(iTeam)) color = GetTeamColor(iTeam);

  CreateScriptPlayer(name, color, iTeam, 0, oneID);

  return(ai);
}

// Nicht benutzt
global func AddAlienAI(int iTeam)
{
  var name = Format("$Team$", iTeam);
  if(!iTeam) name = Format("$NoTeam$", GetPlayerCount(C4PT_Script));

  CreateScriptPlayer(name, HSL(Random(256),200+Random(56),100+Random(220)), iTeam, CSPF_NoScenarioInit | CSPF_NoEliminationCheck, __AI);
  return(true);
}

// Entfernt eine KI aus dem Spiel
global func RemoveAI(int iTeam)
{
  // Spezielles Team
  if(iTeam)
  {
    for(var i=0 ; i < GetPlayerCount() ; i++)
      if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
        if(FindObjectOwner(__AI, GetPlayerByIndex(i)))
          return(EliminatePlayer(GetPlayerByIndex(i)));
  }
  else
  {
    // Wir suchen den Spieler, mit der höchsten Spielernummer
    for(var i=0 ; i < GetPlayerCount() ; i++)
      if(GetPlayerType(GetPlayerByIndex(i)) == C4PT_Script)
        if(FindObjectOwner(__AI, GetPlayerByIndex(i)))
          return(EliminatePlayer(GetPlayerByIndex(i)));
  }
}

// Fügt einen neuen Clonk zum angegeben Team hinzu
global func AddBot(int iTeam)
{
  // Spieler suchen
  for(var i=0, found = -1 ; i < GetPlayerCount(C4PT_Script) ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i, C4PT_Script)) == iTeam)
    {
      found = GetPlayerByIndex(i, C4PT_Script);
      break;
    }
  // Spieler vorhanden?
  if(found == -1) return(AddAI(iTeam));
  // Clonk erstellen
  var bot = CreateObject(HZCK, 0,0, found);
  MakeCrewMember(bot, found);
  GameCall("RelaunchPlayer", found, bot, -1);
  return(bot);
}

// Weg mit dem Bot!
global func RemoveBot(int iTeam)
{
  // Spieler suchen
  for(var i=0, found = -1 ; i < GetPlayerCount(C4PT_Script) ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i, C4PT_Script)) == iTeam)
    {
      found = GetPlayerByIndex(i, C4PT_Script);
      break;
    }
  // Spieler vorhanden?
  if(found == -1) return();
  // Clonk aus der Crew entfernen
  var bot = GetCrew(found);
  if(bot)
  {
  	if(bot->Contained())
  	{
    	if(bot->Contained()->GetID() == TIM1 || bot->Contained()->GetID() == TIM2)
	      bot->Contained()->RemoveObject();
	  }
    else
      bot->RemoveObject();
  }
}

/* Initialisierungen */

protected func Initialize()
{
  // Position
  SetPosition();
  // Kategorie wechseln, damit GameCallEx ankommt
  SetCategory(GetCategory() | C4D_Rule);
}

public func Init(id oneID) // Initialisierung - für die richtige Zuweisung zum Spieler
{
  plrID = oneID;
}

public func GetPlrID() { return(plrID); }

public func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam, id idExtra)
{
  // Richtiger Spieler?
  if(idExtra != plrID) return();
  // Hat schon ein Kontrollobjekt?
  if(FindObjectOwner(GetID(), iPlr)) return(RemoveObject());
  // Ich übernehme
  SetOwner(iPlr);
  SetCategory(C4D_StaticBack);
}

/* Sonstiges */

protected func OnOwnerRemoved() { RemoveObject(); }

/* Verhalten */

public func Activity()
{
  // Kein Besitzer?
  if(GetOwner() == NO_OWNER) return;
  // Spielziel ermitteln (mehrere Spielziele gehen nicht!)
  var goal = GetGoal();
  // Operate according mission objectives!
  for(var i=0, pCrew ; pCrew = GetCrew(GetOwner(), i) ; i++)
  {
    if(pCrew->Contained())
    {
      // Clonk steckt in TIM2?
      if(pCrew->Contained()->GetID() == TIM2) pCrew->Contained()->ContainedLeft();
    }
    if(pCrew->Contained())
    {
      // Clonk steckt in TIM1?
      if(pCrew->Contained()->GetID() == TIM1)
      {
        // Waffenwahl aktiviert? (Achtung: Hardcode!)
        var rule = FindObject(WPCH);
        if(rule)
        {
          // Warenangebot abfragen
          var aWares = rule->GetBuyableWaresFor("IsWeapon",GetOwner());
          // Irgendwas rauspicken (0-3 Sachen)
          var buyCount = BoundBy(Random(4), 0, GetLength(aWares));
          for(var i=0 ; i < buyCount ; i++)
          {
            var randomWp = Random(GetLength(aWares));
            var weapon = aWares[randomWp];
            if(weapon && weapon != PIWP)
            {
              rule->DoBuyItem(weapon, GetOwner(pCrew), pCrew);
              aWares[randomWp] = 0;
            }
          }
          // Menü weg
          rule->Finish(0, pCrew);
        }
      }
    }
  }
  // spezielle Taktik...
  if(goal)
    if(!(goal->~AITactic(this()))) {
      for(var i=0, pCrew=0 ; pCrew = GetCrew(GetOwner(), i) ; i++) {
        // Fallback
        TacticDefault(pCrew);
      }
  }
}

private func GetGoal() { return(FindObject2(Find_Category(C4D_Goal))); }

/* Taktiken */

public func TacticDefault(object pCrew) {
  if(pCrew->Contained()) return(true);
  // Deathmatch hat keine besondere Taktik, wir suchen einen Wegpunkt und laufen hin
  if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
    // Clonk hat nichts zu tun -> das müssen wir ändern
    pCrew->SetMacroCommand(this(), "MoveTo", GetRandomWp(),0,0,0,Aggro_Follow);
  // Standardinventarbehandlung
  pCrew->CheckInventory();
  return(true);
}


// 'beyond' is interceptable because 'obj' is between 'beyond' and its target 'targ'
public func Interceptor(obj, targ, beyond)
{
  // a clonk counts as an interceptor if the angle to flagbase and carrier are more than 90°
  var angle = 0;
  var a1 = Angle(GetX(obj),GetY(obj),GetX(targ),GetY(targ));
  var a2 = Angle(GetX(obj),GetY(obj),GetX(beyond),GetY(beyond));
  angle = Abs(Normalize(a1-a2,0));
  if(angle > 90) return(1);
  return(0);
}

// Follow a target
public func Follow(object pCrew, object target, int maxdist, string text) {
  if(pCrew->GetMacroCommand(0, 1) == target || pCrew->GetMacroCommand(1, 1) == target) return();
  if(ObjectDistance(pCrew,target) > maxdist) {
    pCrew->SetMacroCommand(this(), "Follow", target, 0,0,0, Aggro_Follow);
    pCrew->DebugMessage(Format("@%s", text),"tactics");
  }
}

// Guard
public func Guard(pCrew, text) {
  if(pCrew->GetAggroLevel() != Aggro_Guard) {
    pCrew->SetMacroCommand(this(), "None", 0,0,0,0,Aggro_Guard);
    pCrew->DebugMessage(Format("@%s", text), "tactics");
  }
}

// Run to a target
public func Run(pCrew, target, text) {
  if((pCrew->GetMacroCommand(0,1)) != target) {
    pCrew->SetMacroCommand(this(), "MoveTo", target,0,0,0,Aggro_Shoot);
    pCrew->DebugMessage(Format("@%s", text), "tactics");
  }
}


/* Helferlein */

// Zufälligen Wegpunkt suchen
private func GetRandomWp()
{
  var wps = FindObjects(Find_ID(WAYP));
  return(wps[Random(GetLength(wps))]);
}
