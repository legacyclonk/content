/*-- Feuerball --*/

#strict

// Der Feuerball
local pBall;

func Activate(pCaller, pRealCaster)
{
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("FireballNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  // Zauber
  Sound("Magic*");
  var iX, iY;
  iX=AbsX(GetX(pCaller)-15+30*GetDir(pCaller));
  iY=AbsY(GetY(pCaller));
  pBall=CreateObject(FIRB, AbsX(GetX(pCaller)-25+50*GetDir(pCaller)), AbsY(GetY(pCaller)), GetOwner(pCaller));
  pBall->SetCaster(pCaller);
  // Zielen wenn möglich
  if (!pRealCaster) pRealCaster = pCaller;
  if (pRealCaster->~DoSpellAim(this(), pCaller)) return(1);
  // Clonk kann nicht zielen: Fliegt der Feuerball halt frei
  CheckCombo(pCaller);
  pBall->SetAngle(GetDir(pCaller)*180-90);
  RemoveObject();
  return(1);
}

private func CheckCombo(object pCaller)
  {
  // Komboobjekt anwenden, wenn gefunden
  var pComboObj, iSizeBoost;
  if (!(pComboObj=FindContents(EFLN, pCaller)))
    if (!(pComboObj=FindContents(SFLN, pCaller)))
      if (!(pComboObj=FindContents(FLNT, pCaller)))
        if (!(pComboObj=FindContents(TFLN, pCaller)))
          if (!(pComboObj=FindContents(STFN, pCaller)))
            if (!(pComboObj=FindContents(GUNP, pCaller)))
              ; // Keine Kombo
  if (pComboObj)
    {
    iSizeBoost = pComboObj->~ExplodeSize() * 6;
    if (!iSizeBoost) iSizeBoost = 30 * 6;
    RemoveObject(pComboObj);
    }
  pBall->SetSizeBoost(iSizeBoost);
  return (iSizeBoost);
  }

// Zwischenaufruf: Zielrichtung geändert
public func AimingAngle(object pCaller, int iAngle)
  {
  if (pBall) pBall->AimAngle(iAngle);
  return(1);
  }

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  // Feuerball freigeben (wenn noch existent)
  if (pBall)
    {
    CheckCombo(pCaller);
    pBall->SetAngle(iAngle);
    }
  // Objekt löschen
  return(RemoveObject());
  }

// Abbruch der Zielaktion: Feuerball auch löschen
public func AbortAiming(object pCaller)
  {
  if (pBall) pBall->Abort();
  // Rückgabewert 0: Standardaktion (Zauber löschen und Mana rückerstatten)
  return(0);
  }


public func IsAimerSpell() { return(1); }

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("333"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
