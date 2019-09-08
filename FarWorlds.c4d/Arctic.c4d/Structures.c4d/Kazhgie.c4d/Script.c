/*-- Gemeinschaftsiglu --*/

#strict

#include WRKS
#include IGLO

private func MaxOccupant() { return(6); }

/* ~#exclude für das fundament */

protected func Construction() {}
protected func Destruction() {}

/* Eingangssteuerung */

protected func ActivateEntrance(pByObj) { // (koordinaten geändert)
  // Raus geht's immer
  if (Contained(pByObj)==this())
    return(Exit(pByObj,22,14));
  // Weitere Clonks können nur rein, bis die Maximalzahl erreicht ist
  if (GetOCF(pByObj)&OCF_CrewMember())
    if (IsOccupied())
      return(0,Message("$Full$",this()));
  // Sonst Objekt aufnehmen
  return(Enter(this(),pByObj));
}

/* TimerCall */

protected func Activity() { // (komponenten geändert)
  // Es ist wohl zu warm
  if (GetTemperature()>10) Melt();
  // Zeit als minimale Baustelle berechnen
  if (!(GetComponent(SWBL)||GetComponent(WOOD)||GetComponent(LTHR)))
    Local(9)++;
  // Baustelle errichtet, aber nicht weitergebaut: abbrechen
  if (Local(9)>35) RemoveObject();  
  CheckBuild();
  return(1);
}

/* Wärme */

public func GetWarmth() {
  // Großes Iglu ist etwas wärmer
  return(60);
}

/* Konstruktion */

private func MenuProduction(pCaller) {
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(IGL2,pCaller,this(),1,"$TxtNoconstructionplans$");
  for(var i=0,idKnowledge;idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,C4D_Vehicle());++i)
    AddMenuItem( "$TxtProductions$", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  return(1);
}

private func Building()
{
  if (!Random(2)) Sound("Build*");
}