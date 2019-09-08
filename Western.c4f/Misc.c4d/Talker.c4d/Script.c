/*-- Sprecher --*/

#strict

static const    _TLK_ID                  = _TLK,
        _TLK_TimerInterval   = 1;

local bTalking;
local pTalkTo;
local iTalkIndex;
local iTalkIndex2;
local pNPC;
local pPlayer;
local iWait;

global func MakeTalker(string szName, object pTarget)
  {
  // Ziel finden
  if(!pTarget) pTarget = this();
  if(!pTarget) return();
  // Name setzen
  if(szName) SetName(szName, pTarget);
  else szName = GetName(pTarget);
  // Talker erstellen
  var pTalker = CreateObject(_TLK_ID, 0, 10, -1);
  SetAlive(0, pTalker);
  SetName(szName, pTalker);
  ObjectSetAction(pTalker, "Attach", pTarget);
  return(1);
  }

public func ActivateEntrance(object pObj)
{
  if(bTalking) return();
  if(!GetAlive(pObj)) return(); // Nur Lebende können sprechen
//  LocalN("bTalking", pObj) = 1;
  bTalking = 1;
  iTalkIndex=0;
  pPlayer=pObj;
  pNPC=GetActionTarget();
  LocalN("bTalking", pNPC)=1;
  LocalN("bTalking", pPlayer)=1;
  LocalN("pTalker",  pPlayer)=this();
  FinishCommand(pPlayer, 1);
//  SetDir( (GetX(pPlayer)-GetX(pNPC) > 0), pNPC );
//  SetDir( (GetX(pNPC)-GetX(pPlayer) > 0), pPlayer );
  return(AddEffect("Talk", this(), 1, _TLK_TimerInterval, this()));
}

protected func FxTalkStart(object pTarget, int iEffectNumber, int iTemp)
  {
    Message("", LocalN("pNPC", pTarget));
    Message("", LocalN("pPlayer", pTarget));
    g_MessageExWaitTime=0;
    ObjectCall(pTarget, Format("Dlg%sStart", GetName(LocalN("pNPC", pTarget))), LocalN("pPlayer", pTarget));
    if(!PlrIsInMenu())
        if(!LocalN("iWait", pTarget)) LocalN("iWait", pTarget) = GetLastMsgWaitTime();
    return(0);
  }

protected func FxTalkTimer(object pTarget, int iEffectNumber, int iTemp)
  {
    if(PlrIsInMenu()) return(true);   
    if(LocalN("iWait", pTarget)) { LocalN("iWait", pTarget)--;  return(true); }   
    Message("", LocalN("pNPC", pTarget));
    Message("", LocalN("pPlayer", pTarget));
    LocalN("iTalkIndex", pTarget)++;
    g_MessageExWaitTime=0;
    PrivateCall(pTarget, Format("Dlg%s%d", GetName(LocalN("pNPC", pTarget)), LocalN("iTalkIndex", pTarget)), LocalN("pPlayer", pTarget));
    if(!PlrIsInMenu())
    if(!LocalN("iWait", pTarget)) LocalN("iWait", pTarget) = GetLastMsgWaitTime();
    return(true);
  }

protected func FxTalkStop(object pTarget, int iEffectNumber, int iTemp)
  {
    StopDialog(1);
    return(0);
  }

private func StopDialog(bEffect)
{
  if(!bEffect) RemoveEffect("Talk", this(), 0, 1);
  bTalking = 0;
  if(pNPC) pNPC->~SetTalking(0);
  if(pPlayer) { pPlayer->~SetTalking(0);
        pPlayer->~SetTalker(0);}
  if(sMovName)
  {
    var o;
    var iPlayer;
    while (o = FindObject(0, 0,0,0,0, OCF_Alive, 0,0, 0, o))
    {  o->~AI_Enable(); RemoveEffect("Divinity", o); }
    o=0;
    var pPlayerTalker;
    for(var i = 0; i < GetPlayerCount(); i++)
    {       
      o=0;
      iPlayer = GetPlayerByIndex(i);
          if(GetCursor(iPlayer)) CloseMenu(GetCursor(iPlayer));
      while(o = FindObjectOwner(0, iPlayer, 0, 0, 0, 0, OCF_Alive | OCF_CrewMember, 0, 0, o))
      {
        if(pPlayerTalker=o->~GetTalker()) LocalN("iWait", pPlayerTalker)=5;
        else o->~SetTalking(0);
        SetCrewEnabled(1, o); 
      }
      if(Local(iPlayer)) SetCursor(iPlayer, Local(iPlayer), true, true);
      else SetCursor(iPlayer, GetHiRank(iPlayer), true, true);
      SetPlrView(iPlayer, GetCursor(iPlayer));
    }
    Sound("Ding");
    return(RemoveObject());
  }
  Sound("Ding");
}
local sMovName, fMovie;
global func StartMovie(sName, pNewPlayer, pNewNPC)
{
  if(GetID(this())!=_TLK) return(PrivateCall(CreateObject(_TLK), "DoStartMovie", sName, pNewPlayer, pNewNPC));
  else PrivateCall(this(), "DoStartMovie", sName, pNewPlayer, pNewNPC);
}

