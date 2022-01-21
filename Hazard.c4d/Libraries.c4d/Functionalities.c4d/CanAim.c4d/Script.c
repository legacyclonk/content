#strict 2

/* Dieses Objekt stellt die Grundfunktionalität des Zielens für den Hazardclonk
     bereit, lso das Zielen mit dem Zielkreuz. SquatAiming (Dreimal-Stopp) ist
     hier nicht mit drin.
     Damit ein Clonk diese Funktionalität benutzen kann, muss er bei ReadyToFire
      am Ende return(_inherited()); aufrufen sowie falls AimAngle() überladen werden
     soll den Script hier berücksichtigen.
	 
     Ansonsten um Zielen zu steuern bei den Control*-Funktionen jeweils
     if(ControlAim("ControlUpDouble")) return(1); (im Beispiel von ControlUpDouble)
     aufrufen. Und zwar bei -Update, -Up, -UpDouble, -Down, -Left, -Right,
     -LeftDouble, -RightDouble, -Command, -Contents.

     Oder beim Hazard abgucken.
*/

local crosshair;

static const AIM_Step = 6;
static const AIM_Max = 150;

public func AimStep()	{ return AIM_Step; }
public func AimMax()	{ return AIM_Max; }

// Muss Aktion "Aim" haben (und "AimLow")

protected func ControlAim(string command) {

	if(!IsAiming()) return false;
	
	else if(command == "ControlUpdate")
		AimUpdate(this(), Par(1), 1, "ControlConf");

	else if(command == "ControlUp" || command == "ControlUpDouble")
		AimUp(this(), 1, "ControlConf");

	else if(command == "ControlDown")
		AimDown(this(), 1, "ControlConf");
		
	else if(command == "ControlLeft") {
		if(GetDir() == DIR_Left) return false;
	    SetDir(DIR_Left);
	    UpdateAiming();
	}
	
	else if(command == "ControlRight") {
		if(GetDir() == DIR_Right) return false;
	    SetDir(DIR_Right);
	    UpdateAiming();
	}
	
	else if(command == "ControlLeftDouble" || command == "ControlRightDouble") {
		StopAiming();
	}
	
	else if(command == "ControlCommand") {
		return DoMouseAiming(Par(2),Par(3));
	}
	
	else if(command == "ControlContents") {

		var pTarget = FindContents(Par(1));
		ChangeWeapon(pTarget);
		return false;

	}

	
	return true;
}

/* Zielen */

public func IsAiming () { 
	if(Contained()) return(Contained()->~IsAiming());
	
	return(WildcardMatch(GetAction(), "Aim*"));
}

public func ReadyToAim() {      // bereit zum Zielen
   // In Gebäude
   if(Contained()) return(Contained()->~ReadyToAim());
   // Nur beim Laufen
   if(GetProcedure(this) == "WALK") return true;
}

public func ControlConf(int conf)
{
  // Feineres Zielen mit JnR-Steuerung möglich
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming(conf * AimStep() / 3);
  else
    DoAiming(conf * AimStep());
}

public func DoAiming(int iChange) {
  //zielen wir überhaupt?
  if(!IsAiming())
    return;

  var angle = Abs(crosshair->GetAngle()) + iChange;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AimStep();

  // Winkel wird zu groß?
  if(angle > AimMax() || angle < 0)
    return;

  if(GetDir() == DIR_Left)
    angle = 360-angle;
  crosshair->SetAngle(angle);

  UpdateAiming();
}

public func DoMouseAiming(int iTx, int iTy) {
	if(!Contained()) {

		var iAngle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);

		if(iAngle > 0)
			SetDir(DIR_Right);
		else
			SetDir(DIR_Left);

		//Winkel wird zu groß?
		iAngle = BoundBy(iAngle,-AimMax(),+AimMax());

		crosshair->SetAngle(iAngle);
		UpdateAiming();
		//Wichtig: Waffe updaten
		EffectCall(this,LocalN("wpneffect",this),"Timer");

		// Fertig, Feuern!
		if(!(Contents()->~IsRecharging()) && !(Contents()->~IsShooting()))
			FireAimWeapon();
		return true;
	}
	return false;
}

