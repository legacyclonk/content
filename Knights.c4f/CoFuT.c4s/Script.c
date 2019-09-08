/*-- CoFuT --*/

#strict

static const CTEAM_Left  = 1,
             CTEAM_Right = 2;
              
static g_fGameOver;

protected func Initialize()
{
 // Das Spark-Objekt für Zufallsobjekte vom Himmel initialisieren
 RemoveAll(_SPK);
 CreateObject(_SPK, LandscapeWidth()/2 ,-5, -1)->SetAction("Wait");

 // Team-Melee Objekt erschaffen
 if (!ObjectCount(MELE)) CreateObject(MELE,50,50,-1);
 
 // Team-Heimatbasismaterial
 if (!ObjectCount(THBA)) CreateObject(THBA,50,50,-1);

 // Alle Luken freigraben
 var hatch;
 while (hatch = FindObject(CPHC, 0,0,0,0, 0, 0,0, 0, hatch))
 {
  hatch->Open();
  DigFreeRect(GetX(hatch)-8, GetY(hatch), 16, 12);
  hatch->Close();
 }

 // Virtuellen Nachrichtenreporter erstellen
 CreateObject(_MSG,0,0,-1)->SetAction("MainSaver");

 // Am Anfang weht kein Wind
 SetWind(0);

 // Am Anfang kommt ein Spark! ala will Rushduelle zur Mitte!
 CreateObject(_SPK,LandscapeWidth()/2+28,0,-1);

 // Lehm in der Burg
 var cpr2=FindObject(CPR2,296,207);
 for(var cnt=8;cnt;cnt--)CreateContents(LOAM,cpr2);
 var cpr2=FindObject(CPR2,1421,207);
 for(var cnt=8;cnt;cnt--)CreateContents(LOAM,cpr2);

 // Bäume neu platzieren
 PlaceTrees();

 // Scriptzähler starten
 return(ScriptGo(1));
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
 // Den Spieler ausrüsten und platzieren
 CreateContents(FLAG, GetCrew(player,1));
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player,iTeam);
 return(1);
}

protected func RelaunchPlayer(int player)
{
 // Den Tod des Spielers verkünden
 Log("$Relaunch$", GetPlayerName(player));
 //Noch als nachricht
 ShowMessage(Format("$Relaunch$",GetPlayerName(player)));
 Sound("Trumpet");

 // Dem Spieler zwei neue Ritter geben und diese ausrüsten
 for (var i; i < 2; i++)
 {
  var knight = CreateObject(KNIG, 0,0, player);
  MakeCrewMember(knight, player);
  DoEnergy(100-GetEnergy(knight), knight);
 }
 SetCursor(player, GetCrew(player,1));
 CreateContents(FLAG, GetCrew(player,1));

 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player);
 return(1);
}

protected func Script10()
{
 // Hat bereits Team gewonnen?
 if (ObjectCount(CPOF) == 2) return(goto(10));
 var winnerTeam = 0; // Default: Kein Gewinnerteam (Beide Kontore starben zugleich)
 if (FindObject(CPOF, 0,0, LandscapeWidth()/2,LandscapeHeight())) winnerTeam = CTEAM_Left; // Team links gewinnt
 else if (FindObject(CPOF, LandscapeWidth()/2,0, LandscapeWidth()/2,LandscapeHeight())) winnerTeam = CTEAM_Right; // Team rechts gewinnt
 
 // Nachricht
 ShowMessage("$GameOver$");
 // Wenn ja, den Sieg verkünden...
 Sound("Winning");

 // ...und die Verlierer eliminieren
 for (var i = GetPlayerCount(); i--;)
 {
  var iPlr = GetPlayerByIndex(i);
  if (GetPlayerTeam(iPlr) != winnerTeam)
   EliminatePlayer(iPlr);
 }
 
 // Das Spiel ist aus - Das Spielziel muss es nur noch merken
 g_fGameOver = true;
 
 // Für Einzelspieler gibt es kein MELE (Wer spielt CoFuT alleine? :o)
 if (!ObjectCount(MELE)) GameOver();
}

/* Clonk-Todesnachrichten */

public func OnClonkDeath(object pClonk, int iKiller)
{
  var iOwner = GetOwner(pClonk);
  if (!GetPlayerName(iOwner) || !pClonk) return();
  var sTypeName = GetName(0,GetID(pClonk));
  if (!sTypeName) return();
  
  if (iKiller==iOwner)
    ShowMessage(Format(RandomString("$Selfkill1$","$Selfkill2$","$Selfkill3$"),GetPlayerName(iOwner),sTypeName));
  else if(GetPlayerName(iKiller))
    {
    if (!Hostile(iOwner,iKiller))
      ShowMessage(Format(RandomString("$Teamkill1$","$Teamkill2$","$Teamkill3$"),GetPlayerName(iOwner),sTypeName,GetPlayerName(iKiller)));
    else
      ShowMessage(Format(RandomString("$KilledByPlayer1$","$KilledByPlayer2$","$KilledByPlayer3$"),GetPlayerName(iOwner),sTypeName,GetPlayerName(iKiller)));
    }
  else
    ShowMessage(Format(RandomString("$KilledByGaya1$","$KilledByGaya2$","$KilledByGaya3$"),GetPlayerName(iOwner),sTypeName));
}


/* Teamwechsel - aktiviert in Teams.txt (AllowTeamSwitch=1) */

public func RejectTeamSwitch()
  {
  // Kein Teamwechsel nach Spielende
  return (g_fGameOver);
  }

public func OnTeamSwitch(int iPlr1, int idNewTeam, int idOldTeam)
  {
  // Flaggen und Clonks aus der falschen Burg in die richtige Burg befördern
  var findPos, pTrgOff;
  if (idOldTeam == CTEAM_Left)
    findPos = Find_InRect(0,50, 400,200);
  else
    findPos = Find_InRect(1035,45, 405,205);
  pTrgOff = FindObject2(Find_ID(CPOF), Find_Not(findPos));
  if (pTrgOff)
    for (var obj in FindObjects(findPos, Find_Owner(iPlr1), Find_Or(Find_OCF(OCF_Alive), Find_ID(FLAG))))
      {
      if (Contained(obj)) Exit(obj);
      SetPosition(GetX(pTrgOff), GetY(pTrgOff), obj);
      if (GetID(obj) == FLAG) ObjectSetAction(obj, "Idle");
      }
  // Nachricht
  ShowMessage(Format("$MsgTeamChange$",GetPlayerName(iPlr1),GetTeamName(idNewTeam)));
  Sound("Ding");
  }

protected func PlaceTrees()
{
  var i, id = [TRE1, TRE2, TRE3];
  RemoveAll(BUSH);
  for (i = 0; i < GetLength(id); i++)
    RemoveAll(id[i]);
  for (i = 0; i < 10; i++)
    PlaceVegetation(id[Random(GetLength(id))], 400, 0, 640, 800, -1);
}
