/*-- Spielzielauswahl --*/

#strict

//C4D_Goal | C4D_Rule | C4D_Environment
static const Chooser_Cat = 524384;

/* Nur eine Mitteilung für Neugierige */

protected func Activate(iPlr)
{
  if(iPlr == iChoosingPlr) return(OpenMenu());
  MessageWindow(Format("$Choosing$", GetPlayerName(iChoosingPlr)),iPlr);
}

/* Initialisierung */

local aRules, pGoal, aGoals, Death, iDarkCount, aAI, iAliens;
local aTempGoalSave;
local iChoosingPlr;

protected func Initialize()
{
  SetPosition();
  aRules = CreateArray();
  aGoals = CreateArray();
  aTempGoalSave = CreateArray();
  aAI = CreateArray();
  // Spielziele bald entfernen
  ScheduleCall(this(), "RemoveGoals", 1);
  if(IsDark())
    SetDarkness(0);
  iDarkCount = 0;

	iAliens = 8;

	//Host identifizieren
  iChoosingPlr = -1;
  ScheduleCall(this, "ChoosePlayer", 15);
}

/* Host-Identifizierung */

private func ChoosePlayer()
{
  var lowest = [0x7FFFFFFF, -1];
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var plr = GetPlayerByIndex(i, C4PT_User);
    if(GetPlrClientNr(plr) < lowest[0])
      lowest = [GetPlrClientNr(plr), plr];
  }

  return iChoosingPlr = lowest[1];
}

public func RemovePlayer(int iPlr)
{
  //Neuen Host identifizieren und Menü öffnen
  if(iChoosingPlr == iPlr)
  {
    iChoosingPlr = -1;
    ScheduleCall(this, "ChoosePlayer", 15);
    ScheduleCall(this, "OpenMenu", 16);
  }

  return true;
}

protected func RemoveGoals()
{
  // Spielziele entfernen
  for(var goal in FindObjects(Find_Category(C4D_Goal)))
  {
    aGoals[GetLength(aGoals)] = GetID(goal);
    aTempGoalSave[GetLength(aTempGoalSave)] = GetWinScore(goal);
    goal->RemoveObject();
  }
	
  // Einmal doof hacken (wo wir das hier ja sonst schon nicht gemacht haben °-°)
  OpenGoalChooseMenu();
  // Menü wird einfach nochmal geöffnet, diesmal richtig!
}

/* Spielerinitialisierung */

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam, id idExtra)
{
  if(!aAI) aAI = CreateArray(); // Rückwärtskompatibilität
  
  if(GetPlayerType(iPlr) == C4PT_Script)
    for(var i=1 ; i < aAI[iTeam] ; i++)
      CreateClonk(iPlr);
  aAI[iTeam] = 0;
  if(Death) return();
  // Alle Clonks des Spielers verstauen
  for(var i=0, pCrew, tmp ; pCrew = GetCrew(iPlr, i) ; i++)
    {
    tmp = CreateObject(TIM1, GetX(pCrew), GetY(pCrew), -1);
    SetCategory(GetCategory(tmp) | C4D_Foreground,tmp);
    SetGraphics(0, tmp, GetID(pCrew), 1, 5, 0, 1, pCrew);
    Enter(tmp, pCrew);
    Eastern(tmp);
    }
  // Spieler 1? Dann Menü öffnen
  if(!iPlr)
  {
    Log("$ChoosingPlayer$", GetPlayerName(iPlr));
    return(OpenMenu());
  }
}

private func CreateClonk(int iPlr)
{
	/*
  var bot = CreateObject(HZCK, 0,0, iPlr);
  MakeCrewMember(bot, iPlr);
  //GameCallEx("RelaunchPlayer", iPlr, bot, -1);
  GameCall("RelaunchPlayer", iPlr, bot, -1);
  */
  AddBot(GetPlayerTeam(iPlr));
}

/* Hauptmenü */

