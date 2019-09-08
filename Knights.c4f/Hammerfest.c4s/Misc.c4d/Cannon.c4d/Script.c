/*-- Kanone --*/

#strict
#include CANN

local iRestoredAngle;

protected func Initialize()
{
  SetAction("Stand");
  SetDir(Random(2));
  SetPhase(2);
}


public func ControlDig(object pClonk)
{
  // Winkel verstellen
  if(GetAction() eq "Stand")
    {
    if(GetPhase() < 5)
      {
      SetPhase(GetPhase()+1);
      Sound("CatapultSet");
      }
    return(1);
    }
}

public func ControlDownSingle(pByObject)
{
  // Steuerung an ziehende Pferde weiterleiten
  var pHorse;
  while (pHorse = FindPullingHorse(pHorse))
    {
    pHorse->~ControlDownSingle(pByObject);
    }
  return(1);
}

public func ControlThrow(pClonk)    // Feuern / Inhalt
{
  // Der Clonk will doch bestimmt nur etwas nachladen: nicht vorzeitig abfeuern
  var pThing;
  if (pThing = pClonk->Contents())
    if (AllowLoad(GetID(pThing)))
      return(0);
  // Angekoppelt
  if(GetAction() ne "Stand") return();
  // Abfeuern wenn mˆglich, sonst Objekt herausnehmen
  return(Fire());
}

/* Maussteuerung */

public func ControlCommand(string szCommand,object pTarget,int iX,int iY)
{
  // Feuern
  if(szCommand eq "MoveTo")
    if(Distance(GetX(),GetY(),iX,iY)>50)
    {
      HaltPushers();
      return(FireAt(iX,iY));
    }
}

private func HaltPushers()
{
  var pClonk;
  // Alle Clonks die die Haubitze anfassen, sollen stoppen
  while(pClonk=FindObject(0,0,0,0,0,0,"Push",this(),0,pClonk))
    SetComDir(COMD_Stop(),pClonk);
}

public func FireAt(int iX,int iY,int fAuto)
{
  var iAngle;
  // Zielwinkel
  iAngle = Angle(GetX(),GetY(),iX,iY);
  // Bei grˆﬂerer Distanz hˆher zielen
  if(Inside(iX-GetX(),+1,+300))
     iAngle -= Abs(iX-GetX())/12;
  if(Inside(iX-GetX(),-300,-1))
     iAngle += Abs(iX-GetX())/12;
  // Zielen
  AimToAngle(iAngle);
  // Feuern
  return(Fire(fAuto));
}

public func AimToAngle(int iAngle)
{
  // Winkel anpassen
  while(iAngle > 180) iAngle-=360;
  // Richtung
  if(iAngle > 0) SetDir(DIR_Right());
  if(iAngle < 0) SetDir(DIR_Left());
  // Zielrichtung
  SetPhase(BoundBy( 12*Abs(iAngle)/180-2, 0,5));
}

/* Aktionen */

public func TurnLeft()
{
  if(GetDir() == DIR_Left()) return(1);
  if(WildcardMatch(GetAction(), "*Drive*"))
    SetAction("DriveTurn");

  SetDir(DIR_Left());
}

public func TurnRight()
{
  if(GetDir() == DIR_Right()) return(1);
  if(WildcardMatch(GetAction(), "*Drive*"))
    SetAction("DriveTurn");

  SetDir(DIR_Right());
}

protected func RestoreAngle()
{
  if(!iRestoredAngle) return(SetPhase(2));
  SetPhase(iRestoredAngle-1);
  iRestoredAngle=0;
}

/* Laden und Feuern */

private func AllowLoad(id idObj)
{
  // Noch jede Menge Platz
  if (ContentsCount() < 9) return(1);
  // Niemals mehr als 10 Objekte laden: Wird gebraucht, wenn aus ControlThrow aufgerufen!
  if (ContentsCount() >= 10) return(0);
  // Nicht mehr als 9 Schieﬂpulver laden (Idiotenschutz)
  if (idObj == GUNP)
  {
    if (ContentsCount(GUNP) == 9)
      return(0);
  }
  // Nicht mehr als 9 Projektile laden, wenn noch Platz f¸r Pulver gebraucht wird
  else
  {
    if (ContentsCount(GUNP) == 0)
      return(0);
  }
  // Laden o.k.
  return(1);
}

public func Fire(bool fAuto)
{
  // Schieﬂpulver und Projektil suchen
  var pGunpowder=FindContents(GUNP), pProjectile=FindOtherContents(GUNP);
  // Schieﬂpulver oder Projektil fehlt
  if (!pGunpowder || !pProjectile) 
  {
    Sound("Click");
    return(0);
  }
  // Schieﬂpulver verbrauchen
  RemoveObject(pGunpowder);

  // Austritt berechnen
  var iX = 20*(GetDir()*2-1);
  var iY = GetPhase()-7;
  var iAngle = BoundBy(GetPhase()*12+45,45,120);
  var iXDir = Sin(iAngle,15*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-10);

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch & Feuer
  for (var i = 0; i < 6; ++i)
    {
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));
    CreateParticle("Fire2", iX+RandomX(-5,+5), iY+RandomX(-5,+5), iXDir, iYDir, RandomX(5,12)*5, RGBa(255,255,255,50));
    }
  /*
  // R¸ckstoﬂ
  var iDir = GetDir();
  SetXDir(-iXDir*2);
  SetDir(iDir);
  */
  // Erfolgreich geschossen
  return(1);
}

/* Status */

public func IsHorseWagon() { return(1); }   // Stimmt zwar nicht, aber muss sein, damit anspannbar

private func FindPullingHorse(pHorse)
{
  while (pHorse = FindObject(0, -100,-100,200,200, OCF_Grab(), 0,0, NoContainer(), pHorse))
    if (GetAction(pHorse) eq "Pull" || GetAction(pHorse) eq "Pull2" || GetAction(pHorse) eq "Pull3" || GetAction(pHorse) eq "Turn")
      if (GetActionTarget(0,pHorse) == this())
        return(pHorse); 
  return(0);
}

/* Verbindung */

public func Connect(pHorse)
{
  CreateObject(CHBM,0,0,GetOwner())->CHBM::Connect(pHorse,this());
  Schedule("SetAction(\"Drive0\");", 1);
  Sound("Connect");
  return(1);
}

public func Disconnect(pFromHorse)
{         
  var pBeam = FindObject(CHBM,0,0,0,0,0,"Connect",pFromHorse);
  if (pBeam) RemoveObject(pBeam);
  SetAction("Stand");
  Sound("Connect");
  return(1);
}

public func Driving()
{
  // Kein Pferd
  if(!FindPullingHorse())
    return(SetAction("Stand"));
  // Bewegung
  if(GetXDir() != 0 && GetAction() ne "Drive1")
    return(SetAction("Drive1"));
  if(GetXDir() == 0 && GetAction() ne "Drive0")
    return(SetAction("Drive0"));
}

// Eigentlich sollen nur Pferde die Kanone ziehen
public func PullCheck()
{
  // Ein Pferd
  if(FindPullingHorse()) return();
  // Bewegung und ein schiebender Clonk
  if(GetXDir() != 0)
    if(FindObject(0, 0,0,0,0, OCF_CrewMember(), "Push", this(), NoContainer()))
      SetXDir();
}