public func UpdateAiming() {
  if(!IsAiming())
    return;

  var a = crosshair->GetAngle();

  //Position des Attachvertex so verschieben, dass das Kreuz von der Waffe aus ist
  UpdateVertices();

  if((GetDir() == DIR_Left && a > 0) ||
     (GetDir() == DIR_Right && a < 0))
    crosshair->ChangeDir();
  
  SetPhase(AimAngle2Phase(a));
}

public func StopAiming() {      // Zielen beenden
  SetComDir(COMD_Stop);
  SetAction("Walk");
  this()->~CheckArmed();
  Sound("Grab");
  
  DeleteCrosshair();
  RemoveEffect("UpdateAiming", this);
 
  //Vertices zurücksetzen
  ResetVertices();
  
  // Callback
  if(Contents(0)) Contents(0)->~AimStop();
  return(1);
}

public func FxUpdateAimingTimer(object pTarget, int iNr)
{
  UpdateAiming();
  return true;
}

public func StartAiming() {     // Anfangen zu zielen
  if(Contained()) return(Contained()->~StartAiming());

  SetXDir(0);
  
  SetAction("Aim");
  
  // Aim low if possble
  if(Contents(0)->~GetFMData(FM_Aim) == 2 || Contents(0)->~GetFMData(FM_Aim) == -1)
    if(GetActMapVal("Name","AimLow",GetID()))
      SetAction("AimLow");

  SetPhase(AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  
  if(!GetEffect("UpdateAiming", this))
    AddEffect("UpdateAiming", this, 1, 1, this);

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

/* Interne Funktionen */

private func InitCrosshair() {
  crosshair = CreateObject(HCRH,0,0,GetOwner());
  crosshair->Init(this());
  crosshair->SetAngle(90);
}

private func DeleteCrosshair() {
  if(crosshair) crosshair->RemoveObject();
}

private func UpdateVertices() {
  var x,y,r;
  this->~WeaponAt(x,y,r);
  SetVertex(0,0,x/1000 * (GetDir()*2-1),0,2);
  SetVertex(0,1,y/1000,0,2);
}

private func ResetVertices() {
  SetVertex(0,0,0);
  SetVertex(0,1,0);
}

private func AimAngle2Phase(int angle) {
  angle = Normalize(angle,-180);
  var phases = GetActMapVal("Length",GetAction());
  var newphase = Min( phases-1, phases*Abs(angle)/AimMax() );
  return (newphase);
}

private func FireAimWeapon() {
	this->~Control2Contents("ControlThrow");
}

private func ChangeWeapon(object pTarget) {
	// Zielaktion abbrechen (Spezial: außer wenn mit anwählbarem 
	// Objekt auch gezielt werden kann...)
	// Zielaktion anpassen
	if(pTarget->~IsWeapon())  {
		var phase = GetPhase();
		if(pTarget->~GetFMData(FM_Aim) > 0)
			SetAction("AimLow");
		else
			SetAction("Aim");
		
		SetPhase(phase);
	}
	else {
		StopAiming();
	}
}

/* Overloads */

public func Destruction() {
	DeleteCrosshair();
	return _inherited(...);
}

public func ReadyToFire() {
 
	if(WildcardMatch(GetAction(), "Aim*")) return(true);
 
	return(_inherited());
}

public func AimAngle(int iMaxAngle, int iRange) { // Zielwinkel bestimmen
	var angle;
	var x,y,r;
	this->~WeaponAt(x,y,r);

	if(!IsAiming())
		angle = (90+r)*(GetDir()*2-1);
	else
		angle = crosshair->GetAngle();
  
    if(iMaxAngle) {
		var target = this->~GetTarget(angle,iMaxAngle,iRange);
		if(target)
			angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
	}

	return(angle);
}
