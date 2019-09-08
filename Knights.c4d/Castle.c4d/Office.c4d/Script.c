/*-- Kontor --*/

#strict

#include CPW2 // Burgteilfunktion
#include DOOR // Tuersteuerung

/* Initialisierung */

protected func Initialize()
{
  // Verzögerte Initialisierung
  SetAction("Initialization");
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  return(_inherited() );
}

private func Initialize2()
{
  // Fackeln erzeugen (verzögert, für korrekte Sortierung)
  CreateObject(TORC, -26, 35,GetOwner() )->Activate();
  SetDir(DIR_Right(), CreateObject(TORC, 26, 35)->Activate() );

  // Hinweis: Burgobjekte ändern in Initialize ihre eigene Kategorie auf C4D_StaticBack.
  // Die zugehörige interne Objektsortierung führt die Engine jedoch erst nach mindestens einem
  // kompletten Frame (Execution) durch. Innerhalb der der Initialize-Funktion eines Burgobjekts
  // dürfen also keine weiteren C4D_StaticBack-Objekte erzeugt werden, da diese sonst in der
  // internen Objektliste an einer falschen Position einsortiert werden, höchstwahrscheinlich
  // hinter dem Burgobjekt, was ja meistens nicht sein soll.
}

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (80); }
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return(BSC3); }


/* Forschung */

public func GetResearchBase() { return(CPW2); }