private func DoStartMovie(sName, pNewPlayer, pNewNPC)
{
//  SetCategory(16777217);  
  bTalking = 1;
  iTalkIndex=0;
  sMovName = sName;
  fMovie = 1;
  pPlayer=pNewPlayer;
  pNPC=pNewNPC;
  if(pNPC) pNPC->~SetTalking(1);
  if(pPlayer) pPlayer->~SetTalking(1);
  FinishCommand(pPlayer, 1);
  SetAction("Attach", pPlayer);
  var o;
  while (o = FindObject(0, 0,0,0,0, OCF_Alive, 0,0, 0, o))
  {  o->~AI_Disable(); AddEffect("Divinity", o, 200, 1); }
  o=0;
  var iPlayer;
  var pPlayerTalker;  
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    o=0;
    iPlayer = GetPlayerByIndex(i);
    Local(iPlayer)=GetCursor(iPlayer);
    while(o = FindObjectOwner(0, iPlayer, 0, 0, 0, 0, OCF_Alive | OCF_CrewMember , 0, 0, o))
    {
      if(pPlayerTalker=o->~GetTalker()) LocalN("iWait", pPlayerTalker)=-1;
      o->~SetTalking(1);
      SetComDir(COMD_Stop(), o);
      if(GetCursor(iPlayer)!=o) SetCrewEnabled(0, o);
    }
    SetPlrView(iPlayer, pPlayer);
  }
  Sound("Ding");
//  SetDir( (GetX(pPlayer)-GetX(pNPC) > 0), pNPC );
//  SetDir( (GetX(pNPC)-GetX(pPlayer) > 0), pPlayer );
  return(AddEffect("Movie", this(), 1, _TLK_TimerInterval, this()));
}

public func Entrance(pClonk, pBuilding)
{
   pNPC=GetActionTarget();
   if(PrivateCall(this(), Format("Dlg%sEntrance", GetName(pNPC)), pClonk, pBuilding))
   {
    FinishCommand(pClonk, 1);
    return(1);
   }
   return(0);
}

protected func FxMovieStart(object pTarget, int iEffectNumber, int iTemp)
  {
    Message("", LocalN("pNPC", pTarget));
    Message("", LocalN("pPlayer", pTarget));
    PrivateCall(pTarget, Format("Mov%sStart", LocalN("sMovName", pTarget)));
    if(!LocalN("iWait", pTarget)) LocalN("iWait", pTarget) =  GetLastMsgWaitTime();
    return(0);
  }

protected func FxMovieTimer(object pTarget, int iEffectNumber, int iTemp)
  {
//  if(PlrIsInMenu()) return(true);   
  for(var i = 0; i < GetPlayerCount(); i++)  SetPlrView(GetPlayerByIndex(i), pTarget);

    if(LocalN("iWait", pTarget)) { LocalN("iWait", pTarget)--; return(true); }    
    Message("", LocalN("pNPC", pTarget));
    Message("", LocalN("pPlayer", pTarget));
    for(var i = 0; i < GetPlayerCount(); i++) CloseMenu(GetCursor(GetPlayerByIndex(i)));
    g_MessageExWaitTime=0;
    PrivateCall(pTarget, Format("Mov%s%d", LocalN("sMovName", pTarget), LocalN("iTalkIndex", pTarget)), LocalN("pPlayer", pTarget));
    LocalN("iTalkIndex", pTarget)++;
    //Log(Format("Mov%s%d", LocalN("sMovName", pTarget), LocalN("iTalkIndex", pTarget)));
    if(!LocalN("iWait", pTarget)) LocalN("iWait", pTarget) =  GetLastMsgWaitTime();
    return(true);
  }

