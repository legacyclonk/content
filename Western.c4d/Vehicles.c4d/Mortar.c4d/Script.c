/*-- Mörser --*/

#strict

#include CANN

/* Initialisierung */

protected func Initialize()
{
  SetAction("Ready");
  SetDir(Random(2));
  SetPhase(4);
}

/* Laden und Feuern */

public func ControlThrow(pClonk)    // Feuern / Inhalt
{
  [$TxtFire$|Image=CAN1:1]
  // Der Clonk will doch bestimmt nur etwas nachladen: nicht vorzeitig abfeuern
  var pThing, message;
  if(pThing = pClonk->Contents())
    if((message = AllowLoad(pThing->GetID())) == 0)
      return(0);
  // Abfeuern wenn möglich, sonst Objekt herausnehmen
  if(!Fire()) {
    if(message) {
      Message(message, this());
      Sound("Error");
      return(1);
    }
    return(0);
  }
  return(1);
}

public func AimToAngle(int iAngle)
{
  // Winkel anpassen
  while(iAngle > 180) iAngle-=360;
  // Richtung
  if(iAngle > 0) SetDir(DIR_Right());
  if(iAngle < 0) SetDir(DIR_Left());
  // Zielrichtung
  SetPhase(BoundBy(3*Abs(iAngle)/10,0,7));
}

public func Fire(bool fAuto)
{
  // Noch am Nachladen
  if(GetEffect("IntReload", this())) return(1);
  // Schießpulver und Projektil suchen
  var pGunpowder, pProjectile;
  pGunpowder  = FindContents(GUNP);
  pProjectile = FindOtherContents(GUNP);
  // Schießpulver oder Projektil fehlt
  if (!pGunpowder || !pProjectile) {
    Sound("Click");
    return(0);
  }
  // Schießpulver verbrauchen
  RemoveObject(pGunpowder);

  // Austritt berechnen
  var i, iX, iY, iAngle, iRandom, iXDir, iYDir;
  iAngle = GetPhase()*5*(GetDir()*2-1);
  iX =  Sin(iAngle,16)+3*(GetDir()*2-1)+2*GetDir();
  iY = -Cos(iAngle,16)+2;
  iRandom = RandomX(-10,10);
  iXDir =  Sin(iAngle,160+iRandom);
  iYDir = -Cos(iAngle,160+iRandom);

  // Besitzer setzen
  SetOwner(GetController(), pProjectile);
  // Da dies auch eine Kanone ist, Kanonenkugel aktivieren
  if(GetID(pProjectile) == CABL) LocalN("fActivated",pProjectile) = 1;

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),0,0,+30);
  // Damit sich die Kanonenkugeln besser streuen lassen
  SetXDir(iXDir,pProjectile,10);
  SetYDir(iYDir,pProjectile,10);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch & Feuer
  for(i = 0; i < 6; ++i) {
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));
    CreateParticle("Fire2",iX+RandomX(-5,+5),iY+RandomX(-5,+5),iXDir/10,iYDir/10,RandomX(5,12)*5,RGBa(255,255,255,50));
  }
  // Erfolgreich geschossen
  AddEffect("IntReload", this(), 1, 50, this());
  return(1);
}

private func AllowLoad(id idObj)
{
  // Nur Kugeln und Pulver	
  if(idObj != GUNP && idObj != CABL)
    return(Format("$TxtPossibleContents$", GetName(0, GUNP), GetName(0, CABL)));		
  // Noch jede Menge Platz
  if (ContentsCount() < 4) return(0);
  // Niemals mehr als 8 Objekte laden: Wird gebraucht, wenn aus ControlThrow aufgerufen!
  //if (ContentsCount() >= 8) return(0);
  // Nicht mehr als 4 Schießpulver laden
  if(idObj == GUNP) {
    if(ContentsCount(GUNP) >= 4)
      return(Format("$TxtFull$", ContentsCount(GUNP), GetName(0, GUNP)));
  }
  // Nicht mehr als 4 Projektile laden
  else if(ContentsCount(CABL) >= 4)
      return(Format("$TxtFull$", ContentsCount(CABL), GetName(0, CABL)));
  // Laden o.k.
  return(0);
}

// Nur Pulver und Kugeln laden
public func RejectCollect(id idObj, object pObj) { return(AllowLoad(idObj) != 0); }

/* Status */

public func IsHorseWagon() { return(1); }   // Stimmt zwar nicht, aber muss sein, damit anspannbar

private func FindPullingHorse(pHorse)
{
  return(FindObject2(Find_ActionTarget(this()),Find_Or(Find_Action("Pull"),Find_Action("Pull2"),Find_Action("Pull3"),Find_Action("Turn"))));
}

public func TurnLeft()
{
  if(GetDir() == DIR_Left()) return(0);
  SetDir(DIR_Left());
}

public func TurnRight()
{
  if(GetDir() == DIR_Right()) return(0);
  SetDir(DIR_Right());
}

/* Verbindung */

public func Connect(pHorse)
{
  CreateObject(CHBM,0,0,GetOwner())->CHBM::Connect(pHorse,this());
  Sound("Connect");
  return(1);
}

public func Disconnect(pFromHorse)
{         
  var pBeam = FindObject(CHBM,0,0,0,0,0,"Connect",pFromHorse);
  if (pBeam) RemoveObject(pBeam);
  Sound("Connect");
  return(1);
}

public func PullCheck()
{
  // Ein Pferd
  if(FindPullingHorse()) return();
  // Bewegung und ein schiebender Clonk
  if(GetXDir() != 0)
    if(FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",this(),NoContainer()))
      SetXDir(BoundBy(GetXDir(), -2,2)); // Nur leichte Bewegung
}

/* Forschung */

public func GetResearchBase() { return(CCAN); }