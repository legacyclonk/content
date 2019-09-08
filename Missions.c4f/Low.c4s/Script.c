/*--- In der Tiefe ---*/

#strict

static const g_Allow_Rejoin = false;

/* Initialisierung */

protected func Initialize()
  {
  var sx=LandscapeWidth()/2, sy;
  while (!GBackSolid(sx, sy)) ++sy;
  CreateConstruction(_STA, sx,sy, -1, 100, true, false);
  // Anfangslore mit endlos Material
  var lorry = PlaceVehicle(LORY,50,LandscapeHeight()-40);
  // Lebensmittelversorgung für den Aufstieg
  EnsureFilling(lorry);
  EnsureContCnt(BRED, 6, lorry);
  EnsureContCnt(CBRD, 3, lorry);
  // Nacht
  //ObjectCall(FindObject(TIME), "SetTime", 50);
  // Startnachricht
  Message("$MsgGo$");
  // Leuchten von all der Lava
  SetGamma(0, RGB(160,110,90), RGB(255,255,255));
  // Fahrzeuge in die Höhle
  var cnt=Random(5); while (++cnt<8) PlaceVehicle();
  // Spielziel einrichten
  var goal = CreateObject(SCRG,50,50,-1);
  goal->SetMessages("$MsgGoalFulfilled$", "$MsgGoalUnfulfilled$");
  goal->SetMissionAccess("TheAbyss");
  // Statuenteile erzeugen
  CreateStatuePart(_PA1); CreateStatuePart(_PA2); CreateStatuePart(_PA3);
  CreateStatuePart(_PB4); CreateStatuePart(_PA5); CreateStatuePart(_PA6);
  // Fertig
  return(1);
  }

private func CreateStatuePart(id idPart)
  {
  // Manchmal im Container erzeugen
  var cont;
  if (!Random(5))
    if (cont = FindObject(RndStatContID(), Random(LandscapeWidth()), Random(LandscapeHeight()), -1,-1))
      return(CreateContents(idPart, cont));
  // Sonst im freien erzeugen
  var x,y;
  if (!FindVehPos(x,y))
    {
    // Keine Position gefunden: Dann halt irgendwo...
    x=Random(LandscapeWidth()-100)+50;
    y=Random(LandscapeHeight()*2/3-150)+LandscapeHeight()/3+50;
    }
  var pPart = CreateObject(idPart, x, y, -1);
  pPart->SetR(Random(360));
  return(pPart);
  }

// Ein zufälliges Fahrzeug platzieren
private func PlaceVehicle(id idVeh, int x, int y)
  {
  // Nicht gegebene Parameter mit Zufall füllen
  var obj;
  if (!x && !y) if (!FindVehPos(x, y)) return();
  if (!idVeh) idVeh = RndVehID();
  // Fahrzeug erzeugen
  obj=CreateObject(idVeh,x,y,-1);
  // Haubitze: Schießpulver
  if (GetID(obj)==CANN) while (Var()++<5) CreateContents(GUNP,obj);
  // Armbrust: Pfeile und sonst nix
  if (GetID(obj)==XBOW) { while (Var()++<10) CreateContents(XARW,obj); return(obj); }
  // Normale Objekte mit zufälligem Material
  var cnt; while(++cnt<Random(50)) CreateContents(RndCntID(obj),obj);
  // Loren kriegen immer Standardmaterial
  if (GetID(obj)==LORY) AddEffect("IntLorryFill", obj, 1, 4440);
  return(obj);
  }

// Position für ein Fahrzeug suchen
// PlaceAnimal ginge; aber dann stecken sie ja nicht so schön an der Decke =)
private func FindVehPos(&x, &y)
  {
  var i;
  while (i++<25)
    {
    x=Random(LandscapeWidth()-50)+25;
    y=Random(LandscapeHeight()-350)+300;
    if (GBackSolid(x, y-25)) y+=25;
    if (!GBackSolid(x, y) && !GBackLiquid(x, y)) return(true);
    }
  return(false);
  }

// Zufällige Fahrzeug-ID
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

// Zufälliges Materialteil
private func RndCntID()
  {
  var num;
  if (!(num=Random(25))) return(SFLN);
  if (!--num)            return(STFN);
  if (!--num)            return(GUNP);
  if (!--num)            return(EFLN);
  if (!--num)            return(FBMP);
  if (!--num)            return(WBRL);
  if (!--num)            return(CNKT);
  if (!(num=Random(4)))  return(FLNT);
  if (!--num)            return(LOAM);
  if (!--num)            return(TFLN);
                         return(METL);
  }

// Zufälliger Container für ein Statuenteil
private func RndStatContID()
  {
  if (!Random(2)) return(HUT1); return(WAGN);
  }



/* Spielerinitialisierung */

protected func InitializePlayer(int iPlr)
  {
  if (FrameCounter()>370) return(PlrMessage("$MsgNoRuntimeJoin$", iPlr), EliminatePlayer(iPlr));
  return(JoinPlayer(iPlr));
  }

private func JoinPlayer(int iPlr)
  {
  // Start mit voller Energie
  var obj=GetCrew(Par());
  DoEnergy(100000,obj);
  // Start in der Höhle und mit Lehm
  SetPosition(10+Random(100), LandscapeHeight()-40, obj);
  CreateContents(LOAM,obj);
  return(1);
  }



/* Automatisches Füllen der Anfangslore */

global func FxIntLorryFillTimer(object pTarget, int iEffNumber, int iEffTime)
  {
  return(GameCall("EnsureFilling", pTarget));
  }

public func EnsureFilling(object pOfObj)
  {
  if (!pOfObj) return();
  EnsureContCnt(LOAM,5,pOfObj);
  EnsureContCnt(METL,5,pOfObj);
  EnsureContCnt(FLNT,5,pOfObj);
  //EnsureContCnt(CBRD,1,pOfObj);
  //EnsureContCnt(BRED,1,pOfObj);
  if (!Random(20)) EnsureContCnt(CNKT,1,pOfObj);
  return(1);
  }

private func EnsureContCnt(id idType, int iCount, object pInObj)
  {
  var i=iCount-ContentsCount(idType,pInObj);
  while (i-- > 0) { CreateContents(idType,pInObj); Sparkle(5, GetX(pInObj), GetY(pInObj)); }
  return(1);
  }
