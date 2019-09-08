/*-- Haubitze --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  SetAction("Ready");
  SetPhase(10);
}

/* Steuerung */

protected func ControlConf(int conf)
{
  if(AimStdConf(conf))
    Sound("CatapultSet");
}

public func ControlUp(object clonk)     // Zielen: hoch (klassisch)
{
  [$TxtAimup$|Image=CAN1:2]
  AimUp(clonk, 4, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=CAN1:0]
  AimDown(clonk, 4, "ControlConf");
}

public func ControlDown()
{
  [$TxtAimdown$|Method=JumpAndRun|Image=CAN1:0] // Beschreibung fuer JnR
}

public func ControlUpdate(object clonk, int comdir) // Zielen: JnR
{
  AimUpdate(clonk, comdir, 4, "ControlConf");
}

public func ControlDownSingle() // Sicht zurücksetzen
{
  [$TxtResetview$|Method=None]
}

public func ControlThrow(pClonk)    // Feuern / Inhalt
{
  [$TxtFire$|Image=CAN1:1]
  // Der Clonk will doch bestimmt nur etwas nachladen: nicht vorzeitig abfeuern
  var pThing;
  if (pThing = pClonk->Contents())
    if (AllowLoad(GetID(pThing)))
      return(0);
  // Abfeuern wenn möglich, sonst Objekt herausnehmen
  return(Fire());
}

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

/* Prozesse */

private func HaltPushers()
{
  var pClonk;
  // Alle Clonks die die Haubitze anfassen, sollen stoppen
  while(pClonk=FindObject(0,0,0,0,0,0,"Push",this(),0,pClonk))
    SetComDir(COMD_Stop(),pClonk);
}

public func Fire(bool fAuto)
{
  // Schießpulver und Projektil suchen
  var pGunpowder=FindContents(GUNP), pProjectile=FindOtherContents(GUNP);
  // Schießpulver oder Projektil fehlt
  if (!pGunpowder || !pProjectile) 
  {
    Sound("Click");
    return(0);
  }
  // Schießpulver verbrauchen
  RemoveObject(pGunpowder);

  // Austritt berechnen
  var iX = (GetPhase()+3)*(GetDir()*2-1);
  var iY = GetPhase()-14;
  var iAngle = BoundBy(GetPhase()*5-5,0,90);
  var iXDir = Sin(iAngle,32*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-18);
  
  // Fuer Killverfolgung
  if (GetOCF(pProjectile) & OCF_Alive) pProjectile->SetKiller(GetController());
  pProjectile->SetController(GetController());

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch
  for (var i = 0; i < 6; ++i) 
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));
  // Erfolgreich geschossen
  return(1);
}

/* Zielfunktionen */

public func FireAt(int iX,int iY,int fAuto)
{
  var iAngle;
  // Zielwinkel
  iAngle = Angle(GetX(),GetY(),iX,iY);
  // Bei größerer Distanz höher zielen
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
  SetPhase(BoundBy( 19*Abs(iAngle)/90, 0,18));
}
    
/* Laden */

protected func Collection() { Sound("Connect"); }

protected func RejectCollect(id idObj, object pObj)
{
  return(!AllowLoad(idObj));
}

private func AllowLoad(id idObj)
{
  // Noch jede Menge Platz
  if (ContentsCount() < 9) return(1);
  // Niemals mehr als 10 Objekte laden: Wird gebraucht, wenn aus ControlThrow aufgerufen!
  if (ContentsCount() >= 10) return(0);
  // Nicht mehr als 9 Schießpulver laden (Idiotenschutz)
  if (idObj == GUNP)
  {
    if (ContentsCount(GUNP) == 9)
      return(0);
  }
  // Nicht mehr als 9 Projektile laden, wenn noch Platz für Pulver gebraucht wird
  else
  {
    if (ContentsCount(GUNP) == 0)
      return(0);
  }
  // Laden o.k.
  return(1);
}

/* Forschung */

public func GetResearchBase() { return(CATA); }