protected func OpenMenu()
{
  if(GetLength(aGoals)) return(OpenGoalChooseMenu());

  var pClonk = GetCursor(iChoosingPlr);
  if(!pClonk) return(ScheduleCall(this(), "OpenMenu", 1));

  if(GetMenu(pClonk))
    CloseMenu(pClonk);

  Message("", pClonk);

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);
  // Spielregeln
  AddMenuItem("$CreateRules$", "OpenRuleMenu", CTFL, pClonk, 0,0, "$RuleInfo$");
  // Dunkelheit
  if(IsDark())
    AddMenuItem("%s", "OpenDarknessMenu", DARK, pClonk,0,0,"$DarknessChose$");
  // Spielziel
  if(GoalIsCompatible()) AddMenuItem("%s", "OpenGoalMenu", GetID(pGoal), pClonk,0,0,"$GoalChose$");
  // KI
  if(ObjectCount(WAYP) && !GetLeague()) AddMenuItem("$AIMenu$", "OpenAIMenu", HZCK, pClonk, 0,0, "$AIInfo$");
	// Aliens
	if(ObjectCount(ALAR)) AddMenuItem("$AlienMenu$", "OpenAlienMenu", ALAR, pClonk, 0,0, "$AlienInfo$");
  // Fertig
  AddMenuItem("$Finished$", "ConfigurationFinished", CHOS, pClonk,0,0,"$Finished$",2,3);
}

/* Regelmenü */

protected func OpenRuleMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosingPlr);
  // Menü aufmachen
  CreateMenu(GetID(), pClonk);
  // Regeln einfügen
  for(var i=0, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat) ; i++)
    if(DefinitionCall(idR, "IsChooseable") && !GetLength(FindObjects(Find_ID(idR))))
      {
      def = CreateObject(idR, 0,0, -1);
      if(aRules[i])
        {
        SetGraphics("Chosen", def, WPCH, 1, GFXOV_MODE_Picture);
        SetObjDrawTransform(650,0,5000,0,650,5000, def, 1);
        }
      AddMenuItem("%s", "ChangeRuleConf", idR, pClonk, 0, i, 0, 4, def);
      if(i == iSelection)
        check = true;
      if(!check)
        j++;
      RemoveObject(def);
      }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0,0,2,3);
  // Menüeintrag auswählen
  SelectMenuItem(j, pClonk);
}

protected func ChangeRuleConf(id dummy, int i)
{
  // Regel wurde ausgewählt
  if(!aRules[i])
    aRules[i] = true;
  // Regel wurde abgewählt
  else
    aRules[i] = false;
  // Geräusch
  Sound("Grab", 1,0,0,iChoosingPlr+1);
  // Regelmenü wieder aufmachen
  OpenRuleMenu(0, i);
}

/* Sondermenü: Dunkelheit */

protected func OpenDarknessMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosingPlr);
  // Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  // Anzeige
  AddMenuItem(" ", "OpenDarknessMenu", DARK, pClonk, iDarkCount, 0, " ");
  // Zähler erhöhen
  AddMenuItem("$MoreDarkness$", "ChangeDARKConf", CHOS, pClonk, 0, +1, "$MoreDarkness$",2,1);
  // Zähler senken
  AddMenuItem("$LessDarkness$", "ChangeDARKConf", CHOS, pClonk, 0, -1, "$LessDarkness$",2,2);
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0, "$Finished$",2,3);
  // Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

protected func ChangeDARKConf(id dummy, int iChange)
{
  // Stand verändern
  iDarkCount = BoundBy(iDarkCount+iChange, 0, 10);
  FadeDarkness(iDarkCount,100);
  // Geräusch
  Sound("Grab", 1,0,0,iChoosingPlr+1);
  // Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenDarknessMenu(0, iSel);
}

/* Spielzielmenü */

