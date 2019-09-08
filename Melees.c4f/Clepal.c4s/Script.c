#strict 2

static g_team_join_count;

func Initialize() {
  SetGamma(RGB(15,15,15),RGB(140,140,140),RGB(255,255,255));
  SetSkyParallax(0,14,14, 0,0,SkyPar_Keep,SkyPar_Keep);
  g_team_join_count = CreateArray(3);
}

func InitializePlayer(iPlr,x,y,bas,team) {
  var n = ++g_team_join_count[team];
  if (n<=2) {
    // Die ersten beiden Spieler kriegen 100 Gold. Danach nichts mehr.
    SetWealth(iPlr, 100);
  }
  // Erste Basis
       if (n == 1 && team == 1) CreateBase(352,371,iPlr);
  else if (n == 1 && team == 2) CreateBase(1000,266,iPlr);
  // Zweite Basis
  else if (n == 2 && team == 1) CreateBase(149,486,iPlr);
  else if (n == 2 && team == 2) CreateBase(1088,242,iPlr);
  //Keine Basen ab dem dritten Spieler in einem team
  else for(var i=0; i<GetCrewCount(iPlr);i++) { 
    if(team==1) SetPosition(RandomX(57,73),RandomX(322,328),GetCrew(iPlr,i));
    if(team==2) SetPosition(RandomX(1079,1085),RandomX(184,190),GetCrew(iPlr,i));
  }
  // Startmaterial
  if (n>2) CreateContents(FLAG,GetCrew(iPlr,0));
	CreateContents(_BMR,GetCrew(iPlr,1));
  return true;
}



func CreateBase(int x, int y,int iPlr) {
  //Huette
  var pBase=CreateObject(HUT3,x,y,iPlr);
  //Ausruestung
  CreateContents(FLAG,pBase);
  CreateContents(_BMR,pBase);    
  for(var i=3;i>0;i--) CreateContents(FLNT,pBase);
  //Clonks hineinversetzen
  for(var i; i<GetCrewCount(iPlr);i++) Enter(pBase,GetCrew(iPlr,i));
  return(1);
  }
 