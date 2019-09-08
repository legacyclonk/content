/*-- Der letzte Wille --*/

#strict

static g_iPlayers;

protected func Initialize()
{
  // Spielziel eirichten
  FindObject(SCRG)->SetMessages("$MsgGoalFulfilled$", "$MsgGoalUnfulfilled$");
  FindObject(SCRG)->SetMissionAccess("ReachedTheOcean");
  // Welt einrichten  
  SetSkyAdjust(RGBa(255,255,255,128),RGB(64,196,255));
  CastObjects(ROCK,2,7,344,304);
  CreateObject(WOOD,344,304,-1);
  CastObjects(WOOD,2,7,352,432);
  CreateObject(ROCK,352,432,-1);
  CreateObject(CNKT,112,464, -1);
  CreateObject(_LEI,230,224,-1);
  ProtectedCall(CreateConstruction(TWR2,1184,184,0,100,1,0), "ControlLeft");
  // Schloss unverwundbar
  var castle = CreateConstruction(CST3,1280,176,0,100,1,0);
  castle->LocalN("basement")->SetInvincible();
  CreateContents(CRYS,CreateConstruction(WTWR,1380,176,0,100,1,0));
  CreateObject(GIDL,1920,216,-1);
  CreateConstruction(HUT2,2192,392,0,100,1,0);
  CreateConstruction(CHEM,2245,392,0,100,1,0);
  var pObj;
  while(pObj = FindObject(0,2384,0,512,LandscapeHeight())) RemoveObject(pObj);
  ObjectSetAction(CreateObject(_LEI,3600,240,-1),"SeaSound");
  SetDir(DIR_Right(),CreateObject(MCLK,3620,100,-1));
  ScriptGo(1);
  return(1);
}

protected func InitializePlayer(iPlr)
{
  if(g_iPlayers == 0)
  {
    CreateStartStuff(0);
    BeamClonks(iPlr,180,224);
  }
  else
  {
    TakeOverClonk(iPlr);
  }
  g_iPlayers++;
}

public func TakeOverClonk(iPlr)
{
  var pOld = GetCrew(iPlr);
  var szName;
  SetColorDw(RGB(255,255), pOld);
  for(var pClonk in FindObjects(Find_Not(Find_Owner(iPlr)), Find_Not(Find_Owner(-1)), Find_OCF(OCF_CrewMember)))
  {
    if(GetCursor(GetOwner(pClonk)) == pClonk) continue;
	szName = GetName(pClonk);
	SetCrewStatus(GetOwner(pClonk), false, pClonk);
	SetOwner(iPlr, pClonk);
    SetCrewStatus(iPlr, true, pClonk);
	GrabObjectInfo(pOld, pClonk);
	SetName(szName, pClonk);
	RemoveObject(pOld);
	return 1;
  }
}

private func BeamClonks(iOwner, iToX, iToY)
{
  for(var pClonk in FindObjects(Find_ID(CLNK))) if(GetOwner(pClonk) == iOwner) SetPosition(iToX += 7, iToY, pClonk);
}
private func CreateStartStuff(iPlr)
{
  SetWealth(iPlr,10);
  SetName("Jeremy", GetCrew(iPlr,0));
  CreateContents(ROCK,GetCrew(iPlr,0));
  SetName("Samuel", GetCrew(iPlr,1));
  CreateContents(FLAG,GetCrew(iPlr,1));
  SetName("Edward", GetCrew(iPlr,2));
  CreateContents(WOOD,GetCrew(iPlr,2));
  return(1);
}
protected func ObjectJump(idID, cnt, iX, iY, iStrength, iPosOffs, iPosRand, iOwner)  
{
  while(cnt--) SetYDir(Random(iStrength) -iStrength, CreateObject(idID, iX + iPosOffs + Random(iPosRand), iY + iPosOffs + Random(iPosRand), iOwner));
  return(1);
}
protected func ObjectLine(idID, cnt, iX, iY, iMaxLength, iAngle, iOwner)
{                                                
  var iLength;
  while(cnt--) CreateObject(idID, iX + Cos(iAngle, iLength = Random(iMaxLength)), iY + Sin(iAngle, iLength), iOwner);
  return(1);
}

global func NewGoal(szText)
{
  for(var i = 0; i < GetPlayerCount(); i++)
    CustomMessage(szText, 0, GetPlayerByIndex(i), 90,70, 0xffffff, MD69, "Portrait:CLNK::ffccaa::3");
}

protected func Script1()
{
  NewGoal("$MsgGoal1$");
  return(1);
}
static pStoryClonk;
static pMage;
static fShowdown;
static pFarmer;
static pBalloonClonk;