protected func OpenGoalChooseMenu()
{
  var pClonk = GetCursor(iChoosingPlr);
  if(!pClonk) return(ScheduleCall(this(), "OpenMenu", 1));
  if(!GetLength(aGoals)) return(ScheduleCall(this(), "OpenMenu", 1));

  CloseMenu(pClonk);

  if(GetLength(aGoals) == 1) {
    return(CreateGoal(aGoals[0], aTempGoalSave[0]));
  }

  CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);

  var i;
  for(var goal in aGoals)
  {
    AddMenuItem("%s", "CreateGoal", goal, pClonk, 0, aTempGoalSave[i]);
    i++;
  }
}

protected func CreateGoal(id idGoal, int iScore)
{
  // Spielziel erstellen
  var goal = CreateObject(idGoal, 0,0, -1);
  // Alten Wert setzen
  SetWinScore(iScore, goal);
  // Alle benachrichtigen
  Log("$Goal$", GetName(0, idGoal));
  // Array leeren, damit das Menü nicht nochmal kommt
  aGoals = CreateArray();

	GameCallEx("GoalChosen");
	
  // Normales Menü öffnen
  OpenMenu();
}

private func GoalIsCompatible()
{
  // Schon eines gefunden?
  if(pGoal) return(1);
  // Mehr als ein Spielziel wird nicht unterstützt
  if(GetLength(FindObjects(Find_Category(C4D_Goal()))) != 1) return();
  // Ist das Spielziel kompatibel mit diesem Objekt?
  if(!(FindObject2(Find_Category(C4D_Goal()))->~IsConfigurable())) return();
  // Gut
  pGoal = FindObject2(Find_Category(C4D_Goal()));
  return(1);
}

protected func OpenGoalMenu(id dummy, int iSelection)
{
  if(!pGoal) return(OpenMenu());
  var pClonk = GetCursor(iChoosingPlr);
  // Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  // Anzeige
  AddMenuItem(" ", "OpenGoalMenu", GetID(pGoal), pClonk, GetWinScore2(), 0, " ");

  var moreWinScore = "$MoreWinScore$";
  var lessWinScore = "$LessWinScore$";
  if(pGoal->~GetMoreScoreText()) moreWinScore = pGoal->~GetMoreScoreText();
  if(pGoal->~GetLessScoreText()) lessWinScore = pGoal->~GetLessScoreText();

  // Zähler erhöhen
  AddMenuItem(moreWinScore, "ChangeGoalConf", CHOS, pClonk, 0, +1, moreWinScore,2,1);
  // Zähler senken
  AddMenuItem(lessWinScore, "ChangeGoalConf", CHOS, pClonk, 0, -1, lessWinScore,2,2);
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0,"$Finished$",2,3);
  // Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

protected func ChangeGoalConf(id dummy, int iChange)
{
  // Stand verändern
  SetWinScore(GetWinScore(pGoal)+iChange,pGoal);
  if(GetWinScore(pGoal) <= 0) SetWinScore(1,pGoal);
  // Geräusch
  Sound("Grab", 1,0,0,iChoosingPlr+1);
  // Menü wieder öffnen
  var iSel = 1;
  if(iChange == -1) iSel = 2;
  OpenGoalMenu(0, iSel);
}

private func GetWinScore2()
{
  var pGoal = FindObject2(Find_Category(C4D_Goal()));
  return(GetWinScore(pGoal));
}

/* Sondermenü: KI */

