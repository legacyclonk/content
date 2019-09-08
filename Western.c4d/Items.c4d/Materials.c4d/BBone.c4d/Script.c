#strict

// Wird in BONE umgewandelt, wobei die Grafik beibehalten wird,
// soll bewirken, dass man normale Knochen und Büffelknochen
// gleichwertig für die Weiterverarbeitung einsetzen kann

public func Initialize()
{
  // Alle Komponenten löschen
  var i, id;
  while(id = GetComponent(0, i++, 0, GetID())) SetComponent(id, 0, this() );
  SetComponent(IBON, 4, this() );
  // Ein zusätzlicher Effekt für den Objekt, damit man erkennt, was für ein Objekt das eig. ist:
  AddEffect(Format("%i",GetID()),this(),250);
  // Weitermachen
  var idID = GetID();	
  var szName = GetName();	
  var a = GetDefCoreVal("Picture", "DefCore", idID, 0),
      b = GetDefCoreVal("Picture", "DefCore", idID, 1),
      c = GetDefCoreVal("Picture", "DefCore", idID, 2),
      d = GetDefCoreVal("Picture", "DefCore", idID, 3);
  ChangeDef(ChangeID());
  SetGraphics(0, this(), idID);
  ObjectSetAction(this(), Format("%i",idID));
  SetName(szName, this());
  SetPicture(a, b, c, d, this());
}

public func ChangeID() { return(BONE); }
