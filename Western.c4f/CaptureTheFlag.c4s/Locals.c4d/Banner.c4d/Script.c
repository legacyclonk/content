/*-- Fahne --*/

#strict

local iTeam;
local fFlag;

protected func Initialize()
{
  SetGraphics(0, this(), POS1);	
  SetAction("HangUp");
  fFlag = 1;
  return(1);
}

/* TimerCall */

private func Wind()
{
    //Richtung der Windgeschwindigkeit anpassen
    if(GetAction() S= "Fly") SetDir(BoundBy(GetWind() / 15 + 3, 0, 6));
}

/* Steuerung */

protected func ControlUp(pClonk)
{
    // Nur Teamkameraden
    if(iTeam != CheckTeam(GetOwner(pClonk))) return();
    // Ist die eigene Flagge da?
    if(!fFlag) return(0);
    // Hat er die gegnerische Flagge dabei? (die eigene kann er eh nicht mitnehmen)
    if(!FindObject(POSP, 0, 0, 0, 0, 0, "PortFly", pClonk)) return();
    FindObject(POSP, 0, 0, 0, 0, 0, "PortFly", pClonk)->~Reset(1, pClonk);
    // Punkte geben
    Score(iTeam);
    return(1);
}

public func HangUp()
{
  // Flagge zurückgebracht? :-)
  fFlag=1;
  SetAction("HangUp");
}

protected func ControlDownSingle(pClonk)
{
    // Nur Gegner
    if(iTeam == CheckTeam(GetOwner(pClonk))) return();
    //Fahne nicht mehr da?
    if(!fFlag) return(0);
    //Richtung setzten
    SetDir(0);
    SetAction("HangDown",pClonk);
    return(1);
}

protected func DoHangDown()
{
  // Beim Abhängen muss immer ein Clonk dabeisein	
  if(!FindObject(0, 0, 0, 0, 0, 0, "Push", this())) 
  {
    var iPhase = GetPhase();	  
    SetAction("HangUp"); 
    SetPhase(4-iPhase);
  }
}

protected func FlagStolen()
{
  var pClonk;	
  if(!(pClonk=FindObject(0, 0, 0, 0, 0, 0, "Push", this()))) return(DoHangDown());
  fFlag = 0; // Flagge weg :-(	
  var pFlag = CreateObject(POSP, 0, 0, -1);
  SetColorDw(GetColorDw(), pFlag);
  LocalN("iTeam", pFlag) = iTeam;
  LocalN("pPost", pFlag) = this();
  pFlag->~RejectEntrance(pClonk);
  ObjectSetAction(pClonk, "Walk");
}

private func GetColor()
{
  if(iTeam==1) return(RGB(0,0,255));
  if(iTeam==2) return(RGB(255,0,0));
}