protected func OpenAIMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor(iChoosingPlr);
  var pGoal = FindObject2(Find_Category(C4D_Goal()));
  // Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  // Kein Teamspielziel

  if(!(pGoal->~IsTeamGoal()))
  {
		var botCount = 0;
		if(aAI && aAI[0]) botCount = aAI[0];
    // Zahl
		AddMenuItem(Format("%d $Bots$ ",botCount), Format("OpenAIMenu(0,%d)",i*3-3), HZCK, pClonk, 0, 0, " ");
    AddMenuItem("$OneAIMore$", "AIConfig(0,+1,2)", CHOS, pClonk, 0,0, "$OneAIMore$",2,1);
    AddMenuItem("$OneAILess$", "AIConfig(0,-1,3)", CHOS, pClonk, 0,0, "$OneAILess$",2,2);
  }
  else // Teamspielziel
  {
    for(var i=1 ; GetTeamName(i) ; i++)
    {
			var botCount = 0;
			if(aAI && aAI[i]) botCount = aAI[i];
			
			AddMenuItem(Format("%s: %d $Bots$ ",GetTeamName(i),botCount), Format("OpenAIMenu(0,%d)",i*3-3), HZCK, pClonk, 0, 0, " ");
      AddMenuItem("$OneAIMore$", Format("AIConfig(%d,+1,%d)", i,i*3-2), CHOS, pClonk, 0,0, "$OneAIMore$",2,1);
      AddMenuItem("$OneAILess$", Format("AIConfig(%d,-1,%d)", i,i*3-1), CHOS, pClonk, 0,0, "$OneAILess$",2,2);
    }
    if(GetLength(aAI) > i)
      while(i < GetLength(aAI))
      {
				var botCount = 0;
				if(aAI && aAI[i]) botCount = aAI[i];
				
				AddMenuItem(Format("%s: %d $Bots$ ",Format("Team %d", i),botCount), Format("OpenAIMenu(0,%d)",i*3-3), HZCK, pClonk, 0, 0, " ");
        AddMenuItem("$OneAIMore$", Format("AIConfig(%d,+1,%d)", i,i*3-2), CHOS, pClonk, 0,0, "$OneAIMore$",2,1);
        AddMenuItem("$OneAILess$", Format("AIConfig(%d,-1,%d)", i,i*3-1), CHOS, pClonk, 0,0, "$OneAILess$",2,2);
        i++;
      }
    // Autoteams -> Neue Teams einstellbar
    if(GetTeamConfig(TEAM_AutoGenerateTeams))
      AddMenuItem("$NewTeam$", Format("AIConfig(%d,+1,%d)", i, i*3), HZCK, pClonk, 0,0, "$NewTeam$");
    // Faire Teams
    AddMenuItem("$FairTeams$", "FairTeams", MEL2, pClonk, 0,0, "$FairTeamsInfo$");
  }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0,"$Finished$",2,3);
  // Letzten Eintrag auswählen
  SelectMenuItem(iSelection, pClonk);
}

protected func AIConfig(int iTeam, int iChange, int iSel)
{
  // Kein Team
  if(iChange == +1) aAI[iTeam]++;
  if(iChange == -1 && iTeam <= GetLength(aAI))
  {
    if(aAI[iTeam] > 0)
      aAI[iTeam]--;
    if(iTeam == GetLength(aAI) && aAI[iTeam] <= 0)
    {
      for(var i=0, newArray = CreateArray() ; i < GetLength(aAI)-1 ; i++)
        newArray[i] = aAI[i];
      aAI = newArray;
    }
  }
  // Menü wieder öffnen
  OpenAIMenu(0, iSel);
}

protected func FairTeams()
{
  // Rausfinden, welches Team am stärksten ist
  var maxTeam = Max(GetTeamCount(), GetLength(aAI)-1)+1;
  for(var i=1,str,mstr ; i < maxTeam ; i++)
  {
    str = GetTeamStrength(i);
    if(str > mstr)
      mstr = str;
  }
  // Andere Teams auffüllen
  for(i=1 ; i < maxTeam ; i++)
    while(GetTeamStrength(i) < mstr)
      aAI[i]++;
  // Menü
  OpenAIMenu();
}

protected func GetTeamCount()
{
  var i=1;
  while(GetTeamName(i))
    i++;
  return(i-1);
}

private func GetTeamStrength(int iTeam)
{
  if(!iTeam) return();

  for(var i=0,j,str ; i < GetPlayerCount() ; i++)
  {
    j = GetPlayerByIndex(i);
    if(GetPlayerTeam(j) == iTeam)
    {
      // KI
      if(GetPlayerType(j) == C4PT_Script) str += GetCrewCount(j);
      // Normaler Spieler
      else str++;
    }
  }
  if(iTeam < GetLength(aAI)) str += aAI[iTeam]; 
  return(str);
}

