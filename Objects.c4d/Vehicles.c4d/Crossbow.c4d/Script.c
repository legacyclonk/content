/*-- Armbrust --*/

#strict 2

public func ReloadDelay() { return 10; } // Zeit zum Nachladen

/* Initialisierung */

protected func Initialize()
{
  SetAction("Ready");
  SetPhase(15);
}

/* JumpAndRun Steuerung */

protected func FxIntControlStart(object target, int number, int temp, int dir, object controller)
{
  EffectVar(0, target, number) = dir;
  EffectVar(1, target, number) = controller;
}

protected func FxIntControlTimer(object target, int number)
{
  if(!EffectVar(1, target, number)) return -1;
  if(EffectVar(1, target, number)->GetAction() != "Push") return -1;
  if(EffectVar(1, target, number)->GetActionTarget() != this) return -1;

  var phases = GetActMapVal("Length", "Ready");

  var old_phase = GetPhase();
  SetPhase(BoundBy(GetPhase() + EffectVar(0, target, number), 0, phases-1));

  if(old_phase != GetPhase())
    Sound("CatapultSet");
}

private func RemoveControlEffect(int dir)
{
  var count = GetEffectCount("IntControl", this);
  for(var i = 0; i < count; ++ i)
    if(EffectVar(0, this, GetEffect("IntControl", this, i)) == dir)
      return(RemoveEffect("IntControl", this, i));
}

/* Steuerung */

public func ControlConf(int conf)
{
  if(AimStdConf(conf))
    Sound("CatapultSet");
}

public func ControlUp(object clonk)     // Zielen: hoch
{
  [$TxtAimup$|Image=XBO1:2]
  AimUp(clonk, 5, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=XBO1:0]
  AimDown(clonk, 5, "ControlConf");
}

public func ControlDown(object clonk)   // Zielen: runter (JumpAndRun)
{
  [$TxtAimdown$|Method=JumpAndRun|Image=XBO1:0]
}

public func ControlUpdate(object clonk, int comdir)
{
  AimUpdate(clonk, comdir, 5, "ControlConf");
}

public func ControlDownSingle() // Sicht zurücksetzen
{
  [$TxtResetview$|Method=None]
}

public func ControlThrow(pClonk)  // Schießen
{
  [$TxtFire$|Image=XBO1:1]
  return(Fire(0,pClonk));
}

public func ControlCommand(string szCommand,object pTarget,int iX,int iY)
{
  // Feuern
  if(szCommand == "MoveTo")
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
  // Alle Clonks die die Armbrust anfassen, sollen stoppen
  while(pClonk=FindObject(0,0,0,0,0,0,"Push",this,0,pClonk))
    SetComDir(COMD_Stop,pClonk);
}
  
public func Fire(bool fAuto,object pClonk)
{
  // Noch am Nachladen? Nix da.
  if(GetEffect("IntReload", this) ) return true;

  // Pfeilpakete zerlegen, sofern nötig
  SplitPacks();

  // Pfeil suchen
  var pArrow=Contents();
  if (!pArrow) { Sound("Click"); return false; }

  // Austritt berechnen
  var iX = GetPhase()*(GetDir()*2-1);
  var iY = GetPhase()-15;
  var iR = iX*5;
  var iAngle = BoundBy(GetPhase()*5,0,90);
  var iXDir = Sin(iAngle,13*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-13);
  var iRDir = GetDir()*6-3;

  // Pfeil abfeuern
  if (GetOCF(pArrow) & OCF_Alive) pArrow->SetKiller(GetController(pClonk));
  pArrow->SetController(GetController(pClonk));
  Exit(pArrow,iX,iY,iR,iXDir,iYDir,iRDir);
  pArrow->~Launch(this);
  
  // Pfeil trifft Schuetzen fuer 10 Frames nicht
  pArrow->~NoArrowHit(pClonk,10);
  
  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pArrow);
  // Sound
  Sound("Arrow");
  // Nachladen
  AddEffect("IntReload", this, 1, ReloadDelay(), this );
  // Fertig
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
  if(iAngle > 0) SetDir(DIR_Right);
  if(iAngle < 0) SetDir(DIR_Left);
  // Zielrichtung
  SetPhase(BoundBy( 20*Abs(iAngle)/90, 0,19));
}

/* Laden */

protected func Collection(pObj)
{
  // Sound
  Sound("Connect");
  // Pakete zerlegen
  if(pObj->~IsArrowPack() )
    if(!pObj->~Unpack(this) )
      Split2Components(pObj);
  SplitPacks();
  return(1);
}

private func SplitPacks()
{
  var i, obj;
  while(obj = Contents(i++) )
    if(obj->~IsArrowPack() )
      if(!obj->~Unpack(this) )
        Split2Components(obj);
}

/* Nur Pfeile und Pfeilpakete annehmen */

protected func RejectCollect(idDef, pObj)
{
  // Maximal nur 10 Pfeile aufnehmen
  return( ((!pObj->~IsArrow()) && (!pObj->~IsArrowPack())) || (GetArrowCount() >= 10) );
}

/* Pfeile zaehlen */

private func GetArrowCount()
{
  var i, obj, count;
  while(obj = Contents(i ++) )
  {
    if(obj->~IsArrow() ) count ++;
    if(obj->~IsArrowPack() ) count += obj->PackCount();
  }
  return(count);
}

