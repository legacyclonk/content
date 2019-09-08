/*-- FoW-Generator --*/

#strict

local w,h; // Breite/Höhe des Objektes
local search_x, search_y, search_w, search_h; // Suchrechteck für Clonks - x/y ist Offset zur Position!
local iHiddenObjCnt; // Anzahl unsichtbarer Objekte (in Local(i))

static const FOW_PlayerSearchRangeX = 180, // Zu w/h dazugezählte Suchreichweite für Clonks,
              FOW_PlayerSearchRangeY = 120; //   die den FoW entfernen

protected func Initialize()
  {
  // Sicherheitshalber: FoW-Generator für alle Spieler
  SetOwner(-1);
  // Anderes Placement-Objekt gefunden? Dann platzieren
  var pOther;
  if (pOther = FindObject(GetID(), 0,0,0,0, 0, "Placement"))
    return(ShadowInit(GetX(pOther), GetY(pOther), GetX(), GetY()), RemoveObject(pOther));
  // Ansonsten Platzierung starten
  return(SetAction("Placement"));
  }

public func ShadowInit(int x1, int y1, int x2, int y2)
  {
  // Positionieren
  w = Abs(x2-x1); h = Abs(y2-y1);
  w=h=(w+h)/2; // Solange FoW immer quadratisch ist, besser gleich breit wie hoch...
  SetPosition((x1+x2)/2, (y1+y2)/2);
  SetShape(w/-2, h/-2, w,h);
  InitSearchRect();
  // Starte deaktiviert und sichtbar
  SetAction("Passive");
  MakeVisible();
  }
  
public func SetSearchRect(int sx, int sy, int sw, int sh)
  {
  search_x=sx; search_y=sy; search_w=sw; search_h=sh;
  MakeVisible(true);
  }
  
private func InitSearchRect()
  {
  // Default-Suchrechteck
  search_x = -w/2-FOW_PlayerSearchRangeX;
  search_y = -h/2-FOW_PlayerSearchRangeY;
  search_w = w+FOW_PlayerSearchRangeX*2;
  search_h = w+FOW_PlayerSearchRangeX*2;
  }
  
public func MakeVisible(bool fShowRange)
  {
  SetVisibility(VIS_All);
  if (fShowRange)
    {
    SetShape(search_x, search_y, search_w, search_h);
    SetObjDrawTransform(search_w*1000, 0,search_w*search_w*500, 0,search_h*1000,search_h*search_h*500);
    SetClrModulation(RGB(0,255,0));
    }
  else
    {
    SetShape(w/-2, h/-2, w,h);
    SetObjDrawTransform(w*1000, 0,w*w*500, 0,h*1000,h*h*500);
    SetClrModulation(RGB(255,0,0));
    }
  }
  
protected func UpdateTransferZone()
  {
  if (!search_w) InitSearchRect();
  if (GetVisibility() && GetVisibility() != VIS_None) SetVisibility(VIS_None);
  SetShape(w/-2, h/-2, w,h); // h4x
  if (GetAction() eq "Active") SetPlrViewRange(Min((w+h)/-2+40, -1));
  }
  
public func MakeInvisible()
  {
  SetVisibility(VIS_None);
  SetClrModulation();
  SetShape(-1,-1,2,2);
  SetObjDrawTransform(1000, 0, 0, 0, 1000, 0);
  }
  
public func Activate()
  {
  // Macht das Objekt "Scharf": FoW wird gesetzt, Objekte unterhalb deaktiviert
  // Mehrfache Aufrufe sind OK; zusätzlich gefundene Objekte werden dann aufgenommen
  SetAction("Active");
  var pObj;
  for (pObj in FindObjects(Find_InRect(w/-2,h/-2,w,h)))
    if (GetID(pObj) != _FOW && GetID(pObj) != _WSC && GetID() != _CST && GetID(Contained()) != _CST) // FoW-Generatoren, Schilder, Kisten und Gegenstände darin überspringen
      SetObjectStatus(C4OS_INACTIVE, Local(iHiddenObjCnt++) = pObj);
  // FoW generieren
  SetPlrViewRange(Min((w+h)/-2+40, -1));
  }
  
public func Deactivate()
  {
  // Entschärft das Objekt: FoW wird entfernt und Objekte eingefadet
  SetAction("Passive");
  var pObj;
  while (iHiddenObjCnt) if (pObj = Local(--iHiddenObjCnt))
    {
    if (GetObjectStatus(pObj) != C4OS_INACTIVE) continue;
    var s = Format("%v", pObj);
    SetObjectStatus(C4OS_NORMAL, pObj);
    // Callback ans Objekt für spezifische Reinitialisierungen
    if (pObj)
      pObj->~OnReactivate();
    else
      Log("%s is dead", s);
    }
  SetPlrViewRange();
  }
  
  
// Clonksuche zum Entfernen des FoW
protected func CheckClonk()
  {
  var o;
  for (o in FindObjects(Find_InRect(search_x, search_y, search_w, search_h), Find_OCF(OCF_CrewMember), Find_NoContainer(), Find_Not(Find_Owner(GetPlayerByIndex(0, C4PT_Script)))))
    if (GetController(o) > NO_OWNER)
      {
      Deactivate();
      // 2do: Callback ans Szenarienscript für spezielle Bereiche?
      return(RemoveObject());
      }
  }


// Debug-Funktionen
global func FOW_Show()
  {
  var o; while (o = FindObject(_FOW, 0,0,0,0, 0, 0,0, 0, o))
    if (GetAction(o) ne "Placement")
      o->MakeVisible();
  }
  
global func FOW_ShowRange()
  {
  var o; while (o = FindObject(_FOW, 0,0,0,0, 0, 0,0, 0, o))
    if (GetAction(o) ne "Placement")
      o->MakeVisible(true);
  }
  
global func FOW_Hide()
  {
  var o; while (o = FindObject(_FOW, 0,0,0,0, 0, 0,0, 0, o))
    if (GetAction(o) ne "Placement")
      o->MakeInvisible();
  }
  
global func FOW_Activate()
  {
  var i;
  var o; while (o = FindObject(_FOW, 0,0,0,0, 0, 0,0, 0, o))
    if (GetAction(o) ne "Placement")
      Schedule(Format("Object(%d)->Activate()", ObjectNumber(o)), i++%20+1); // Verzögert aktivieren, damit FindObject-Schleifen nicht alle gleich laufen
  }
  
global func FOW_Deactivate()
  {
  var o; while (o = FindObject(_FOW, 0,0,0,0, 0, 0,0, 0, o))
    if (GetAction(o) ne "Placement")
      o->Deactivate();
  }