/* Sondermenü: Aliens */

protected func OpenAlienMenu(id dummy, int iSelection)
{
  var pClonk = GetCursor();
  var pGoal = FindObject2(Find_Category(C4D_Goal));
  // Menü aufmachen
  CreateMenu(GetID(), pClonk, 0,0,0,0, 1);
  
  AddMenuItem(Format("$AlienInfo2$", iAliens), "OpenAlienMenu(0,0)", ALN2, pClonk);
  // Einer mehr!
  AddMenuItem("$OneAlienMore$", "AlienConfig(+1,1)", ALN1, pClonk, 0,0, "$OneAlienMore$");
  // Einer weniger!
  AddMenuItem("$OneAlienLess$", "AlienConfig(-1,2)", ALN2, pClonk, 0,0, "$OneAlienLess$");

  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0,"$Finished$",2,3);
  // Letzten Eintrag auswählen
  if(!iSelection)
  	iSelection = 1;
  SelectMenuItem(iSelection, pClonk);
}

protected func AlienConfig(int iChange, int iSel)
{
  iAliens += iChange;
  iAliens= BoundBy(iAliens,1,FindObject(ALAR)->GetMaxAliens());
  // Menü wieder öffnen (verzögert)
  OpenAlienMenu(0, iSel);
}

/* Konfiguration abschließen */

protected func ConfigurationFinished()
{
  // KI-Spieler erzeugen
  var i=0;
  for(var ai in aAI)
  {
    if(ai)
      AddAI(i);
    i++;
  }

	// Init Aliens
	if(ObjectCount(ALAR))
	{
		FindObject(ALAR)->Init(iAliens,5);
	}
	
  ScheduleCall(this(), "ConfigurationFinished2", 5);
}

protected func ConfigurationFinished2()
{
  Death = true;
  // Regeln erzeugen
  var i = 0, j, pCrew, tmp, log = "$Rules$", comma = "";
  for(var check in aRules)
  {
    if(check)
    {
      CreateObject(GetDefinition(i, Chooser_Cat), 10, 10, -1);
      log = Format("%s%s%s", log, comma, GetName(0, GetDefinition(i, Chooser_Cat)));
      comma = ", ";
    }
    i++;
  }
  // Dunkelheit erzeugen
  log = Format("%s%s%s x%d", log, comma, GetName(0, DARK), iDarkCount);
  Log(log);
  // ein schneller GameCall für Einstellungen
  GameCallEx("ChooserFinished");

  // Spieler freilassen
  for(i = 0 ; i < GetPlayerCount() ; i++)
  {
    for(j = 0 ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; j++)
    {
      tmp = Contained(pCrew);
      RemoveObject(tmp, 1);
      
      pCrew->~Recruitment(pCrew->GetOwner());
    }
    for(var rule in FindObjects(Find_Category(Chooser_Cat), Find_Exclude(this())))
      rule->~InitializePlayer(GetPlayerByIndex(i));
  }

  // Überschüssiges TIM1-Objekte entfernen (falls Spieler ziwschenzeitlich geflogen sind)
  for(tmp in FindObjects(Find_ID(TIM1)))
    if(!(tmp->Contents()))
      RemoveObject(tmp, 1);
  // Selber entfernen
  RemoveObject();
}

/* Generelles */

public func MenuQueryCancel() { return(1); }

private func Eastern(object P)
{
  var rand = Random(6);
  if(!rand) SetName(GetPlayerName(GetPlayerByIndex(Random(GetPlayerCount()))), P);
  if(rand == 1) SetName("$Emptiness$", P);
  if(rand == 2) SetName("$YourBrain$", P);
  if(rand == 3) SetName("$Nothing$", P);
  if(rand == 4) SetName("$Freezer$", P);
  if(rand == 5) SetName("$Space$", P);
}
