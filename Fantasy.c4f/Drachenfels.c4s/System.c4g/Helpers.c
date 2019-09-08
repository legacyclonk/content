/* Hilfsfunktionen */

#strict

static g_MessageExWaitTime;

// Nachricht in Clonkfarbe mit Namen
global func MessageEx(string sMessage, object pTarget)
  {
  var dwColor = GetColorDw(pTarget); if (!dwColor) dwColor=255;
  dwColor = MakeReadableColor(dwColor);
  if (!pTarget) dwColor=16777215;
  sMessage = ColorizeString(Format("%s:|%s", GetName(pTarget), sMessage), dwColor);
  sMessage = Format(sMessage, ...);
  // Wartezeit berechnen
  g_MessageExWaitTime = 0; var i;
  while (GetChar(sMessage, i++)) g_MessageExWaitTime += 2;
  g_MessageExWaitTime = Max(g_MessageExWaitTime, 20);
  // Nachricht ausgeben
  return(Message(sMessage, pTarget));
  }


// Färbt einen mehrzeiligen Text ein
global func ColorizeString(string sText, int dwColor)
  {
  // Enginebug ist obsolet :D
  return(Format("<c %x>%s</c>", dwColor, sText));
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

// Crewobjekt nach Namen suchen
global func FindCrewByName(id idCrew, string sName)  
  {
  var pObj;
  while (pObj = FindObject(idCrew, 0,0,0,0, 0, 0,0, 0, pObj))
    if (pObj->GetName() eq sName)
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

global func FadeClrModulation()
  {
  AddEffect("IntClrMod", this(), 1, 4, this());
  }

global func FxIntClrModTimer(object pTarget, int iNumber)
  {
  var r,g,b,a;
  SplitRGBaValue(GetClrModulation(),r,g,b,a);
  r = Min(r+5, 255);
  g = Min(g+5, 255);
  b = Min(b+5, 255);
  a = Max(a-5);
  r = RGBa(r,g,b,a);
  if (r == 16777216) r=0;
  SetClrModulation(r);
  if (!r) return(-1);
  }

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
  // Nicht einsammelbar beim Ausfaden
  SetObjectLayer(this());
  }

global func FxIntFadeTimer(object pTarget, int iEff)
  {
  // Alpha aufzählen
  var dwMod = pTarget->GetClrModulation();
  var iAlpha = BoundBy((dwMod>>24&255) + EffectVar(0, pTarget, iEff), 0, 255);
  if (iAlpha && !dwMod) dwMod=16777215;
  pTarget->SetClrModulation(dwMod & 16777215 | (iAlpha<<24));
  // Ende?
  if (!iAlpha) return(FX_Execute_Kill);
  if (iAlpha==255) return(FX_Execute_Kill, RemoveObject(pTarget));
  }

global func FxIntFadeEffect(string szNewEffect, object pTarget, int iEff)
  {
  if (szNewEffect eq "IntFade") return(FX_Effect_Annul);
  }

global func FxIntFadeAdd(object pTarget, int iEff, string szName, int iNewTimer, int iFadeDir)
  {
  // Fading ändern
  EffectVar(0, pTarget, iEff) = iFadeDir;
  ChangeEffect(0, pTarget, iEff, "IntFade", iNewTimer);
  }
  
global func AddDivineShield(object pTarget)
  {
  if (!pTarget) pTarget = this();
  if (!pTarget) return(Log("MakeDivine: No target"));
  if (!AddEffect("DivineShield", pTarget, 1000))
    return(Log("MakeDivine: Unknown error"));
  Log("%s is shielded!", GetName(pTarget));
  return(true);
  }

global func FxDivineShieldDamage(object pTarget, int iNumber, int iDmgEngy, int iCause)
  {
  // Nie Schaden nehmen
  }

global func FxDivineShieldEffect(string szNewEffectName)
  {
  // nicht anbrennen lassen
  if(WildcardMatch(szNewEffectName,"*Fire*")) return(FX_Effect_Deny);
  // gleichen Effekt ablehnen
  if(szNewEffectName eq "DivineShield") return(FX_Effect_Deny);
  }
  