protected func FxMovieStop(object pTarget, int iEffectNumber, int iReason, int iTemp)
  {
    //Log(Format("Mov%sStop%d", LocalN("sMovName", pTarget), iReason));
//  StopDialog(1);
    return(-1);
  }

public func MenuQueryCancel(int MenuItem, object pClonk) { return(fMovie); }
    
protected func Destruction()
{
  //Log("%s_%s", sMovName, GetName(GetActionTarget())); 
}

/* Entfernen, wenn das Dialogziel weg ist */

protected func AttachTargetLost() { return(RemoveObject()); }

/* Ansprechen über Linksklick */

protected func MouseSelection(int iByPlayer)
  {
  // Ziel verloren?
  if (!GetActionTarget()) return(RemoveObject());
  // Spieler muss seinen Clonk lenken
  var pClonk = GetCursor(iByPlayer);
  if (!pClonk) return();
  if (pClonk->~IsNotReady()) return();
  // Clonk zum Ziel laufen lassen und ansprechen
  SetCommand(pClonk, "Call", this(), 0,0, 0, "ActivateEntrance");
  AddCommand(pClonk, "MoveTo", this());
  }

local arAskedQuestions; // Schon gefragte Fragen

static const _TLK_DlgEffectWaitTime = 5;
static const _TLK_SpeakRadius = 100; // Px nach d2, in dessen Radius andere Clonks Nachrichten mitkriegen
static const _TLK_Wait_Menu = -1; // Konstante für DlgWait: Warten, bis Menü zu

private func PlrIsInMenu()
  {
  var pCursor = GetCursor(GetController(pPlayer));
  if (!pCursor) if (!(pCursor = pPlayer)) return();
  // Enginemenü oder Dialog
  if (GetMenu(pCursor) == _TLK) return (true);
  // Ringmenü
  if (pCursor->~IsMenu()) return (true);
  // Kein Menü
  }

global func RecheckPlrCursor(object pPlayer)
  {
  if (!pPlayer) return();
  var pDisabler;
//  if (!GetCursor(GetController(pPlayer)))
//    if (pDisabler = FindObject(_TLK, 0,0,0,0, 0, "AttachDisable", this()))
//      pDisabler->AttachDisableCheck();
  return(true);
  }

global func MsgBox(object pPlayer, string sMessage, object pSpeaker, string szPortrait)
  {
  // Defaults
  if (!pPlayer) if (!(pPlayer = this())) return();
  if (!pSpeaker) if (!(pSpeaker = this())) return();
  if (!sMessage) return();
  if (!szPortrait) if (!(szPortrait = pSpeaker->~GetMsgPortrait())) szPortrait = "1";
  // Portrait ermitteln
  if (GetChar(szPortrait) == GetChar("@"))
    {
    // @Portrait: Aus dieser Definition holen
    var c,i,p2="";
    while (c=GetChar(szPortrait, ++i)) p2 = Format("%s%c", p2, c);
    szPortrait = Format("Portrait:%i::%x::%s", _TLK, GetOriginalColorDw(pSpeaker), szPortrait, p2);
    }
  else
    {
    // Ansonsten direkt vom Sprecher
    szPortrait = Format("Portrait:%i::%x::%s", GetID(pSpeaker), GetOriginalColorDw(pSpeaker), szPortrait);
    }
  var iSpeakerNameLen = 0;
  while (GetChar(GetName(pSpeaker), iSpeakerNameLen)) ++iSpeakerNameLen;
  // Farbe ermitteln
  var dwClr = pSpeaker->~GetDlgMsgColor();
  if (!dwClr) dwClr = GetOriginalColorDw(pSpeaker);
  if (!dwClr) dwClr = 255;
  // Sicher stellen, dass es einen Cursor gibt
  RecheckPlrCursor(pPlayer);
  // Nachricht als Menü
  var pMsgTarget = GetCursor(GetController(pPlayer));
  if (!pMsgTarget) pMsgTarget = pPlayer;
  var C4MN_Style_Dialog = 3, C4MN_Add_ImgTextSpec = 5, C4MN_Add_ForceNoDesc = 512;
  if (!CreateMenu(_TLK, pMsgTarget, this(), 0, "", 0, C4MN_Style_Dialog, false, _TLK)) return();
  AddMenuItem(szPortrait, "", NONE, pMsgTarget, 0, 0, "", C4MN_Add_ImgTextSpec, 0, 0);
  AddMenuItem(Format("<c %x>%s:</c> %s", dwClr, GetName(pSpeaker), sMessage), "", NONE, pMsgTarget, 0, 0, "", C4MN_Add_ForceNoDesc, 0, 0);
  SetMenuDecoration(MD69, pMsgTarget);
  SetMenuTextProgress(iSpeakerNameLen+1, pMsgTarget);
  return (true);
  }
  
