/*-- Turnier --*/

#strict

protected func Initialize()
{
  ScriptGo(1);
  // Zugbrücken öffnen
  ProtectedCall(FindObject(CPBL),"ControlDown");
  ProtectedCall(FindObject(CPBR),"ControlDown");
  //Eingänge schließen
  var pLeft=FindObject(CPT3); if(pLeft) pLeft->SetEntrance(0);
  var pRight=FindObject(CPT4); if(pRight) pRight->SetEntrance(0);
  return(1);
}
  
protected func Script20()
{
  Sound("Fanfare");
  ScriptGo(0);
  return(1);  
}

protected func InitializePlayer(iPlayer)
{
 Relaunches(iPlayer)=1;
  var plr=iPlayer;
  var crew=GetCrew(plr);
  var right=false;
  // Position
  if(GetPlayerTeam(plr)==2) right = true;
  
  /*
  var x = 20 + 300 * (plr / 2); 
  if (right) x = LandscapeWidth() - x;
  var y = 230; */
  
  var x=GetX(crew);
  var y=GetY(crew);
  
  // Ausrüstung
  var horse=CreateObject(HRSK,x,y,plr);
  if (!right) SetDir(DIR_Right(),horse);
  ObjectSetAction(crew,"Ride",horse);
  var lance;
  ObjectSetAction(lance = CreateObject(LNCA,0,0,plr),"Lance",crew);
  if(right) SetR(330, lance); else SetR(30, lance);
  ProtectedCall(CreateContents(SHIE,crew), "Activate", crew);
  CreateContents(SWOR,crew);  
  if(right)
    SetCommand(horse,"MoveTo",0,LandscapeWidth()/4,y);
  else
    SetCommand(horse,"MoveTo",0,LandscapeWidth()*3/4,y);
  //crew->SetArmored(1, ARMR); - looks cool, but makes gameplay horrible...

  InitializeScoreboard(iPlayer);	  
  
  // Flaggen
  var banr;
  while (banr=FindObject(BANR,x-400,-200,800,400,0,0,0,0,banr))
    SetOwner(plr,banr);

  // Melee vorhanden?
  if (!ObjectCount(MELE)) CreateObject(MELE,0,0,-1);

  return(1);
}

func RelaunchPlayer(int iPlr, int iKilledBy) {
 if(Hostile(iPlr,iKilledBy) && iKilledBy != -1) GiveKill(iKilledBy);
 
 if(Relaunches(iPlr)-- > 0) {
   //Relaunch mit Effekt
   var iX=LandscapeWidth()/2+RandomX(-50,50);
   var iY=100;
   while(!GBackSemiSolid(iX, iY)) ++iY;  
   PlacementEffects(iX,iY);	
   Sound("PlayerJoin");
   var pObj = CreateObject(KNIG,iX,iY,iPlr); 
   SetOwner(iPlr,pObj);
   MakeCrewMember(pObj, iPlr);
   pObj->DoEnergy(100);
   SetCursor(iPlr, pObj);
   AddEffect("Spawn",pObj,100,1);
   
   //Ausrüstung
   ProtectedCall(CreateContents(SHIE,pObj), "Activate", pObj);
   var lance;
   ObjectSetAction(lance = CreateObject(LNCA,0,0,iPlr),"Lance",pObj);
   SetR(RandomX(30,330),lance);
   CreateContents(SWOR,pObj);   
   CreateContents(ROCK,pObj);   
  
   if(iKilledBy != iPlr && iKilledBy != -1) Log(Format("$DeathKilled$",GetPlayerName(iPlr),GetPlayerName(iKilledBy))); 
   else Log(Format("$Death$",GetPlayerName(iPlr)));   
   }
 else {
  if(iKilledBy != iPlr && iKilledBy != -1) Log(Format("$DeathPermanentKilled$",GetPlayerName(iPlr),GetPlayerName(iKilledBy))); 
  else Log("$DeathPermanent$");  
  }
 UpdateScoreboard(iPlr, true);    
 return(1);
 }

func RemovePlayer(iPlr) {
 // Relaunchzähler im Scoreboard ausleeren
 var iPlrID = GetPlayerID(iPlr), iColID = ScoreboardCol(KNIG);
 SetScoreboardData(iPlrID,       iColID,       "{{SKUL}}", -1);
 SortScoreboard(iColID, true);
 } 
 
global func PlacementEffects(x, y, color1, color2) 
{
  CastParticles("PxSpark", 50, 100, x, y, 5*8, 5*50, RGB(196, 196, 196), RGB(255, 255, 255));

  for (var i = 0; i < 5; i++)
    DrawParticleLine("PSpark", x-Random(400)+200,0, x,y, 8, 150 + 100 * (Random(3) == 0), RGBa(255,50+Random(100),50+Random(100),50), RGBa(50+Random(100),255,50+Random(100),100));
  
  return(1);
}