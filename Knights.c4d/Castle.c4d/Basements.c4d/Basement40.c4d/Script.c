/*-- Fundament --*/

#strict

local hatch;

/* Alle Fundamente leiten ihre Grundfunktionalität von BAS7 ab */

#include BAS7

private func BasementID() { return(BSC1); }
private func BasementWidth() { return(GetDefCoreVal("Width", "DefCore", BasementID()) ); }
private func BasementDamage() { return(250); }
private func HatchID() { return(BSO1); }

protected func Damage(iChange, iByPlayer)
{
  if(GetID() != BasementID() ) return(_inherited(iChange, iByPlayer) );
  if(GetDamage() < BasementDamage() ) return(_inherited(iChange, iByPlayer) );

  CastObjects(ROCK,4,15,0,-5);
  RemoveObject();
  
  return(_inherited(iChange, iByPlayer) );
}

/* In ein Fundament mit Loch (für die Bodenklappe) verwandeln */

public func ChangeToHatch(object pHatch)
{
  // Kein entsprechendes Bodenklappe-Fundament? Geht hier nicht.
  if(!HatchID() ) return(0);
  // Bodenklappe speichern
  hatch = pHatch;
  // Umwandeln
  ChangeDef(HatchID() );
}

/* Zerstörung */

protected func Destruction()
{
  // Bodenklappe löschen
  if(hatch) RemoveObject(hatch);
  // Basisfunktion aufrufen
  return(_inherited() );
}