global func MsgBoxAddText(object pPlayer, string sText)
  {
  var pMsgTarget = GetCursor(GetController(pPlayer));
  if (!pMsgTarget) pMsgTarget = pPlayer;
  var C4MN_Add_ForceNoDesc = 512;
  return (AddMenuItem(sText, "", NONE, pMsgTarget, 0, 0, 0, C4MN_Add_ForceNoDesc));
  }
  
global func MsgBoxAddOption(object pPlayer, id idIcon, string sOption, string sCommand, vParam)
  {
  var pMsgTarget = GetCursor(GetController(pPlayer));
  if (!pMsgTarget) pMsgTarget = pPlayer;
  var C4MN_Add_ForceNoDesc = 512;
  return (AddMenuItem(sOption, sCommand, idIcon, pMsgTarget, 0, vParam, 0, C4MN_Add_ForceNoDesc));
  }
  
private func DlgMsgFindNextTarget(object pFindNext)
  {
  var container = NoContainer();
  if(fMovie) container = 0;
  while (pFindNext = FindObject(0, 0,0,-1,-1, OCF_CrewMember, 0,0, container, pFindNext))
    if (ObjectDistance(pFindNext) > _TLK_SpeakRadius && !fMovie) break;
    else if (GetAlive(pFindNext) && pFindNext != pPlayer && GetOwner(pFindNext) != NO_OWNER)
      if(GetCrewEnabled(pFindNext))//pFindNext->IsControlDisabled() &&
              if(!GetEffect("Talk", pFindNext))
        return (pFindNext);
  }

public func DlgMsg(string sMessage, object pSpeaker, string szPortrait, bool fWhisper)
  {
  // Nachricht an umliegende Clonks
  var o;
  if (!fWhisper)
    while (o = DlgMsgFindNextTarget(o))
    {
      MsgBox(o, sMessage, pSpeaker, szPortrait);
      if(fMovie) StopClonkEx(o, 1);
    }
  // Nachricht zum Spieler
  return (MsgBox(pPlayer, sMessage, pSpeaker, szPortrait));
  }

