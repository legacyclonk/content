/* -- Golden Canyon -- */

#strict

static pWagon, pHorse;
static fJoinWagon;

func Initialize()
  {
  CreateObject(REPR);  // Nachwuchs

  var iX, iY, iDir;
  HutY(iX, iY, iDir);
  pWagon=CreateObject(COAC, iX, iY, -1);
  pHorse = CreateObject(HORS, iX-54+108*iDir, iY, -1);
  pHorse->~ConnectWagon(pWagon);
  SetCategory(1, FindObject(CHBM));
  SetDir(iDir, pHorse);
  SetDir(iDir, pWagon);
  for(var i=0;i<4;i++)
  CreateContents(BRDG, pWagon);
  for(var i=0;i<4;i++)
  CreateContents(FHMC, pWagon);
  for(var i=0;i<6;i++)
  CreateContents(MEAC, pWagon);
  for(var i=0;i<2;i++)
  CreateContents(BMAC, pWagon);
  for(var i=0;i<3;i++)
  CreateContents(WHKY, pWagon);
  for(var i=0;i<2;i++)
  CreateContents(IGNB, pWagon);
  for(var i=0;i<4;i++)
  CreateContents(DYNB, pWagon);
  for(var i=0;i<12;i++)
  CreateContents(METL, pWagon);
  for(var i=0;i<14;i++)
  CreateContents(WOOD, pWagon);
  for(var i=0;i<4;i++)
  CreateContents(CNKT, pWagon);
  fJoinWagon = 1;
  ScriptGo(1);
  return(1);
  }

func HutY(&iX, &iY, &iDir)
  {
  iX = 27; iDir = 1;
  if(Random(100)>50) { iX = LandscapeWidth() - 27; iDir=0; }
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
    SetPosition(GetX(pWagon)+RandomX(-8,8), GetY(pWagon)+15, GetCrew(iPlr,i));
    if(fJoinWagon) Enter(pWagon, GetCrew(iPlr,i));
    ++i;
    }
  var pTrapper = FindObjectOwner(TRPR, iPlr);
  if(pTrapper)
  {
    CreateContents(FIPL, pTrapper);
    CreateContents(KNFE, pTrapper);
    CreateContents(MLLT, pTrapper);
  }
  // Einer sollte den Wagen lenken
  if(!(pWagon->GetRider()))
  {
    pWagon->ContainedUp(GetHiRank(iPlr));
    if(GetX(pWagon)<50) AddEffect("HorseMoveTo", pHorse, 1, 1, 0, 0, 189, GetY(pWagon));
    else AddEffect("HorseMoveTo", pHorse, 1, 1, 0, 0, 858, GetY(pWagon));
//    if(GetX(pWagon)<50) SetCommand(pHorse, "MoveTo", 0, 162, GetY(pWagon));
//    else SetCommand(pHorse, "MoveTo", 0, 885, GetY(pWagon));
  }
  return(1);
  }

global func FxHorseMoveToStart(pHorse, iNumber, fTmp, iX, iY)
{
  if(fTmp) return();
  EffectVar(0, pHorse, iNumber) = iX;
  EffectVar(1, pHorse, iNumber) = iY;
  SetCommand(pHorse, "MoveTo", 0, iX, iY);
}

global func FxHorseMoveToTimer(pHorse, iNumber)
{
  // Kein Kommando mehr? -> abbrechen (z.B. Spieler übernimmt die Kontrolle)    
  if(!GetCommand(pHorse)) return(-1);
  // Angekommen? Anhalten
  if(Distance(GetX(pHorse), GetY(pHorse), EffectVar(0, pHorse, iNumber), EffectVar(1, pHorse, iNumber))<20)
  {
    pHorse->~SetGait();
    SetCommand(pHorse, "None");
    return(-1);
  }
}

protected func Script1()
{
  fJoinWagon=0;
  ScriptGo(1);
}

protected func Script2000()
{
  if(ObjectCount(BBRD)<3) CreateObject(BBRD, LandscapeWidth()/2+RandomX(-50,50), -10);
  goto(5);
}

protected func OnGoalsFulfilled()
{
  GainMissionAccess("WestGC");
  return(0);
}