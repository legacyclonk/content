/*-- Falling Stars Peak --*/

#strict

global func & Lifes(int iPlr)   { return (Global(iPlr+10)); }
global func & Bandits(int iIndex) { return (Global(Min(iIndex, 10))); }

static iBandits;

func Initialize() {
   
 // Regeln und Ziel   
  CreateObject(_ETG, 100, 100, -1); // Löscher
  CreateObject(REAC, 100, 100, -1); // Entlassen
                
 // Fahrstühle aufbauen und Schächte graben 
 var ELEV1 = CreateConstruction(ELEV, 123, 351, NO_OWNER, 100, 1);
 var ELEV2 = CreateConstruction(ELEV, 498, 242, NO_OWNER, 100, 1);
 var ELEV3 = CreateConstruction(ELEV, 899, 361, NO_OWNER, 100, 1);
 while(SetVar(0, FindObject(ELEV, 104, 292, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 30);
 while(SetVar(0, FindObject(ELEV, 484, 184, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 70);
 while(SetVar(0, FindObject(ELEV, 880, 304, 50, 70, 0, 0, 0, 0, Var())))
    ObjectCall(Var(), "CreateShaft", 30);

 AddEffect("StarFall", 0, 10, 1);

 SpawnBandit();
 iLevel = 1;
}

global func SpawnBandit()
{
  var pBNDT = CreateObject(BNDT, 0, 0, -1);
  var pCave, iX, iY;
  GetBanditPosition(Random(6), iX, iY, pCave);
  Enter(pCave, pBNDT);
  SetCommand(pBNDT, "MoveTo", 0, iX, iY);
  AddCommand(pBNDT, "Exit");
  iBandits++;
  UpdateBandits();
  ScheduleCall(pBNDT, "SetAI", 35, 1, "Bandit", 3);
}

global func GetBanditPosition( iIndex, &x, &y, &pCave)
{
  if(Bandits(iIndex))
  {
    for(var i; i < 8; i++)
    {
      iIndex = Mod(iIndex+1, 7);
      if(!Bandits(iIndex)) break;
    }
  }
  Bandits(iIndex)=1;
  if(iIndex==0)
  {
    pCave = FindObject(CAVE, 18-GetX(), 155-GetY());
    x = 48;
    y = 158;
    return();  
  }
  if(iIndex==1)
  {
    pCave = FindObject(CAVE, 20-GetX(), 495-GetY());
    x = 65;
    y = 503;
    return();  
  }
  if(iIndex==2)
  {
    pCave = FindObject(CAVE, 582-GetX(), 574-GetY());
    x = 582;
    y = 578;
    return();  
  }
  if(iIndex==3)
  {
    pCave = FindObject(CAVE, 489-GetX(), 135-GetY());
    { x = 559;   y = 139; }
    return();  
  }
  if(iIndex==4)
  {
    pCave = FindObject(CAVE, 489-GetX(), 135-GetY());
    { x = 443; y = 137 ;}
    return();  
  }
  if(iIndex==5)
  {
    pCave = FindObject(CAVE, 980-GetX(), 155-GetY());
    x = 939;
    y = 160;
    return();  
  }
  if(iIndex==6)
  {
    pCave = FindObject(CAVE, 977-GetX(), 484-GetY());
    x = 940;
    y = 489;
    return();  
  }
}

global func GetRandomCave()
{
  // Liste mit allen Höhlen machen	
  var obj, count;
  while(obj = FindObject(CAVE, 0, 0, 0, 0, 0, 0, 0, 0, obj)) 
     { Var(count++) = obj; }
  // Zufällig eine auswählen
  return(Var(Random(count)));
}

global func FxStarFallTimer()
{
  if(Random(10)) return();

  if((iLevel * 100)<=EffectVar(5, 0, GetEffect("Counting")) && iBandits)
  {
    if(!Random(4))  CreateObject(_STL, Random(LandscapeWidth()), 0);
  }
  else
  {
  if(!Random(4))  CreateObject(_STY, Random(LandscapeWidth()), 0);
  if(!Random(8))  CreateObject(_STG, Random(LandscapeWidth()), 0);
  if(!Random(12)) CreateObject(_STB, Random(LandscapeWidth()), 0);
  if(!Random(14)) CreateObject(_STP, Random(LandscapeWidth()), 0);
  if(!Random(15)) CreateObject(_STR, Random(LandscapeWidth()), 0);
  if(!Random(16)) CreateObject(_STA, Random(LandscapeWidth()), 0);
  }

  if(!Random(10)) CreateObject(DYNA, Random(LandscapeWidth()), 0);
  if(!Random(15)) CreateObject(AMBO, Random(LandscapeWidth()), 0);
  if(!Random(20)) CreateObject(WHKY, Random(LandscapeWidth()), 0);
  if(!Random(25)) CreateObject(DYNB, Random(LandscapeWidth()), 0);
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
  // Den Spieler ausrüsten und platzieren
  CreateContents(AMBO, GetHiRank(player));
  CreateContents(AMBO, GetHiRank(player));
  CreateContents(REVR, GetHiRank(player));
  CreateContents(WINC, GetHiRank(player));
  CreateObject(_CTD, 0, 0, player);
  Enter(FindObject(SALO), GetHiRank(player));
  SetCommand(GetHiRank(player), "Exit");
  Lifes(player) = 3;
  FindObjectOwner(_CTD, player)->~UpdateLifes();
  var obj, i;
  return(1);
}

public func RelaunchPlayer(int iPlr, int iDeathCausedBy)
  {
  // Kein Relaunch für eliminierte Spieler
  if(!GetPlayerName(Par())) return();
  
  // Keine Leben mehr?
  if(!Lifes(iPlr)) return();

  // Spieler neu beitreten lassen
  var pNewClonk = CreateObject(COWB, 0,0, iPlr);
  MakeCrewMember(pNewClonk, iPlr);
  SetCursor(iPlr,  pNewClonk);
  SetPlrView(iPlr, pNewClonk);
  CreateContents(AMBO, pNewClonk);
  CreateContents(AMBO, pNewClonk);
  CreateContents(REVR, pNewClonk);
  CreateContents(WINC, pNewClonk);

  Enter(FindObject(SALO), pNewClonk);
  SetCommand(pNewClonk, "Exit");

  Lifes(iPlr)--;
  DoTime(30*3, iPlr);
  FindObjectOwner(_CTD, iPlr)->~UpdateLifes();
  }

global func UpdateBandits()
{
  for(var i=0;i < GetPlayerCount(); i++)
  FindObjectOwner(_CTD, GetPlayerByIndex(i))->~UpdateBandits();
}

static iLevel;

global func GetFallSpeed() { return(15+2*iLevel); }

global func DoTime(iAmount, iPlr)
{
  EffectVar(0, 0, GetEffect("Counting")) = Min(3*60,EffectVar(0, 0, GetEffect("Counting"))+iAmount);
}

global func DoPoints(iAmount, iPlr)
{
  if((iLevel * 100)>EffectVar(5, 0, GetEffect("Counting"))) EffectVar(5, 0, GetEffect("Counting")) += iAmount;
  if((iLevel * 100)<=EffectVar(5, 0, GetEffect("Counting")) && !iBandits)
  {
    iLevel++;
    for(var i; i < 8; i++)
      Bandits(i) = 0;
    for(var i=0; i < Min((iLevel+1)/2, 8); i++)
      SpawnBandit();
    if(!Mod(iLevel, 5)) CreateObject(MEGG, LandscapeWidth()/2, 0);
  }
}

global func DoLifes(iAmount, iPlr)
{
  if((Lifes(iPlr)+iAmount)<=5)
  {
    Lifes(iPlr)+=iAmount;
    FindObjectOwner(_CTD, iPlr)->~UpdateLifes();
  }	  
  else DoPoints(iAmount, iPlr);
}


func GetMaxTime() { return(1*60); }

func TimeUp() { ~FindObject(COWB)->~Kill(); }// return(GameOver()); }