global func CreateArray(a,b,c,d,e,f,g,h) { return(_inherited(a,b,c,d,e,f,g,h)); }
global func ScoreboardCol(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
global func SetMenuDecoration(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
global func SetMenuTextProgress(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
  
private func DlgAddOption(aIcon, string sOption, string sCommand, vParam, int iQIdx, int iExtra, xPar)
  {
  var C4MN_Add_ImgObject = 4, C4MN_Add_ImgTextSpec = 5, C4MN_Add_ForceNoDesc = 512;
  // Optionen
  iExtra |= C4MN_Add_ForceNoDesc;
  // Icon auflösen
  if (!aIcon)
    // Standardicon
    aIcon = ROCK;
  else if (GetType(aIcon) == C4V_C4Object)
    {
    // Icon nach Objekt in Objektfarbe
    iExtra |= C4MN_Add_ImgObject;
    aIcon = GetID(xPar = aIcon);
    }
  // Ansonsten bleibt das angegebene Icon
  // Optionen gehen nur an den echten Dialogspieler
  var pMsgTarget = GetCursor(GetController(pPlayer));
  if (!pMsgTarget) pMsgTarget = pPlayer;
  return (AddMenuItem(sOption, sCommand, aIcon, pMsgTarget, 0, vParam, 0, iExtra, xPar));
  }
  
private func DlgAddText(string sText, bool fWhisper)
  {
  // Nachricht an umliegende Clonks
  var o;
  if (!fWhisper)
    while (o = DlgMsgFindNextTarget(o))
      MsgBoxAddText(o, sText);
  // Nachricht zum Spieler
  return (MsgBoxAddText(pPlayer, sText)); 
  }
  
  
// Helpers
static g_MessageExWaitTime, g_GlobalContainer;

// Nachricht in Clonkfarbe mit Namen
global func MessageEx(string sMessage, object pTarget, v1,v2,v3,v4,v5,v6,v7,v8)
  {
  var szPortrait = GetPortrait(pTarget, 0, 1);
  if(GetID(pTarget)==TRPR) szPortrait = "1";
  if(pTarget->~IsChief()) szPortrait = "IndianChief";
  if(pTarget->~IsCaptain()) if(GetID(pTarget)==BNDT) szPortrait = "BanditBoss"; else szPortrait = "Captain1";
  if(this()) this()->~DlgMsg(sMessage, pTarget, szPortrait);    
  return(1);
  }

// Fehlernachricht
global func PlayerError(string szMessage, object pPlayer, v1,v2,v3,v4,v5,v6,v7,v8)
  {
  MessageEx(szMessage, pPlayer, v1,v2,v3,v4,v5,v6,v7,v8);
  Sound("CommandFailure*", 0, pPlayer, 100, GetController(pPlayer));
  // return(0);
  }
  
// Richtung anpassen, so dass das Objekt zum anderen Objekt schaut
global func SetDirTo(object pTarget, object pThis)
  {
  if (GetX(pTarget) > GetX(pThis))
    SetDir(DIR_Right, pThis);
  else if (GetX(pTarget) < GetX(pThis))
    SetDir(DIR_Left, pThis);
  }
  
// Clonk anhalten
global func StopClonk(object pTarget)
  {
  if (!pTarget) if (!this()) return();
  SetComDir(COMD_Stop, pTarget);
  SetCommand(pTarget, "None");
  Call("~StopAiming");
  }
  
// Clonk für Dialog vorbereiten: Anhalten, Menüs schließen, etc.
global func StopClonkEx(object pTarget, bool fKeepAction)
  {
  if (!pTarget) if (!this()) return();
  if (!fKeepAction) if (pTarget) ObjectSetAction(pTarget, "Walk"); else SetAction("Walk");
  StopClonk(pTarget);
  CloseMenu(pTarget);
  }

// Crewobjekt nach Namen suchen - Spielerclonks ignorieren!
global func FindCrewByName(id idCrew, string sName)  
  {
  var pObj;
  while (pObj = FindObject(idCrew, 0,0,0,0, 0, 0,0, 0, pObj))
    if (pObj->GetName() eq sName)
      if (GetOwner(pObj) == NO_OWNER)
        return(pObj);
  }
  

global func FATAL_ERROR(string sError)
  {
  Log(sError);
  Message(sError);
  return(Schedule("GameOver(2000)", 1));
  }

global func CheckObjectOrder(bool fCorrect)
  {
  var obj, cat=16, c2;
  while (obj = FindObject(0, 0,0,0,0, 0, 0,0, 0, obj))
    {
    c2 = obj->GetCategory()&31;
    if (c2<cat)
      {
      cat=c2;
      Log("Object %d (%s) setting category %d", ObjectNumber(obj), obj->GetName(), c2);
      }
    else if (c2>cat)
      {
      Log("!!! Object %d (%s) is in wrong sort-order %d!", ObjectNumber(obj), obj->GetName(), c2);
      if (fCorrect) obj->SetCategory(c2);
      }
    }
  }


// Wartezeit für eine Nachricht von MessageEx
global func GetLastMsgWaitTime() { return(g_MessageExWaitTime); }


/* Fade: Objekte, Landschaft und Himmel nach Schwarz ausfaden */
// Fadet leider keine Overlays :(
// EffectVars: 0 - Zielfading
//             1 - Aktuelles Fading
//             2 - Vorherige ClrModulation / Vorheriger SkyAdjust (BackClr nicht unterstützt)
//             3 - Vorherige ColorDw / Vorheriger MatAdjust
//             4 - Schrittweite
//             5 - Vorheriger BlitMode
//             5 - Vorheriger BlitMode

global func GlobalFadeTo(int iToVal, int iStepSize)
  {
  if (!iStepSize) iStepSize = 5;
  iToVal = BoundBy(iToVal, 0, 255);
  // Himmel+Erde
  AddEffect("IntGlobalClrMod", 0, 200, 4, this(), 0, iToVal, iStepSize);
  // Objekte
  var o;
  while (o = FindObject(0, 0,0,0,0, 0, 0,0, 0, o))
    AddEffect("IntGlobalClrMod", o, 200, 4, this(), 0, iToVal, iStepSize);
  }
  
global func GlobalFadeSetTo(int iToVal)
  {
  // Fade setzen: Einfach Fade mit maximaler Schrittweite starten
  return (GlobalFadeTo(iToVal, 255));
  }
  
global func GetOriginalColorDw(object pObj)
  {
  // Ursprüngliche Färbung eines Objektes ohne ColorMod ermitteln: Umfärbeeffekt abfragen wenn vorhanden
  if (!pObj) if (!(pObj = this())) return();
  var iEff;
  if (iEff = GetEffect("IntGlobalClrMod", pObj)) return(EffectVar(3, pObj, iEff));
  return (GetColorDw(pObj));
  }
  
global func FxIntGlobalClrModStart(object pTarget, int iEff, int iTemp, int iToVal, int iStep)
  {
  if (!iTemp)
    {
    EffectVar(0, pTarget, iEff) = iToVal;
    EffectVar(1, pTarget, iEff) = 255;
    EffectVar(4, pTarget, iEff) = iStep;
    }
  if (pTarget)
    {
    var dwClr = GetClrModulation(pTarget);
    if (!dwClr) dwClr = 16777215;
    EffectVar(2, pTarget, iEff) = dwClr;
    dwClr = GetColorDw(pTarget); if (!dwClr) dwClr = 255;
    EffectVar(3, pTarget, iEff) = dwClr;
    EffectVar(5, pTarget, iEff) = GetObjectBlitMode(pTarget);
    }
  else
    {
    EffectVar(2, pTarget, iEff) = GetSkyAdjust();
    EffectVar(3, pTarget, iEff) = GetMatAdjust();
    }
  return(FxIntGlobalClrModTimer(pTarget, iEff));
  }
  
global func FxIntGlobalClrModTimer(object pTarget, int iEff)
  {
  var iFade = EffectVar(1, pTarget, iEff);
  var iFadeTo = EffectVar(0, pTarget, iEff);
  var iStepSize = EffectVar(4, pTarget, iEff);
  var dwBlitMode = EffectVar(5, pTarget, iEff);
  iFade += BoundBy(iFadeTo-iFade, -iStepSize, iStepSize);
  EffectVar(1, pTarget, iEff) = iFade;
  // Fading aktualisieren
  var dwClr1 = EffectVar(2, pTarget, iEff);
  var dwClr2 = EffectVar(3, pTarget, iEff), dw;
  if (iFade != 255)
    {
    dwClr1 = ModulateColor(dwClr1, dw=RGB(iFade, iFade, iFade));
    dwClr2 = ModulateColor(dwClr2, dw);
    }
  if (dwClr1 == 16777215) dwClr1 = 0; else if (!dwClr1)
    {
    dwClr1 = 1;
    dwBlitMode = dwBlitMode & (-1 - GFX_BLIT_Mod2 - GFX_BLIT_ClrSfc_Mod2);
    }
  if (!dwClr2) dwClr2 = 1;
  if (pTarget)
    {
    SetClrModulation(dwClr1, pTarget);
    SetColorDw(dwClr2, pTarget);
    SetObjectBlitMode(dwBlitMode, pTarget);
    }
  else
    {
    SetSkyAdjust(dwClr1);
    SetMatAdjust(dwClr2);
    }
  // Ziel erreicht: Timer aus
  if (iFade == iFadeTo)
    if (iFadeTo == 255)
      // Alles normal: Effekt löschen
      return(FX_Execute_Kill);
    else
      // Im anormalen Zustand: Nur den Timer ausschalten
      ChangeEffect(0, pTarget, iEff, "IntGlobalClrMod", 0);
  }
  
global func FxIntGlobalClrModStop(object pTarget, int iEff, bool fTemp)
  {
  if (pTarget)
    {
    SetClrModulation(EffectVar(2, pTarget, iEff), pTarget);
    SetColorDw(EffectVar(3, pTarget, iEff), pTarget);
    }
  else
    {
    SetSkyAdjust(EffectVar(2, pTarget, iEff));
    SetMatAdjust(EffectVar(3, pTarget, iEff));
    }
  }
  
global func FxIntGlobalClrModEffect(string szNewEffect, object pTarget, int iEff)
  {
  if (szNewEffect eq "IntGlobalClrMod") return(-2);
  }

global func FxIntGlobalClrModAdd(object pTarget, int iEff, string szName, int iNewTimer, int iToVal, int iStep)
  {
  // Fading ändern
  EffectVar(0, pTarget, iEff) = iToVal;
  EffectVar(4, pTarget, iEff) = iStep;
  ChangeEffect(0, pTarget, iEff, "IntGlobalClrMod", iNewTimer);
  }

    
/* Fade: Objekte nach sichtbar/unsichtbar faden */

global func FadeIn(int iTimer, int iStep, object pObj)
  {
  // Lokaler Aufruf/Sicherheit
  if (!pObj) if (!(pObj=this())) return();
  if (!iTimer) iTimer=1;
  if (!iStep) iStep=1;
  // Faden
  return(AddEffect("IntFade", pObj, 2, iTimer, 0, 0, -iStep));
  }

global func FadeOut(int iTimer, int iStep, object pObj)
  {
  // Lokaler Aufruf/Sicherheit
  if (!pObj) if (!(pObj=this())) return();
  if (!iTimer) iTimer=1;
  if (!iStep) iStep=1;
  // Faden
  return(AddEffect("IntFade", pObj, 2, iTimer, pObj, 0, iStep));
  }

global func FxIntFadeStart(object pTarget, int iEff, int iTemp, int iFadeDir)
  {
  if (iTemp) return();
  EffectVar(0, pTarget, iEff) = iFadeDir;
  }

global func FxIntFadeTimer(object pTarget, int iEff)
  {
  // Alpha aufzählen
  var dwMod = pTarget->GetClrModulation();
  var iAlpha = BoundBy((dwMod>>24&255) + EffectVar(0, pTarget, iEff), 0, 255);
  if (iAlpha && !dwMod) dwMod=16777215;
  pTarget->SetClrModulation(dwMod & 16777215 | (iAlpha<<24));
  // Ende?
  if (!iAlpha) return(-1);
  if (iAlpha==255) return(-1, RemoveObject(pTarget));
  }

global func FxIntFadeEffect(string szNewEffect, object pTarget, int iEff)
  {
  if (szNewEffect eq "IntFade") return(-2);
  }

global func FxIntFadeAdd(object pTarget, int iEff, string szName, int iNewTimer, int iFadeDir)
  {
  // Fading ändern
  EffectVar(0, pTarget, iEff) = iFadeDir;
  ChangeEffect(0, pTarget, iEff, "IntFade", iNewTimer);
  }
  
  
/* Objektrepos */

global func HideObject(object pObj)
  {
  if (!pObj) if (!(pObj = this())) return();
  if (!g_GlobalContainer) g_GlobalContainer = CreateObject(AT69,50,50,-1);
  Enter(g_GlobalContainer, pObj);
  }
  
global func ShowObject(object pObj)
  {
  if (Contained(pObj) == g_GlobalContainer) Exit(pObj);
  }
  
  
/* Bitshift */

global func ShrU(x, n) { return((x & 2147483647) >> n | (x<0) << 31-n); }
global func ShlU(x, n) { return((x & 2147483647) << n); }


/* Verbrenneffekt */

global func SetScorching(object obj)
  {
  if (!obj) if (!(obj = this())) return();
  // Abdunkeln
  SetClrModulation(RGB(10,10), obj);
  // Qualmen solange contained
  return (AddEffect("IntScorching", obj, 80, 10, obj));
  }
  
global func FxIntScorchingTimer(object obj, int iEff, int iTime)
  {
  // Dunkler Rauch
  Smoke(0,0, GetMass()+5, RGB(127,127,127));
  // Raucht solange wie Contained
  if (!Contained()) return (FX_Execute_Kill, Sound("Pshshsh"));
  }

