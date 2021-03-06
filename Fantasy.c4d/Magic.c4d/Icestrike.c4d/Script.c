/*-- Eisschlag --*/

#strict

// Der Eisball
local pBall;

func Activate(pCaller)
{
  // Effekt pr�fen
  var iResult;
  if (iResult = CheckEffect("IcestrikeNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  
  // Zauber
  Sound("Magic*");
  var iX, iY;
  iX=AbsX(GetX(pCaller)-15+30*GetDir(pCaller));
  iY=AbsY(GetY(pCaller));
  pBall=CreateObject(ICEB, AbsX(GetX(pCaller)-25+50*GetDir(pCaller)), AbsY(GetY(pCaller)), GetOwner(pCaller));
  pBall->SetCaster(pCaller);
  // Zielen wenn m�glich
  if (pCaller->~DoSpellAim(this())) return(1);
  // Clonk kann nicht zielen: Fliegt der Feuerball halt frei
  pBall->SetAngle(GetDir(pCaller)*180-90);
  RemoveObject();
  return(1);
}

// Zwischenaufruf: Zielrichtung ge�ndert
public func AimingAngle(object pCaller, int iAngle)
  {
  if (pBall) pBall->AimAngle(iAngle);
  return(1);
  }

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  // Eisball freigeben (wenn noch existent)
  if (pBall) pBall->SetAngle(iAngle);
  if(GetCursor(GetOwner())==pCaller)
  {
    SetCursor(GetOwner(pCaller), pBall);
    SetPlrViewRange(200, pBall);
  }
  // Objekt l�schen
  return(RemoveObject());
  }

// Abbruch der Zielaktion: Eisball auch l�schen
public func AbortAiming(object pCaller)
  {
  if (pBall) pBall->Abort();
  // R�ckgabewert 0: Standardaktion (Zauber l�schen und Mana r�ckerstatten)
  return(0);
  }

public func IsAimerSpell() { return(1); }

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("344"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
