/*-- Königsmord --*/

#strict

protected func Initialize()
{
 // Ziel und Regeln erschaffen
 if (!FindObject(MELE)) CreateObject(MELE);  //Teammelee
 if (!FindObject(REAC)) CreateObject(REAC);  //Clonks entlassen
 if (!FindObject(REPR)) CreateObject(REPR);  //Nachwuchs
 //Aufzüge bauen und Schächte graben
 var ELEV1 = CreateConstruction(ELEV, 30, 298, NO_OWNER, 100, 1);
 var ELEV2 = CreateConstruction(ELEV, 371, 299, NO_OWNER, 100, 1);
 var ELEV3 = CreateConstruction(ELEV, 1811, 207, NO_OWNER, 100, 1);
 while(SetVar(0, FindObject(ELEV, 10, 233, 40, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 215);
 while(SetVar(0, FindObject(ELEV, 351, 238, 40, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 160);
 while(SetVar(0, FindObject(ELEV, 1792, 148, 40, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 215);
 while(SetVar(0, FindObject(CPEL, 1374, 289, 0, 0, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 90);
 //Kraftfelder am Durchgang erstellen
 CreateObject(_FFW, 2034, 640, NO_OWNER);
 CreateObject(_FFB, 2066, 481, NO_OWNER);
 
 // Scriptzähler starten
 return(ScriptGo(1));  
       
 return(1);
}


protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
 // Den Spieler ausrüsten und platzieren
 SetOwner(player, CreateContents(FLAG, GetCrew(player,1)));
 SetOwner(player, CreateObject(_CTD, 0, 0, player));
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player, iTeam);
 return(1);
}


 protected func RemovePlayer(int player)
{
 // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben klappt
 Global(player) = 0;
 return(1);
}


protected func RelaunchPlayer(int player)
{
 // Den Tod des Spielers verkünden
 Log("$MsgRelaunch$", GetPlayerName(player));
 Sound("Trumpet");

 // Dem Spieler drei neue Ritter geben und diese ausrüsten
 for (var i; i < 3; i++)
 {
  var knight = CreateObject(KNIG, 0,0, player);
  MakeCrewMember(knight, player);
  DoEnergy(100-GetEnergy(knight), knight);
 }
 SetCursor(player, GetCrew(player,1));
 SetOwner(player, CreateContents(FLAG, GetCrew(player,1)));

 // Jetzt muss der Spieler noch etwas warten
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->RelaunchPlayer(player);
 return(1);
}

protected func Script10()
{
 if(!FindObject(KING)) goto(9);
}

protected func Script14()
{
 if (FindObject(KING, 2055, 500, 45, 100)) 
   return(Team1Lose());
 if (!GetAlive(FindObject(KING))) 
   return(Team2Lose());
 goto(13);
}

protected func Team1Lose()
{
   Log("$MsgKingWin$");
   var winnerTeam = 2; 
// ...und die Verlierer eliminieren
  for (var i = GetPlayerCount(); i--;)
   {
  if (GetPlayerTeam(GetPlayerByIndex(i)) != winnerTeam)
  EliminatePlayer(GetPlayerByIndex(i));
   }
}

protected func Team2Lose()
{
   Log("$MsgKingLost$");
   var winnerTeam = 1; 
// ...und die Verlierer eliminieren
  for (var i = GetPlayerCount(); i--;)
   {
  if (GetPlayerTeam(GetPlayerByIndex(i)) != winnerTeam)
  EliminatePlayer(GetPlayerByIndex(i));
   }
}

protected func TimeUp()
{
  var i=0;
  var obj;
  while(obj = FindObject(_FWS, 2010, 460, 90, 140, 0, 0, 0, 0, obj)) obj->Destroy();
  while(obj = FindObject(_FBS, 2010, 460, 90, 140, 0, 0, 0, 0, obj)) obj->Destroy();
  CastParticles("MSpark", 10, 50, 2033, 538, 100, 200, RGBa(100,100,255,128), RGBa(0,0,255,0));
  CastParticles("MSpark", 10, 50, 2068, 479, 100, 200, RGBa(100,100,255,128), RGBa(0,0,255,0));
  Sound("DeEnergize");
  Log("$MsgWayFree$");
}

// Callbacks aus dem PlayerObject
func JoinTeam(iPlr, iTeam)
{
  // Die Schattenklingen mir Rüstungen ausstatten.
  if (iTeam==1)
  {
    CreateContents(BRMR,GetHiRank(iPlr));
  }
}

func GetTeamStartPosition(iTeam, fY)
{
  if(iTeam==1) { if(fY) return(288); return(RandomX( 142, 261)); }
  if(iTeam==2) { if(fY) return(323); return(RandomX(1212,1341)); }
}

func InitializeTeam(iTeam, iPlr)
{
  // Der 1. Spieler der Königsgarde erhält den König und die Schattenklingen mehr Geld.
  if (iTeam==2)
  {
    var pKing=CreateObject(KING,AbsX(1290),AbsY(265),iPlr);
    MakeCrewMember(pKing,iPlr);
    DoEnergy(100-GetEnergy(pKing),pKing);
    AcquireBase(iPlr, 975,20,600,330);
  }
  else
  {
    var pKnight=CreateObject(KNIG,AbsX(1984),AbsY(164),iPlr);
    CreateContents(SWOR,pKnight);
    CreateContents(SHIE,pKnight);
    MakeCrewMember(pKnight,iPlr);
    DoEnergy(100-GetEnergy(pKnight),pKnight);
    AcquireBase(iPlr, 5,80,395,240); 
    AcquireBase(iPlr, 1785,10,310,330);
    SetWealth(iPlr, GetWealth(iPlr)+150);
  }
}