protected func Script22()
{
  if(!(pStoryClonk = FindObject2(Find_ID(CLNK), Find_InRect(1050, 0, LandscapeWidth()-1050, LandscapeHeight())))) return goto(21);
  goto(252);
  StartSequence("Mage");
  ScriptGo(false);
}              

protected func Script252()
{
  if(ContentsCount(CRYS,FindObject(CST3)) >= 30) return(goto(300));
  for(var pClonk in FindObjects(Find_ID(CLNK), Find_InRect(1808, 0, LandscapeWidth()-1808, LandscapeHeight())))
  {
    Enter(FindObject(CST3),pClonk);
	for(var i = 0; i < GetPlayerCount(); i++)
	  CustomMessage(Format("<c %x>%s:</c> %s", RGB(0,0,255), GetName(0, SCLK), "$MsgErrorCrystals$"), 0, GetPlayerByIndex(i), 90,70, 0xffffff, MD69, Format("Portrait:%i::%x::%s", SCLK, RGB(0,0,255), "1"));
	Sound("Error");
  }
  goto(251);
}
protected func Script300()
{
  StartSequence("Crystal");
  ScriptGo(false);
  goto(393);
  return 1;
}

static pBalloon;
protected func Script394()
{
  if( pBalloon = FindObject2(Find_ID(BALN), Find_InRect(1808, 0, LandscapeWidth()-1808, LandscapeHeight())) ) return goto(450);
  if(FindObject2(Find_ID(CLNK), Find_InRect(2500, 0, LandscapeWidth()-2500, LandscapeHeight()))) return goto(550);
  goto(393);
}
protected func Script450()
{
  StartSequence("Balloon");
  ScriptGo(false);

  // Destroy more balloons
  AddEffect("IntBalloonCheck", 0, 1, 25);
  return 1;    
}

protected func Script482()
{
  if(FindObject2(Find_ID(CLNK), Find_InRect(2500, 0, LandscapeWidth()-2500, LandscapeHeight()))) return goto(550);
  goto(481);
}
protected func Script550()
{
  // Destroy more balloons
  AddEffect("IntBalloonCheck", 0, 1, 25);

  var pFarmer;
  ObjectSetAction(pFarmer = CreateObject(_LEI,2700,0,-1),"RumbleFall");
  SetYDir(100,pFarmer);
  var cnt = 4;
  while(cnt--) SetYDir(80+Random(16),CreateObject(SAND,2670+Random(51),0,-1));
  SetYDir(-10,CreateObject(EFLN,2695,0,-1));
  SetYDir(10,CreateObject(SFLN,2710,0,-1));
  return(1);
}
protected func Script572()
{
  var pClonk = GetCursor(GetPlayerByIndex(0));
  for(var i = 0; i < GetPlayerCount(); i++)
    CustomMessage(Format("<c %x>%s:</c> %s", GetColorDw(pClonk), GetName(pClonk), "$MsgVolcano$"), 0, GetPlayerByIndex(i), 90,70, 0xffffff, MD69, Format("Portrait:%i::%x::%s", GetID(pClonk), GetColorDw(pClonk), "1"));
  return(1);
}
protected func Script575()
{
  if(FindObject2(Find_ID(CLNK), Find_InRect(3040, 0, LandscapeWidth()-3040, LandscapeHeight()))) return goto(600);
  Smoke(2688,480,Random(41));
  return(1);
}
protected func Script577()
{
  goto(573);
}
protected func Script600()
{
  Music();
  return(1);
}
protected func Script603()
{
  var pClonk = GetCursor(GetPlayerByIndex(0));
  for(var i = 0; i < GetPlayerCount(); i++)
    CustomMessage(Format("<c %x>%s:</c> %s", GetColorDw(pClonk), GetName(pClonk), "$MsgSea$"), 0, GetPlayerByIndex(i), 90,70, 0xffffff, MD69, Format("Portrait:%i::%x::%s", GetID(pClonk), GetColorDw(pClonk), "1"));
  return(1);
}
protected func Script605()
{
  if(pStoryClonk = FindObject2(Find_ID(CLNK), Find_InRect(3540, 0, LandscapeWidth()-3540, LandscapeHeight()))) return goto(700);
  goto(604);
}
protected func Script700()
{
  StartSequence("Suicide");
  ScriptGo(false);
}

public func End()
{
  StartSequence("ShowDown");
}

global func FxIntBalloonCheckTimer()
{
  var balloon = FindObject2(Find_ID(BALN), Find_InRect(1808, 0, LandscapeWidth()-1808, LandscapeHeight()));
  if(balloon) DestroyBalloon(balloon);
}
