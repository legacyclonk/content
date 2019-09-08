/* Hilfsfunktionen */

#strict

static g_MessageExWaitTime, g_GlobalContainer;

// Nachricht in Clonkfarbe mit Namen
global func MessageEx(string sMessage, object pTarget, v1,v2,v3,v4,v5,v6,v7,v8)
  {
  if(ScoreboardCol(NET2))
  {
    var szPortrait = GetPortrait(pTarget, 0, 1);
    if(GetID(pTarget)==TRPR) szPortrait = "1";
    if(pTarget->~IsChief()) szPortrait = "IndianChief";
    if(pTarget->~IsCaptain()) if(GetID(pTarget)==BNDT) szPortrait = "BanditBoss"; else szPortrait = "Captain1";
    if(this()) this()->~DlgMsg(sMessage, pTarget, szPortrait);	  
    return(1); 
  }		  
  var dwColor = GetColorDw(pTarget); if (!dwColor) dwColor=255;
  dwColor = MakeReadableColor(dwColor);
  if (!pTarget) dwColor=16777215;
  sMessage = ColorizeString(Format("%s:|%s", GetName(pTarget), sMessage), dwColor);
  sMessage = Format(sMessage, v1,v2,v3,v4,v5,v6,v7,v8);
  // Wartezeit berechnen
  g_MessageExWaitTime = 0; var i;
  while (GetChar(sMessage, i++)) g_MessageExWaitTime += 2;
  g_MessageExWaitTime = Max(g_MessageExWaitTime, 20);
  // Nachricht ausgeben
  return(Message(sMessage, pTarget));
  }

// Fehlernachricht
global func PlayerError(string szMessage, object pPlayer, v1,v2,v3,v4,v5,v6,v7,v8)
  {
  MessageEx(szMessage, pPlayer, v1,v2,v3,v4,v5,v6,v7,v8);
  Sound("CommandFailure*", 0, pPlayer, 100, GetController(pPlayer));
  // return(0);
  }


// Färbt einen mehrzeiligen Text ein
global func ColorizeString(string sText, int dwColor)
  {
  // Enginebug ist obsolet :D
  return(Format("<c %x>%s</c>", dwColor, sText));
  // wg. Enginebug zeilenweise einfärben...
  var c,i,sLine="",sResult="";
  while (c = GetChar(sText, i++))
    if (c == GetChar("|"))
      {
      if (GetChar(sResult))
        sResult = Format("%s|<c %x>%s</c>", sResult, dwColor, sLine);
      else
        sResult = Format("<c %x>%s</c>", dwColor, sLine);
      sLine = "";
      }
    else
      sLine = Format("%s%c", sLine, c);
  if (GetChar(sLine))
    if (GetChar(sResult))
      sResult = Format("%s|<c %x>%s</c>", sResult, dwColor, sLine);
    else
      sResult = Format("<c %x>%s</c>", dwColor, sLine);
  return(sResult);
  }
  
// Hellt dunkle Farben auf
global func MakeReadableColor(int dwColor)
  {
  // Helligkeit ermitteln
  // 50% rot, 87% grün, 27% blau (Max 164 * 255)
  var r=(dwColor>>16&255), g=(dwColor>>8&255), b=(dwColor&255);
  var iLightness = r*50 + g*87 + b*27;
  // Oberhalb 35/164 (*255) ist OK; dann nur Alpha verwerfen
  if (iLightness >= 8925) return(dwColor & 16777215);
  // Sonst aufhellen
  return(RGB(Min(r+50, 255), Min(g+50, 255), Min(b+50, 255)));
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
