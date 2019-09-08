/*-- Counter --*/

#strict

local pMessage1;
local pMessage2;

protected func Initialize()
{
    if(ObjectCount2(Find_ID(GetID()), Find_Owner(GetOwner()))>1) return;
    SetPosition(79, 275);       // Positionieren 66,124
    SetClrModulation(RGBa(255,255,255,50)); // Transparent machen
    Local(0)=Local(1)=0;        // Parallax auf 0 Setzen
    SetCategory(44040208);      // Parallax+Object+FoWIgnore
    SetVisibility(VIS_Owner()); // Nur für den Besitzer sichtbar
    SetAction("Counting");      // Anzeige-Action setzen
    UpdateBandits();		// Banditen anzeigen
    // Tocherobjekte erzeugen
    pMessage1 = CreateContents(GetID());
    pMessage2 = CreateContents(GetID());
    // Zähler starten
    if(!GetEffect("Counting")) AddEffect("Counting", 0, 1, 35, 0, GetID());
}

public func UpdateScore(szMessage1, szMessage2, szMessage3, iColor1, iColor2, iColor3)
{
  CustomMessage(szMessage1, pMessage1, GetOwner(), 0,-55, iColor1);
  CustomMessage(szMessage2, pMessage2, GetOwner(), 0,-27, iColor2);
  CustomMessage(szMessage3, this     , GetOwner(), 0,  5, iColor3);
}

public func UpdateLifes()
{
  var i;	
  while(i++ < Lifes(GetOwner()))
  {
    SetGraphics("Hat", this(), GetID(), i, GFXOV_MODE_Action, "Hat");
    SetObjDrawTransform(1000, 0, 46000, 0, 1000, -175000+15000*i, this(), i);
    if(!ScoreboardCol(NET2)) SetObjDrawTransform(1000, 0,+ 35000, 0, 1000, -185000+15000*i, this(), i);
  }
  i--;
  while(i++ < 5)
  {
    SetGraphics(0, this(), 0, i, GFXOV_MODE_Action);
  }
}

public func UpdateBandits()
{
  var i;	
  while(i++ < iBandits)
  {
    SetGraphics("Hat2", this(), GetID(), i+10, GFXOV_MODE_Action, "Hat");
    SetObjDrawTransform(1000, 0, -46000, 0, 1000, -175000+15000*(i), this(), i+10);
    if(!ScoreboardCol(NET2)) SetObjDrawTransform(1000, 0, -60000, 0, 1000, -185000+15000*(i), this(), i+10);
  }
  i--;
  while(i++ < 5)
  {
    SetGraphics(0, this(), 0, i+10, GFXOV_MODE_Action);
  }
}

global func ScoreboardCol(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }

func FxCountingStart(pTarget, iNumber, fTmp)
{
  if(fTmp) return();
  // Zeit aus Szen abfragen
  EffectVar(0, pTarget, iNumber) = GameCall("GetMaxTime");
  if(!EffectVar(0, pTarget, iNumber)) EffectVar(0, pTarget, iNumber) = 15*60;
}

func FxCountingTimer(pTarget, iNumber)
{
  // Zeit runterzählen bzw. blinker setzen  
  if(EffectVar(0, pTarget, iNumber)) EffectVar(0, pTarget, iNumber)--;
  else EffectVar(1, pTarget, iNumber) = !EffectVar(1, pTarget, iNumber);
  // Zeit splitten
  var iSec = EffectVar(0, pTarget, iNumber) % 60;
  var iMin = EffectVar(0, pTarget, iNumber) / 60;
  // Farbe anpassen
  var iColor = RGB(255,255,255);
  if(EffectVar(0, pTarget, iNumber) < 120) iColor = RGB(255,0,0);
  // Nachricht speichern
  var szMessage1 = Format("@%d", iLevel);
  var szMessage2 = Format("@%d", EffectVar(5, pTarget, iNumber));
  var szMessage3 = Format("@%.2d:%.2d", iMin, iSec);
  // Über allen Zählern ausgeben
  var pCounter;
  for(pCounter in FindObjects(Find_ID(GetID()), Find_Container()) )
  {
    // Besitzerlose Counter löschen   
    if(GetOwner(pCounter)==-1)
    {
      RemoveObject(pCounter);
      continue;
    }
    
    // Und anzeigen
    pCounter->UpdateScore(szMessage1, szMessage2, szMessage3, 0, 0, iColor);
    // Blinken bei abgelaufener Zeit
    if(EffectVar(1, pTarget, iNumber)) Message("", pCounter);
  }
  if(!EffectVar(0, pTarget, iNumber))
  {
    GameCall("TimeUp");
  } 
}
