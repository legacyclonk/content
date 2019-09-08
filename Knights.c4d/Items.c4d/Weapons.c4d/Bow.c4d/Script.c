/*-- Bogen --*/

#strict

/* Aktivieren (zum Zielen per Maussteuerung) */
public func Activate(pClonk)
{
  [$CtrlActivateDesc$|Condition=ShowInContextMenu]
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return(0);
  // Träger nicht bereit zum neuen Zielen
  if (!pClonk->~ReadyToFire())
  {
    if(pClonk->GetProcedure() ne "SCALE") return(0);
    // Beim Klettern nur nach Doppelstop ablegen
    if(!GetPlrDownDouble(pClonk->GetOwner()))
      return(1);
    else
      return(0);
  }
  // Träger: neue Zielaktion setzen
  if (!pClonk->~LoadBow(1))
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
  if (IsAiming())
    { Fire(pClonk); return(1); }
  // Bogen einsetzen
  return(Activate(pClonk));
}

protected func ControlConf(int conf)
{
  var rVal;
  var pClonk = Contained();
  if(!pClonk) return(0);

  if(conf < 0)
  {
    if(pClonk->~BowControlUp(this(), rVal))
      return(rVal);
    else if(IsAiming())
      return(pClonk->AimStdConf(conf));
    else
      return(0);
  }
  else
  {
    if(pClonk->~BowControlDown(this(), rVal))
      return(rVal);
    else if(IsAiming())
      return(pClonk->AimStdConf(conf));
    else
      return(0);
  }
}

public func ControlUp(pClonk)
{
  return(AimUp(pClonk, 5, "ControlConf"));
}

public func ControlDown(pClonk)
{
  return(AimDown(pClonk, 5, "ControlConf"));
}

public func ControlUpdate(pClonk, int comdir)
{
  AimUpdate(pClonk, comdir, 5, "ControlConf");
}

public func ControlLeft(pClonk)
{
  // Träger Zielaktion
  if (IsAiming())
  {
    SetDir(DIR_Left(), pClonk);
    return(1);
  }
  // Keine Spezialsteuerung
  return(0);
}

public func ControlRight(pClonk)
{
  // Träger Zielaktion
  if (IsAiming())
  {
    SetDir(DIR_Right(), pClonk);
    return(1);
  }
  // Keine Spezialsteuerung
  return(0);               
}

public func ControlRightDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming())
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
  if (IsAiming())
  {
    pClonk->StopAiming();
    SetComDir(COMD_Left(), pClonk);
  }
  // Keine Spezialsteuerung
  return(0);
}

/* Wird gerade vom Träger gezielt? */
public func IsAiming() 
{ 
  if(!Contained()) return(false);
    return (Contained()->~IsAiming()); 
}

/* Hardgecodet, für alte Ritter */
private func GetBowOutOld(pClonk, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir)
{
  var iOutDir; if (GetDir(pClonk) == DIR_Left()) iOutDir=-1; else iOutDir=1;
  var iOutPhase = GetPhase(pClonk);
  // X-Austritt  
  iOutX = iOutDir*iOutPhase*2;
  // Y-Austritt
  iOutY = iOutPhase+3; // + Random(9)-4;
  // R-Austritt
  iOutR = iOutX * 13;
  // XDir
  iOutXDir = iOutDir*iOutPhase*3+2;
  // YDir
  if (iOutPhase == 5) iOutYDir=10; else iOutYDir = (iOutPhase-3)*3;
  // Größerer Abstand
  iOutX += iOutXDir*2;
  iOutY += iOutYDir*2;
  // RDir
  iOutRDir = iOutDir*3;
}
public func GetBowOutAngle(pClonk, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
{
  // Zufallsabweichung
  if(fDeviate) { iAngle += Random(11) - 5; }
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 70); 
  iOutYDir = -Cos(iAngle, 70); 
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
public func Fire()
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return(0);
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon()) return(0);
  // Noch beim Nachladen
  if (GetAction() eq "Reload") return(0);
  // Pfeil vom Träger holen
  var pArrow = pClonk->~GetArrow();
  if (!pArrow) return(0);
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(pClonk, 0, BOW1);
  // Austrittsgeschwindigkeit ermitteln
  var iAngle = pClonk->~GetBowAimAngle();
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
    GetBowOutOld(pClonk, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
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
public func FireAngle(iAngle)
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return(0);
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon()) return(0);
  // Noch beim Nachladen
  if (GetAction() eq "Reload") return(0);
  // Pfeil vom Träger holen
  var pArrow = Contained()->~GetArrow();
  if (!pArrow) return();
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(Contained(), 0, BOW1);
  // Austrittsgeschwindigkeit ermitteln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
    GetBowOutAngle(Contained(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
    // Sound
  Sound("Bow");  
  // Nachladen
  SetAction("Reload");
  Contained()->~LoadBow();
  return(1);
}


/* Anzünden: Verbrannte Aktion zur Markierung, dass er unbenutzbar ist */

protected func Incineration()
 {
  //Einfärben für Menüs
  SetClrModulation(RGB(50,50,50),this());
  SetAction("Burned");
  return(1);
 }

/* "Bogen einsetzen" wird nicht im Kontextmenue angezeigt */
private func ShowInContextMenu()
{
 return(false);
}

// Ist ein Bogen
public func IsBow() { return(1); }

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }
