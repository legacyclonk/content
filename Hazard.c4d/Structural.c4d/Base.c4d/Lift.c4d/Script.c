/*-- Lift --*/

#strict

// Lokale Variable: Feuerfarbe :')
local UpperY, LowerY, WaitComDir, WaitY;
local rgb;

protected func Initialize()
{
  AddEffect("Sparks", this(), 1, 4, this());
  rgb=RGB(40+Random(120), 40+Random(120), 40+Random(120));
  SetAction("Travel");
  UpdateTransferZone();
  return(1);
}

public func FxSparksTimer(target, number, time)
{
	// Funken! :D
	CreateParticle("PSpark", 0, -20, RandomX(-1,1), RandomX(-1,1)-GetYDir()/4, 50+Random(80), rgb, this(), true);
	CreateParticle("PSpark", 0, +20, RandomX(-1,1), RandomX(-1,1)-GetYDir()/4, 50+Random(80), rgb, this(), true);
}

func FlareUp()
{
	for(var i=3; i>0; --i)
		CreateParticle("PSpark", 0, -20, RandomX(-4,4), RandomX(-2,-4), 130+Random(80), rgb, this(), true);
}

func FlareDown()
{
	for(var i=3; i>0; --i)
		CreateParticle("PSpark", 0, +20, RandomX(-4,4), RandomX(2,4), 130+Random(80), rgb, this(), true);
}

/* Steuerung */

public func ControlCommand (string command, object target, int x, int y) {
  // Bewegungskommando vertikal
  if (command eq "MoveTo") {
    if (Inside(x - GetX(),-16,+16)) {
      if (y < GetY())
        return(SetComDir(COMD_Up()));
      if (y > GetY())
        return(SetComDir(COMD_Down()));
    }
  }
  // Andere Kommandos nicht auswerten
}

public func ControlUpdate(object caller, int comdir, bool dig, bool throw)
{
  // Remove horizontal Comdir component
  if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
  if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);

  if(comdir != GetComDir())
    Sound("Click");

  // Clear X/Y-Dir
  SetXDir(0);
  if(!ComDirLike(comdir, COMD_Up) && !ComDirLike(comdir, COMD_Down))
    SetYDir(0);

  // Ab geth.
  SetComDir(comdir);
  SetAction("Travel");
  return(1);
}

public func ControlUp (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    SetComDir(COMD_Up());
    SetXDir(0);
    SetAction("Travel");
  }
  return(1);
}

public func ControlDownSingle (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    SetComDir(COMD_Down());
    SetXDir(0);
    SetAction("Travel");
  }
  return(1);
}

public func ControlLeft (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    SetComDir(COMD_Stop());
    SetYDir(0);
    SetAction("Travel");
  }
  return(1);
}

public func ControlRight (object caller) {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  {
    Sound("Click");
    SetComDir(COMD_Stop());
    SetYDir(0);
    SetAction("Travel");
  }
  return(1);
}

// Without HorizontalFix=1, these should prevent that the lift moves horizontally,
// but they do not. They still move after having pressed double left/right and
// waiting a few frames.
/*public func ControlLeftSingle (object caller) {
  [|Method=None]
  return(1);
}

public func ControlLeftDouble (object caller) {
  [|Method=None]
  return(1);
}

public func ControlRightSingle (object caller) {
  [|Method=None]
  return(1);
}

public func ControlRightDouble (object caller) {
  [|Method=None]
  return(1);
}*/

public func ControlLeftReleased (object caller) {
  [|Method=None]
  return(1);
}

/* Kontakt */

protected func ContactTop () {
  Wait(COMD_Down());
  return(1);
}

protected func ContactBottom () {
  UnstuckClonks();
  Wait(COMD_Up());
  return(1);
}

protected func ContactLeft () {
  Stop();
  return(1);
}

protected func ContactRight () {
  Stop();
  return(1);
}

public func SetYTop (int iNewTop) {
  return(UpperY = iNewTop);
}
  
private func YTop () {
  return(UpperY);
}

/* Kommandos */

private func Stop () {
  SetAction("Travel");
  SetComDir(COMD_Stop());
  SetSpeed();
  return(1);
}

private func Wait (int iComDir) {
  Stop();
  // Aktion Wait ruft als EndCall ResumeTravel auf
  SetAction("Wait"); 

  WaitComDir = iComDir;
  return(1);
}

private func ResumeTravel () { // EndCall von Aktion Wait
  SetAction("Travel");
  SetComDir(WaitComDir);
  return(1);
}

private func Automatic () { // EndCall von Aktion Travel
  // Keine Automatik, wenn ein Clonk mitfährt
  if (Passenger()) return(0); 
  // Zu einem wartenden Clonk hinfahren
  if (SpeedToVertical()) return(1);
  // Eigenständige Bewegung beginnen
  if (SelfTravel()) return(1);
  return(0);
}

private func SelfTravel () {
  // Nur wenn angehalten
  if (GetComDir() != COMD_Stop()) return(0);
  // Eigenständige Bewegung je nach zugelassener Richtung beginnen
  SetComDir(COMD_Up());
  return(1);
}

private func SpeedToVertical () {
  var WaitClonk;
  if(!(WaitClonk=FindObject(0, -50,-1000,100,2000, OCF_CrewMember(),0,0,NoContainer()))) return();
  if(Inside(WaitClonk->GetX(), GetX() - 12, GetX() + 12))
    if(Inside(WaitClonk->GetY(), GetY() - 40, GetY() - 20))
      return();
  if (GetComDir(WaitClonk) != COMD_Stop()) return();
  if(!Inside(GetY(WaitClonk), UpperY-50, LowerY+50)) return();
  SetAction("SpeedToY");
  WaitY = Max(GetY(WaitClonk), YTop()-10);
  return(1);
}

/* Aktion */

private func CheckYTop () { // PhaseCall von Travel
  if (GetY() > YTop()-10) return(0);
  SetPosition(GetX(), YTop()-10);
  ContactTop();
  return(1);
}

private func SpeedToY () { // StartCall von SpeedToY
  SetYDir(BoundBy(WaitY - GetY(), -50, 50));
  if (Inside(WaitY-GetY(),-4,+2)) return(Stop());
  if(GetYDir() < 0) FlareDown();
  else FlareUp();
  return(1);
}

private func UnstuckClonks () { // Sucht Clonks in der SolidMask und setzt sie auf die Platte
  var pClonk;
  while(pClonk = FindObject(0,-25,0,50,25, OCF_CrewMember, 0,0, NoContainer(), pClonk))
    if(Stuck(pClonk))
      SetPosition(GetX(pClonk), GetY(), pClonk);
}

/* Status */

private func Passenger () {
  return( FindObject(0, -24,-13,48,16, OCF_CrewMember(),0,0,NoContainer()) );
}

/* Transfer */

protected func UpdateTransferZone () {
  // Obergrenze ermitteln
  var y1 = -20, y2 = +20;
  while(GetY() + y1 > 5 && !GBackSolid(0, y1))
    y1-=5;
  // Untergrenze ermitteln
  while(GetY() + y2 < LandscapeHeight()-5 && !GBackSolid(0, y2+15))
    y2+=5;
  // Transferzone setzen
  SetTransferZone(-23,y1,48,y2-y1 +10);
  // Ober- und Untergrenze speichern
  UpperY = GetY() + y1;
  LowerY = GetY() + y2;
  return(1);
}

protected func ControlTransfer (object pObj, int iX, int iY) {
  // Durchgang am Boden des Fahrstuhlschachts: nicht warten
  if(Inside(GetY(pObj) - LowerY, -10, +10))
    if(Inside(iY - LowerY, -10, +10))
      return();

  // Zielposition
  // Maximale Obergrenze
  iY = Max(iY, UpperY);

  // Fahrstuhl mit Clonk an Zielposition: anhalten, erledigt
  if(Inside(iY - GetY(), -5, +5))
    if(Passenger())
      return(0,Stop());

  // Fahrstuhl noch nicht beim Clonk: warten
  if(!Inside(GetY() - GetY(pObj) -15, +15))
    return(1);

  // Fahrstuhl nur in der Mitte anfassen
  if(!Inside(GetX() - GetX(pObj), -15, +15))
    return(AddCommand(pObj,"MoveTo",this(),0,0,0,15));

  // Fahrstuhl anfassen
  if(GetAction(pObj) ne "Push" || GetActionTarget(0, pObj) != this())
    return(AddCommand(pObj,"Grab",this()));

  // Fahrstuhl zur Zielposition steuern
  if(iY < GetY())
    return(SetComDir(COMD_Up()));  
  return(SetComDir(COMD_Down()));
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetYTop(%d)", UpperY);
	extra[GetLength(extra)] = Format("LocalN(\"rgb\")=%d", rgb);
}
