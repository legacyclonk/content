/*--- Caverace ---*/

#strict

/* Initialisierung */

protected func Initialize()
{
  var cnt=Random(5); while (++cnt<8) PlaceVehicle();
  CreateObject(GIDL,LandscapeWidth()/2,167,-1);
  var pLorry;
  CreateContents(RndCntID(),pLorry=CreateObject(LORY,50,LandscapeHeight()-50,-1));
  CreateContents(EFLN,pLorry);
  Log("$MsgGo$");
  ScriptGo(1);
  return(1);
}

func Script20()
{
  var obj;
  if (obj=FindObject(LORY,0,LandscapeHeight()-100,LandscapeWidth(),100)) EnsureFilling(obj);
  return(goto(1));
}

static rnd_x, rnd_y;

private func PlaceVehicle()
{
  var obj, c=0, i=0;
  if (!FindVehPos()) return();
  obj=CreateObject(RndVehID(),rnd_x,rnd_y,-1);
  if (GetID(obj)==CANN) while (c++<5) CreateContents(GUNP,obj);
  if (GetID(obj)==XBOW) { while (i++<10) CreateContents(XARW,obj); return(obj); }
  var cnt; while(++cnt<Random(30)) CreateContents(RndCntID(obj),obj);
  return(obj);
}

private func FindVehPos()
{
  var cnt=0;
  while (cnt++<25)
    {
    rnd_x=Random(LandscapeWidth());
    rnd_y=Random(LandscapeHeight()-300)+300;
    if (GBackSolid(rnd_x, rnd_y-25)) rnd_y+=25;
    if (!GBackSolid(rnd_x, rnd_y) && !GBackLiquid(rnd_x, rnd_y)) return(1);
    }
  return();
}

private func RndVehID()
{
  var num;
  if (!(num=Random(6))) return(LORY);
  if (!--num)           return(CATA);
  if (!--num)           return(CANN);
  if (!--num)           return(WAGN);
  if (!--num)           return(XBOW);
                        return(HUT1);
}

private func RndCntID()
{
  var num;
  if (!(num=Random(25))) return(SFLN);
  if (!--num)            return(STFN);
  if (!--num)            return(GUNP);
  if (!--num)            return(EFLN);
  if (!--num)            return(FBMP);
  if (!(num=Random(4)))  return(FLNT);
  if (!--num)            return(LOAM);
  if (!--num)            return(METL);
                         return(CNKT);
}


// -- Callbacks des Rennen-Spielziels --
// wenn diese Funktionen nicht im Szenarioscript sind
// oder 0 zurück geben, wird der Default-Wert verwendet

// Richtung:
// 1: links -> rechts
// 2: rechts -> links
// 3: untern -> oben
// 4: oben -> unten
// default: links -> rechts
func GetRACEDirection() { return(3); }

// Start: Anzahl an Pixeln, ab dem Rand, von dort beginnt die Zählung
// default: 50 px
func GetRACEStartOffset() { return(20); }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Zählung
// default: 50 px
func GetRACEEndOffset() { return(160); }

/* Spielerinitialisierung */

protected func InitializePlayer(int iPlr){ return(JoinPlayer(iPlr));}

private func JoinPlayer(int iPlr)
{
  if (!(++Global(2+iPlr)%5)) Log(RndJoinCntMsg(), GetPlayerName(iPlr), Global(2+iPlr));
  var obj=GetCrew(iPlr);
  DoEnergy(100000,obj);
  if (Global(0)) SetPhysical("CanScale", 1, 2, obj);
  if (Global(1)) SetPhysical("CanHangle", 1, 2, obj);
  SetPosition(10+Random(100), LandscapeHeight()-50, obj);
  CreateContents(LOAM,obj);
  return(1);
}

private func EnsureFilling(object pObj)
{
  EnsureContCnt(LOAM,5,pObj);
  EnsureContCnt(METL,5,pObj);
  EnsureContCnt(FLNT,5,pObj);
  return(1);
}

private func EnsureContCnt(id iId, int iAmount, object pObj)
{
  while (ContentsCount(iId,pObj)<iAmount) CreateContents(iId,pObj);
  return(1);
}

func RndJoinCntMsg()
{
  var rand=Random(5);
  if (!rand)  return("$MsgRelaunch1$");
  if (!--rand)            return("$MsgRelaunch2$");
  if (!--rand)            return("$MsgRelaunch3$");
  if (!--rand)            return("$MsgRelaunch4$");
  if (!--rand)            return("$MsgRelaunch5$");
}

/* Neubeitritt */

public func RelaunchPlayer(int iPlr)
{
  var pCrew;
  MakeCrewMember(pCrew=CreateObject(CLNK,0,0,iPlr),iPlr);
  SelectCrew(iPlr, pCrew, 1);
  Log(RndRelaunchMsg(), GetPlayerName(iPlr));
  return(JoinPlayer(iPlr));
}

private func RndRelaunchMsg()
{
  var rand=Random(11);
  if (!rand) return("$MsgDeath1$");
  if (!--rand)            return("$MsgDeath2$");
  if (!--rand)            return("$MsgDeath3$");
  if (!--rand)            return("$MsgDeath4$");
  if (!--rand)            return("$MsgDeath5$");
  if (!--rand)            return("$MsgDeath6$");
  if (!--rand)            return("$MsgDeath7$");
  if (!--rand)            return("$MsgDeath8$");
  if (!--rand)            return("$MsgDeath9$");
  if (!--rand)            return("$MsgDeath10$");
                          return("$MsgDeath11$");
}

global func __AllScale() { return(Global(0)=1);}
global func __AllHangle(){ return(Global(1)=1);}
global func __NoScale()  { return(Global(0)=0);}
global func __NoHangle() { return(Global(1)=0);}
