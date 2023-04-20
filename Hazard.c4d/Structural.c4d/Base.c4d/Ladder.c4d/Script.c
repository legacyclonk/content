/*-- Leiter --*/

#strict

/* Narf, die Leiter hat immer noch alte Effekt-Arrays. Aber das umbauen geht sehr doof und hackig, also behält sie die einfach, tah! */

static const LADR_Left = 1;
static const LADR_Right = 2;
static const LADR_Front = 4;

local length; //gibt an aus wievielen Teilen die Leiter besteht
local sizeX, sizeY; //4great leicht umstelling
local Graphics, ID; //für andere leitern :D
local GraCount;

func Initialize() {
  SetAction("2Be"); //damit sie nicht runterfallen ;)
  ID = GetID();
  sizeX = GetDefCoreVal("Height",0,GetID())/2;
  sizeY = GetDefCoreVal("Width",0,GetID())/2;
  Graphics = AddEffect("LadderPartSave",this(),1,0,this(),GetID());
  ID = AddEffect("LadderPartSave",this(),1,0,this(),GetID());
//  SetGraphics(Format("Part%d",Random(3)+1),0,LADR);
  // Standardverhalten: nur Part1 anzeigen
  AddGraphics("Part1",LADR);
  AddPart(); //einen Teil hinzufügen :>
  AdjustSize();
}

public func AddGraphics(string sNewGraphic, id idNewID) {
  EffectCall(this(),Graphics,"AddPart",sNewGraphic,GraCount);
  EffectCall(this(),ID,"AddPart",idNewID,GraCount);
  GraCount++;
}

public func ClearGraphics() {
  while(GraCount >= 0) {
    EffectCall(this(),Graphics,"ClearParts",GraCount);
    EffectCall(this(),ID,"ClearParts",GraCount);
    GraCount--;
  }
  GraCount = 0;
}

public func SetSize(int x,int y) {
  sizeX = x;
  sizeY = y;
  AdjustSize();
}

public func Set(int iNewLength, int iDir) {
  if(iNewLength <= 0)
    return(0);
  var cmd;
  // iDir angegeben, speziell für LADR... andere
  // Kletterobjekte können das überladen
  if(iDir) {
    ClearGraphics();
    if(iDir & LADR_Left) AddGraphics("Part2",LADR);
    if(iDir & LADR_Right) AddGraphics("Part3",LADR);
    if(iDir & LADR_Front) AddGraphics("Part1",LADR);
    Redraw();
    return(Set(iNewLength));
  } 
  //falls neue Länge größer als momentane
  if(length < iNewLength)
    cmd = "Add"; //hinzufügen
  //falls kleiner
  if(length > iNewLength)
    cmd = "Remove"; //löschen
  /*tu*/
  for(var x=Abs(length-iNewLength); x > 0; x--)
    PrivateCall(this(),Format("%sPart",cmd));
  //zum Schluss das ganze Zeug noch auf die neue Größe anpassen
  AdjustSize();
  return(1);
}

//Zeichnet einfach die Grafiken neu.
public func Redraw() {
  for(var x=1; x <= length; x++) {
    var graph = Random(GraCount);
    SetGraphics(EffectCall(this(),Graphics,"GetPart",graph),this(),EffectCall(this(),ID,"GetPart",graph),x,GFXOV_MODE_Base);
    SetObjDrawTransform(1000,0,0,0,1000,1000*sizeY*(x-1),this(),x);
  }
}

private func AddPart() {
  //ein Teil mehr, Länge raufzählen
  length++;
  //Sicherheitshalber checken, evtl. gibt es ja unvorhergesehenen Fehler durch kosmische Strahlung!
  var graph = Random(GraCount);
  if(!SetGraphics(EffectCall(this(),Graphics,"GetPart",graph),this(),EffectCall(this(),ID,"GetPart",graph),length,GFXOV_MODE_Base))
    return(0);
  //Noch an die richtige Position schieben
  SetObjDrawTransform(1000,0,0,0,1000,1000*sizeY*(length-1),this(),length);
  //keine kosmische Strahlung, alles gut =D
  return(1);
}

private func RemovePart() {
  //Minimal ein Teil muss schon da sein..
  if(length < 0)
    return(0);
  //letzten Teil entfernen
  SetGraphics(0,0,0,length);
  //ein Teil weniger, Länge runterzählen
  length--;
  //kein Kommentar..
  return(1);
}

private func AdjustSize() {
  var ydiff = GetObjHeight();
  SetShape(-sizeX,-sizeY,sizeX*2,sizeY+sizeY*length,this());
  ydiff -= GetObjHeight();
  SetPosition(GetX(),GetY()+ydiff);
  SetVertex(0,1,-sizeY,this());
  SetVertex(1,1,sizeY*length,this());

  //die ganzen Positionen der einzelnen Elemente neu adjustieren
  for(var x=1; x <= length; x++)
    SetObjDrawTransform(1000,0,0,0,1000,sizeY*1000*(x-1),this(),x);
}

private func FxLadderPartSaveAddPart(object pTarget, int iEffectNr, graphic, int index) {
  var length;
  if(!(length=index))
    while(EffectVar(length,pTarget,iEffectNr))
      length++;
  EffectVar(length,pTarget,iEffectNr) = graphic;
}

private func FxLadderPartSaveClearParts(object pTarget, int iEffectNr, int index) {
  EffectVar(index,pTarget,iEffectNr)=0;
}

private func FxLadderPartSaveGetPart(object pTarget, int iEffectNr, int index) {
  return(EffectVar(index,pTarget,iEffectNr));
}

//bla
public func UpdateTransferZone() {
  ClearGraphics();
  for(var z=1; z <= 3; z++)
    AddGraphics(Format("Part%d",z),LADR);
  AdjustSize();
  Redraw();
}

//Ja, Leitern kann man auch hochklettern.. :D
public func IsClimbable() { return(true); }

/* Serialisierung */

public func Serialize(array& extra, int& extray)
{
	extra[GetLength(extra)] = Format("Set(%d)", length);
	extray = - (length-1)*sizeY;
}
