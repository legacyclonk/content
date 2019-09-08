/*-- Fahne --*/

#strict

local iTeam;
local pPost;
local fCollect;

protected func Initialize()
{
  // Grafik setzten
  SetGraphics(0, this(), POS1);
  return(1);
}

protected func RejectEntrance(object pIntoObj)
{
  // Nur von Crewmembern einsammeln lassen
  if(GetOCF(pIntoObj) & OCF_CrewMember)
  {
    // Beim eigenen Team: Flagge zurück an den Mast
    if(CheckTeam(GetOwner(pIntoObj))==iTeam) return(Reset());
    // Beim gegner Team: Flagge aufnehmen
    SetAction("PortFly", pIntoObj); SetObjectLayer(pIntoObj);
    AddEffect("PortFlag", pIntoObj, 1, 1, this());
    // Und Logmeldung ausgeben
    if(iTeam==1) ExtraLog("$MsgFlagStolenB$", GetPlayerName(GetOwner(pIntoObj)));
    else ExtraLog("$MsgFlagStolenR$", GetPlayerName(GetOwner(pIntoObj)));
    return(1);
  }
  return(!fCollect);
}

// An den Mast zurückkehren
public func Reset(fScore, pClonk)
{
  // Mast benachrichtigen
  pPost->~HangUp();
  // Kein Punkte? Dann einfache Zurückmeldung
  if(!fScore)
  {
    if(iTeam==1) ExtraLog("$MsgFlagReturnB$");
    else ExtraLog("$MsgFlagReturnR$");
  }
  // Bei Punkt: Punktemeldung
  else
  {
    if(iTeam==1) ExtraLog("$MsgPointR$", GetPlayerName(GetOwner(pClonk)));
    else ExtraLog("$MsgPointB$", GetPlayerName(GetOwner(pClonk)));    
  }
  // Flaggenstatus updaten
  if(iTeam==1) stateleft=1;
  else stateright=1;
  // Scoarboard updaten
  ActualizeScoreboard();
  // Tragbares Flaggenobjekt wird nun nicht mehr gebraucht
  RemoveObject();
}

/* TimerCall */

local iTime;

private func Wind()
{
    //Richtung der Windgeschwindigkeit anpassen
    if(GetAction() S= "Fly")
    {  
       SetDir(BoundBy(GetWind() / 15 + 3, 0, 6));
       // Und bei zu langen Zeit wieder an den Mast hängen
       if(GetActTime()>35*60*2 && !GetActionTarget()) return(Reset());      
    }
    return();
}

// Der Trageeffekt
func FxPortFlagStart(pTarget, iNumber, fTmp)
{
  if(fTmp) return();
  // Flaggenstatus updaten
  if(iTeam==1) stateleft=2;
  else stateright=2;
  ActualizeScoreboard();
  // Spielernamen in das Scoreboard eintragen
  SetScoreboardData(SBRD_Caption, ScoreboardCol(CVRM),       "{{SBFL}}",              ScoreboardCol(CVRM));
  if(CheckTeam(GetOwner(pTarget))==1)
       SetScoreboardData(ScoreboardCol(TEM2), ScoreboardCol(CVRM), Format("<i>%s",GetPlayerName(GetOwner(pTarget))), 1);
  else SetScoreboardData(ScoreboardCol(TEM1), ScoreboardCol(CVRM), Format("<i>%s",GetPlayerName(GetOwner(pTarget))), 1);
}

func FxPortFlagTimer(pTarget, iNumber, iTime)
{
  // Grafik und Position immer an den Träger anpassen	
  SetClrModulation();
  SetPosition(GetX(pTarget)+4-8*GetDir(pTarget), GetY(pTarget)-8);
  SetXDir(GetXDir(pTarget));
  SetYDir(GetYDir(pTarget));
  SetDir(5-4*GetDir(pTarget));
  SetDTRotation(-30+60*GetDir(pTarget));
  // Die Fortbereiche sind verboten
  if( GetY()<257 && (GetX()<490 || GetX()>1500) )
  {
    Message("$MsgNoFlags$", pTarget);    
    return(Reset());
  }
  // und die Aufzugsschächte sind auch verboten
  if((GetY()<404 && Inside(GetX(), 408, 433))
  || (GetY()<442 && Inside(GetX(),   8,  31))
  || (GetY()<401 && Inside(GetX(),1568,1593))
  || (GetY()<444 && Inside(GetX(),1969,1992)) )
  {
    Message("$MsgNoFlags$", pTarget);    
    return(Reset());
  }
  // Betritt der Spieler ein Gebäude, muss er die Flagge drausen stehen lassen
  var obj;
  if(obj = Contained(pTarget))
  {
    if(GetID(obj)==CTWR || GetID(obj)==CSTE) return(SetClrModulation(RGBa(255,255,255,255)));
    fCollect=1;
    Enter(Contained(pTarget));
    fCollect=0;
    SetCommand(this(), "Exit");
    return(-1);
  }
}

func FxPortFlagStop(pTarget,iNumber, iReason, fTmp)
{
  if(fTmp) return();
  if(!this()) return();
  // Aktionen zurücksetzen
  SetAction("Fly", 0);
  SetDir(0);
  SetObjectLayer();
  SetActionTargets();
  SetDTRotation(0);
  // Status updaten
  if(iTeam==1) stateleft=3;
  else stateright=3;
  ActualizeScoreboard();
  // Spielernamen aus dem Board entfernen
  if(iTeam==1) ExtraLog("$MsgFlagDropB$", GetPlayerName(GetOwner(pTarget)));
  else ExtraLog("$MsgFlagDropR$", GetPlayerName(GetOwner(pTarget)));
}

// Funktion für eine Fake-Rotation
global func SetDTRotation (int r, int xoff, int yoff, object obj) {
  var fsin=Sin(r, 1000), fcos=Cos(r, 1000);
  // set matrix values
  SetObjDrawTransform (
    +fcos, +fsin, (1000-fcos)*xoff - fsin*yoff,
    -fsin, +fcos, (1000-fcos)*yoff + fsin*xoff,
    obj
  );
}
