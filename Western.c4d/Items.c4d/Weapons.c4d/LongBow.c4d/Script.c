/*-- Bogen --*/

#strict

/* Aktivieren (zum Zielen per Maussteuerung) */
public func Activate(pClonk)
{
  [$CtrlActivateDesc$]
  // Unvollständig
  if(!(GetOCF() & OCF_Fullcon())) return();
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return(0);
  // Träger nicht bereit zum neuen Zielen
  if (!pClonk->~ReadyToFire()) return();
  // Träger: neue Zielaktion setzen
  if (!pClonk->~LoadBow(this(), 1))
  {
    if (!pClonk->~AimBow()) 
      return();
    SetPhase(7, pClonk);
  }
  // Erfolg
  return(1);
}

/* Spezialsteuerung durch Träger */
public func ControlThrow(pClonk)
{
  // Träger hat gezielt
  if (IsAiming(pClonk))
    { Fire(pClonk); return(1); }
  // Bogen einsetzen
  return(Activate(pClonk));
}

/* Zielen */
protected func ControlConf(int conf, object controller)
{
  var rVal;
  if(conf < 0)
  {
    if(controller->~BowControlUp(this(), rVal))
      return(rVal);
    else if(IsAiming())
      return(controller->AimStdConf(conf));
    else
      return(0);
  }
  else
  {
    if(controller->~BowControlDown(this(), rVal))
      return(rVal);
    else if(IsAiming())
      return(controller->AimStdConf(conf));
    else
      return(0);
  }
}

public func ControlUp(pClonk)
{
  // GetID() geht nicht, weil wir keinen Objektkontext haben
  return(AimUp(pClonk, 5, "ControlConf", BOW2));
}

public func ControlDown(pClonk)
{
  // GetID() geht nicht, weil wir keinen Objektkontext haben
  return(AimDown(pClonk, 5, "ControlConf", BOW2));
}

public func ControlUpdate(pClonk, int comdir)
{
  // GetID() geht nicht, weil wir keinen Objektkontext haben
  AimUpdate(pClonk, comdir, 5, "ControlConf", BOW2);
}

public func ControlLeft(pClonk)
{
  if(!IsAiming(pClonk)) return();
  if(pClonk->~IsRiding())
    {
    GetActionTarget(0,pClonk)->~ControlLeft(pClonk);
    return(1);
    }
  if(GetDir(pClonk))
    SetDir(0,pClonk);
  return(1);
}

public func ControlRight(pClonk)
{
  if(!IsAiming(pClonk)) return();
  if(pClonk->~IsRiding())
    {
    GetActionTarget(0,pClonk)->~ControlRight(pClonk);
    return(1);
    }
  if(!GetDir(pClonk))
    SetDir(1,pClonk);
  return(1);
}

// Bei Graben soll der Köcher durchgewechselt werden.
public func ControlDig(pClonk)
{
  var pObj;
  // Träger Zielaktion
  if(pObj = FindContents(QUIV,pClonk)) {
    pObj->~Activate(pClonk);
    // Kein "return(0);", ansonsten wird der Köcher nicht durchgewechselt
    return(1);
  }
  // Keine Spezialsteuerung
  return(0);               
}

public func ControlRightDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming(pClonk))
  {
    pClonk->StopAiming();
    SetComDir(COMD_Right(), pClonk);
  }
  // Keine Spezialsteuerung
  return(0);
}

public func ControlLeftDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming(pClonk))
  {
    pClonk->StopAiming();
    SetComDir(COMD_Left(), pClonk);
  }
  // Keine Spezialsteuerung
  return(0);
}

/* Wird gerade vom Träger gezielt? */
public func IsAiming(pClonk) 
{ 
  return (pClonk->~IsAiming()); 
}

public func GetBowOutAngle(pClonk, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
{
  // Zufallsabweichung
  if(fDeviate) { iAngle += Random(11) - 5; }
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 100); 
  iOutYDir = -Cos(iAngle, 100); 
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25); 
  iOutY = -Cos(iAngle, 25); 
  // Pfeildrehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig 
  return(1);
} 

/* Pfeil abfeuern */
public func Fire(pClonk)
{
  // Träger zielt nicht, dann zielen
  if (!IsAiming(pClonk))
    { return(1, Activate(pClonk)); }  
  // Kein Träger?
  if(!pClonk) return(1);
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(1); }
  // Pfeil vom Träger holen
  var pArrow = pClonk->~GetArrow();
  if (!pArrow) return(1);
  // Austrittsgeschwindigkeit ermitteln
  var iAngle = pClonk->~GetBowAimAngle();
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
    ;//GetBowOutOld(pClonk, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(pClonk->GetOwner());
    // Pfeil abfeuern
  pClonk->Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
  // Sicht verfolgen
  if (GetPlrView(GetController(pClonk)) != 2)
    SetPlrView(GetController(pClonk), pArrow);
  // Sound
  Sound("Bow");
  // Nachladen
  SetAction("Reload");
  pClonk->~LoadBow();
  // Fertig!
  return(1);
}

/* Pfeil abfeuern */
public func FireAngle(pClonk, iAngle)
{
  // Kein Träger?
  if(!pClonk) return(0);
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Pfeil vom Träger holen
  var pArrow = pClonk->~GetArrow();
  if (!pArrow) return();
  // Austrittsgeschwindigkeit ermitteln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
//    DefinitionCall(BOW1, "GetBowOutAngle", pClonk, iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
return();
  // Besitzer des Projektils setzen
  pArrow->SetOwner(pClonk->GetOwner());
    // Pfeil abfeuern
  pClonk->Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
    // Sound
  Sound("Bow");  
  // Nachladen
  pClonk->~LoadBow();
  return(1);
}

public func ControlCommand(pClonk,iX,iY)
{
  // noch am Nachladen
  if(pClonk->~IsLoading()) return(1);
  // nicht am Zielen?
  if(!pClonk->~IsAiming()) return(0);

  pClonk->~FireBowAt(iX, iY, true);

  return(1);
}

// Ist von nun an vor der Sehne zu erforschen (kleine Hilfe für Einsteiger)
public func GetResearchBase() { return(); }

// Ist ein Bogen
public func IsBow() { return(1); }

// Kein Ambosprodukt
public func IsAnvilProduct() { return(0); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }
