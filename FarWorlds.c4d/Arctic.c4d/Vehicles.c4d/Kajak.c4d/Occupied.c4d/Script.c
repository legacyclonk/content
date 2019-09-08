/*-- Besetztes Kajak --*/

#strict

local turn_end_dir;

protected func Initialize()
{
  turn_end_dir = -1;
}

/* Steuerung */

private func TurnEnd()
{
  if(turn_end_dir != -1)
  {
    if(turn_end_dir == COMD_Stop) ComStop();
    if(turn_end_dir == COMD_Left) ComLeft();
    if(turn_end_dir == COMD_Right) ComRight();
    turn_end_dir = -1;
  }
}

private func ComLeft()
{
  if(!InLiquid()) return(false);
  if(GetAction() S= "Turn") return(turn_end_dir = COMD_Left);   
  if(GetDir() != DIR_Left) SetAction("Turn");
  if(GetAction() S= "Stop") SetAction("Paddle");
  SetComDir(COMD_Left);
  SetDir(DIR_Left);
}

private func ComRight()
{
  if(!InLiquid()) return(false);
  if(GetAction() S= "Turn") return(turn_end_dir = COMD_Right);
  if(GetDir() != DIR_Right) SetAction("Turn");
  if(GetAction() S= "Stop") SetAction("Paddle");
  SetComDir(COMD_Right);
  SetDir(DIR_Right);
}

private func ComStop()
{
  if(GetAction() S= "Turn")
    return(turn_end_dir = COMD_Stop);
  else
    if (GetAction() ne "Throw") 
      SetAction("Stop");
}

protected func ContainedLeft(clonk) 
{
  [$Links$]
  // Steuerung durch Clonk: eigene Kommandos löschen
  if (clonk) ClearCommand();
  // JnR ermitteln
  var jnr; if (clonk) jnr = GetPlrJumpAndRunControl(clonk->GetController());
  // Steuerung durch Script-Kommando oder klassische Steuerung
  if (!clonk || !jnr)
    // Dauerhaft nach links fahren
    ComLeft();
  return(1);
}

protected func ContainedRight(clonk) 
{
  [$Rechts$]
  // Steuerung durch Clonk: eigene Kommandos löschen
  if (clonk) ClearCommand();
  // JnR ermitteln
  var jnr; if (clonk) jnr = GetPlrJumpAndRunControl(clonk->GetController());
  // Steuerung durch Script-Kommando oder klassische Steuerung
  if (!clonk || !jnr)
    // Dauerhaft nach rechts fahren
    ComRight();
  return(1);
}

protected func ContainedDown(object clonk) {
  [$Stop$|Method=Classic]
  if(!GetPlrJumpAndRunControl(clonk->GetController()))
  {
    ClearCommand();
    ComStop();
  }
  return(1);
}

protected func ContainedUpdate(object clonk, int comdir, bool dig, bool throw)
{
  if(comdir == COMD_UpRight || comdir == COMD_DownRight || comdir == COMD_Right)
    ComRight();
  if(comdir == COMD_UpLeft || comdir == COMD_DownLeft || comdir == COMD_Left)
    ComLeft();
  if(comdir == COMD_Up || comdir == COMD_Down || comdir == COMD_Stop)
    ComStop();
}

protected func ContainedUpDouble(pClonk) 
{
  [$Aussteigen$]
  // Kommandos zurücksetzen
  ClearCommand();
  // Im Wasser: Clonk springt raus
  if(InLiquid()) 
  {
    Exit(pClonk,0,+5,0,(GetDir()*2-1)*2,-3,0 );
    ObjectSetAction(pClonk,"Jump");
    SetDir(GetDir(),pClonk);
    SetYDir(5);
  }
  // An Land: Clonk steigt normal aus
  else 
    Exit(pClonk,0,+5,0,0,0,0);
  // Verwandeln
  ChangeDef(KAJK);
  return(1);
}

protected func ContainedThrow(pClonk) {
  [$Werfen$]
  ClearCommand();
  // Wenn der aufrufende Clonk eine Harpune bei sich trägt...
  if (DefinitionCall(GetID(Contents(0,pClonk)),"IsHarpoon"))
    // ...soll diese geworfen werden
    return (SetAction("Throw"));
  // Ansonsten das Aktivieren-Menü öffnen (aktivierte Gegenstände werden dann direkt vom Clonk aufgenommen)
  if (ContentsCount() >= GetDefCoreVal("CollectionLimit", "DefCore"))
    return (AddCommand(pClonk,"Activate",0,0,0,this()));
  return(0);
}

func ControlCommand(szCommand, pTarget, iTX, iTY) 
{
  if (szCommand eq "MoveTo" || szCommand eq "Follow") {
    if(InLiquid()) {
      SetCommand (this (), "MoveTo", pTarget, iTX, iTY);
      AddCommand (this (), "Call", this(), iTX, 0, 0, 0, "ExecMove");
      return (1);
    } else {
      ContainedUpDouble(GetDriver());
      return (0);
    }
  }
  if (szCommand eq "Enter") return (0);
  if (szCommand eq "Exit") return (0);
  if (szCommand eq "Grab")  return (0);
  if (szCommand eq "Get") {
    // Zu holendes Objekt ist nicht in diesem Kajak (und es handelt sich nicht um ein Kommando zum Öffnen des Get-Menüs)
    if ((Contained(pTarget) != this()) && (pTarget != this()))
      // Erstmal aussteigen
      ContainedUpDouble(GetDriver());
    return (0);
  }
  if (szCommand eq "Throw") {
    if(DefinitionCall(GetID(pTarget),"IsHarpoon")) return(SetAction("Throw"));
    ContainedUpDouble(GetDriver());
    return (0);
  }
  return (0);
}

func ExecMove(me, iTX) 
{
  var x1 = Min (GetX (), GetX() + GetXDir (0, 40));
  var x2 = Max (GetX (), GetX() + GetXDir (0, 40));
  // Am Ziel
  if (Inside (iTX, x1, x2)) if (GetComDir()!= COMD_Stop()) return(ComStop());
  // Hindernis im Weg, vermutlich Land: na dann mal rausspringen
  if ((iTX < x1) && (GetContact(0, -1) & CNAT_Left)) ContainedUpDouble(GetDriver());
  if ((iTX > x2) && (GetContact(0, -1) & CNAT_Right)) ContainedUpDouble(GetDriver());    
  // Nach rechts bzw. links fahren
  if (iTX > x2) if (GetComDir() != COMD_Right()) ContainedRight();
  if (iTX < x1) if (GetComDir() != COMD_Left())  ContainedLeft();
  return(1);
}

/* Kontakt */

protected func ContactLeft()  { ComStop(); }
protected func ContactRight() { ComStop(); }

/* Bewegung */

protected func Paddling() {
  if (GetCommand () eq "MoveTo") ExecMove(this(), GetCommand (0, 2));
  // Fahrer überprüfen
  if (!GetDriver())
    return( ExitContents(), ChangeDef(KAJK) );
  if (InLiquid()) {
    // Geräusch
    Sound("KajakPaddle");
    // Vortrieb
    if (GetComDir()==COMD_Right()) SetXDir(+20);
    if (GetComDir()==COMD_Left())  SetXDir(-20);
  }
}

protected func Stopping() {
  // Fahrer überprüfen
  if (!GetDriver())
    return( ExitContents(), ChangeDef(KAJK) );
  SetComDir(COMD_Stop);
}

/* Werfen */

protected func Throwing(pClonk,pObj) {
  var iX,iY,iR,iXDir,iYDir,iRDir;
  // Kein werfender Clonk
  if (!pClonk) if(!(pClonk=GetDriver())) return();
  // Angegebenes Objekt... 
  if (!pObj)
    // ...oder erstes Inhaltsobjekt des Clonks
    if (!(pObj=Contents(0,pClonk))) 
      // Kein Objekt!
      return();
  // Objekt auswerfen
  if(!DefinitionCall(GetID(pObj),"IsHarpoon"))
  {
    // Standardwurf  
    iX = 0; if (!GetDir()) iX = -iX;
    iY = -10;
    iR = Random(360);
    iXDir = GetPhysical("Throw") / 25000; if(!GetDir()) iXDir = -iXDir;
    iYDir = -GetPhysical("Throw") / 25000;
    iRDir = Random(40) - 20;
  }
  else
  {
    // Speerwurf
    iX = 10; if (!GetDir()) iX = -iX;
    iY = -6;
    iR = 150; if(!GetDir()) iR = 180 - iR;
    iXDir = 5; if(!GetDir()) iXDir = -iXDir;
    iYDir = -2;
    iRDir = 4; if(!GetDir()) iRDir = -iRDir;
  }
  Exit(pObj, iX, iY, iR, iXDir, iYDir, iRDir);
  return(1);
}

/* Eingang */

protected func ActivateEntrance(pObj) {
  var pClonk;
  // Aktivierte Gegenstände soll der Clonk direkt aus dem Boot nehmen
  if (!(GetOCF(pObj)&OCF_CrewMember()))
    if (Contained(pObj)==this())
      if (pClonk=FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this()))
        return(SetCommand(pClonk,"Get",pObj));
  // Sonstiger Eingang/Ausgang nicht möglich
}

/* Eintritt in Gebäude */

protected func Entrance(pContainer) 
{
  var pDriver=GetDriver();
  ContainedUpDouble(pDriver);
  Enter(pContainer,pDriver);
}

/* Aktionen */

private func ExitContents() {
  for(var i=ContentsCount();i>0;--i)
    Exit(Contents(i-1),0,+5);
}
  
private func GetDriver() {  
  return(FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this()));
}

private func ClearCommand() { SetCommand(this(),"None"); }
  
/* Status */

public func IsOccupied() { return(1); }

/* Kontextmenü */

protected func ContextJumpOut(object pClonk)
{
  [$Aussteigen$|Image=KAJK|Condition=IsDriver]
  pClonk->ContainedCall("ContainedUpDouble", this());
}

protected func IsDriver(object pClonk)
{
  return (pClonk->Contained() == this());
}
