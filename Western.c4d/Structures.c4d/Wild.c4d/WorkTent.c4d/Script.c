/*-- Arbeitszelt --*/

#strict 2

#include AMRY
#include TIPI

// Das Arbeitszelt hat extra eine zu kleine Grafik und wird mit einer Aktion voll angezeigt
// so kann es unter Palisaden errichtet werden und blockiert höhere Palisadenebenen nicht

local pAnvil;

/* Initialisierung */

protected func Initialize()
{
  _inherited();
  SetAction("Exist");
  ScheduleCall(this(),"CreateAnvil",1,1);
  return(1);
}

public func CreateAnvil()
{
  if(GetCon() == 100 && GetAction() == "Exist" && !pAnvil) {
    pAnvil = CreateObject(ANVL,-22,26,GetOwner());
    SetObjectOrder(this(),pAnvil);
    return(pAnvil);
  }
  return(0);
}

protected func Destruction()
{
  if(pAnvil) RemoveObject(pAnvil,1);
  return(1);
}

/* Aktivität */

private func Smoking() {
  Smoke(8, -15, 5 + Random(3));
  Smoke(8, -15, 5 + Random(3));
}

/* Steuerung */

// Hier lassen sich keine Captains Rekrutieren
protected func ContextRecruitCaptain(object pClonk)
{
  [Condition=Cancel]
  return(0);
}

/* Aktivität */

private func CheckBuild() {
  // TimerCall: die Werkstatt startet ihre eigene Build-Aktion, sobald
  // jemand in der Werkstatt arbeitet. Die Arbeit in einem Gebäude schreitet
  // nur voran, wenn das Gebäude dies durch eine eigene Build-Aktion unterstützt.
  var fWorkingClonk = IsWorking();
  var fBuildingAction = (GetAction() == "Build");
  if( fWorkingClonk && !fBuildingAction) if(GetAction() == "Exist") SetAction("Build");
  if(!fWorkingClonk &&  fBuildingAction) SetAction("Exist");
  return(1);
}

/* Boss ausbilden */

protected func ContainedDig(object pClonk)
{
  [$TxtRecruitBoss$|Image=BNDT]
  return(_inherited(pClonk));
}

/* Kann nicht von Indianern erforscht werden */
public func IsIndianResearch() { return(0); }

/* Kann nicht vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(0); }

/* Kann vom Trapper hergestellt werden */
public func IsTrapperHandcraft() { return(1); }