/*-- Kran --*/

#strict

local length, size,
      slide;

protected func Initialize() {
  SetAction("Crane");   // Standardaktion
  // Standardteil
  size=14;
  AddPart();
  // "Kran" erzeugen
  CreateCrane();
}

public func CreateCrane() {
  // Schon ein Kran da?
  if(slide) return(slide);
  // Erstellen
  slide = CreateObject(SLDE, 0, 5, GetOwner());
  // Ransetzen
  slide->Set(this());
}

public func Set(int iNewLength) {
  // 0 geht nicht
  if(!iNewLength) return();
  // Länger machen?
  if(iNewLength > length)
    AddParts(iNewLength);
  // Kürzer machen
  if(iNewLength < length)
    RemoveParts(iNewLength);
}

/* Länge */

private func AddParts(int iNewLength) {
  // Solang hinzufügen, bis richtig
  while(length != iNewLength)
    AddPart();
}

private func RemoveParts(int iNewLength) {
  // Ein Teil ist Minimum
  if(!iNewLength) iNewLength = 1;
  // Solang wegnehmen, bis richtig
  while(length != iNewLength)
    RemovePart();
}

public func AddPart() {
  // Position
  var iX = GetX(),iY = GetY();
  length++;
  // Ein Teil dransetzen
  SetGraphics("Part",this(),GetID(),length, GFXOV_MODE_Base);
  // Position
  SetPosition(iX, iY);
  // Anpassen
  AdjustSize();
}

public func RemovePart() {
  // Nicht ganz weg
  if(length == 1) return();
  // Position
  var iX = GetX(),iY = GetY();
  // Ein Teil wegnehmen
  SetGraphics(0,this(),0,length);
  length--;
  // Position
  SetPosition(iX, iY);
  // Anpassen
  AdjustSize();
}

public func AdjustSize() {
  // Objektgröße anpassen
  var Wdt = size*length;
  SetShape(-Wdt/2, -4, Wdt, 8);

  // Grafik anpassen
  for(var i = 1, j=-length+1; i <= length; i++) {
    SetObjDrawTransform(1000, 0, size*500*j, 0, 1000, 0, this(), i);
    j += 2;
    }
}

/* Callbacks */

public func LeftX() {
  if(length%2)
    return(GetX()-(length/2*size)-size/2);

  return(GetX()-(length/2*size));
}

public func RightX() {
  if(length%2)
    return(GetX()+(length/2*size)+size/2);

  return(GetX()+(length/2*size));
}

public func ConsoleControl(int i)
{
  if(slide) return(slide->ConsoleControl(i));
}

public func ConsoleControlled(int i, int Clonk)
{
  if(slide) return(slide->ConsoleControlled(i, Clonk));
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("Set(%d)", length);
	var clamp = slide->LocalN("clamp");
	if (clamp && clamp->LocalN("load")) {
		extra[GetLength(extra)] = Format("Reconfigure(%d,%d,Object(%%d))", GetX(slide)-GetX(), GetY(clamp)-(GetY(slide)+25), clamp->LocalN("clamp"));
	} else {
		extra[GetLength(extra)] = Format("Reconfigure(%d,%d)", GetX(slide)-GetX(), GetY(clamp)-(GetY(slide)+25));
	}
}
