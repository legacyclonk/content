#strict

static pHut;

func Initialize()
  {
  var iX, iY;
  HutY(iX, iY);
  pHut=CreateConstruction(HUT2, iX, iY, -1, 100, 1);
  ScriptGo(1);
  return(1);
  }

func HutY(&iX, &iY)
  {
  iX = 30 + Random(LandscapeWidth() - 80);
  while(Inside(iX, LandscapeWidth()/2-150, LandscapeWidth()/2+150)) 
    iX = 30 + Random(LandscapeWidth() - 80);
  iY=0;
  while(!GBackSolid(iX, iY))
    {
    ++iY;
    }
  return(1);
  }

func InitializePlayer(iPlr)
  {
  var i;
  while(GetCrew(iPlr,i))
    {
    SetPosition(GetX(pHut)+RandomX(-8,8), GetY(pHut)+15, GetCrew(iPlr,i));
    ++i;
    }
  CreateContents(FLAG, GetCrew(iPlr, 0));
  CreateContents(CNKT, GetCrew(iPlr, 1));
  for(i=3; i>0; --i) CreateContents(WOOD, pHut);
  for(i=5; i>0; --i) CreateContents(ROCK, pHut);
  return(1);
  }
func Script1000() { CreateClouds(); }

func Script3000() { RemoveClouds(); goto(1); }


func CreateClouds()
  {
  for(var i=RandomX(4,8); i>0; --i)
    {
    var pRain=CreateObject(FXP1,LandscapeWidth()/2,-50,-1);
    pRain->~Activate(Material("Water"), LandscapeWidth(), 20);
    }
  return(1);
  }

func RemoveClouds()
  {
  return(RemoveAll(FXP1));
  }