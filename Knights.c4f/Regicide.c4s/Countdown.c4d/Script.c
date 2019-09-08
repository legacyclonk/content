/*-- Counter --*/

#strict

protected func Initialize()
{
    SetPosition(67,81);        // Positionieren 66,124
    Local(0)=Local(1)=0;        // Parallax auf 0 Setzen
    SetCategory(44040208);   // Parallax+Object+FoWIgnore
    SetVisibility(VIS_Owner());   // Nur für den Besitzer sichtbar
    SetAction("Counting");     // Anzeige-Action setzen
    // Zähler starten
    if(!GetEffect("Counting")) AddEffect("Counting", 0, 1, 35, 0, GetID());
}

func FxCountingStart(pTarget, iNumber, fTmp)
{
  if(fTmp) return();
  // Zeit aus Szen abfragen
  EffectVar(0, pTarget, iNumber) = GameCall("GetMaxTime");
  if(!EffectVar(0, pTarget, iNumber)) EffectVar(0, pTarget, iNumber) = 15*60;
}

func FxCountingTimer(pTarget, iNumber)
{
  // Zeit runterzählen 
  if(EffectVar(0, pTarget, iNumber)) EffectVar(0, pTarget, iNumber)--;
  // Zeit splitten
  var iSec = EffectVar(0, pTarget, iNumber) % 60;
  var iMin = EffectVar(0, pTarget, iNumber) / 60;
  // Farbe anpassen
  var iColor = RGB(255,255,255);
  if(EffectVar(0, pTarget, iNumber) < 120) iColor = RGB(255,0,0);
  // Nachricht speichern
  var szMessage = Format("@%.2d:%.2d", iMin, iSec);
  // Über allen Zählern ausgeben
  var pCounter;
  for(pCounter in FindObjects(Find_ID(GetID())) )
  {
    // Besitzerlose Counter löschen   
    if(GetOwner(pCounter)==-1)
    {
      RemoveObject(pCounter);
      continue;
    }
    
    // Und anzeigen
    CustomMessage(szMessage, pCounter, GetOwner(pCounter), -7,51, iColor);
  }
  if(!EffectVar(0, pTarget, iNumber))
  {
    GameCall("TimeUp");
    AddEffect("CounterBlink", 0, 1, 15, 0, GetID());
    return(-1);
  } 
}

func FxCounterBlinkTimer(pTarget, iNumber)
{
  // Blinker setzen 
  EffectVar(1, pTarget, iNumber) = !EffectVar(1, pTarget, iNumber);
  // Nachricht speichern
  var szMessage = Format("@00:00");
  // Über allen Zählern ausgeben
  var pCounter;
  while(pCounter = FindObject(GetID(), 0, 0, 0, 0, 0, 0, 0, 0, pCounter) )
  {
    // Besitzerlose Counter löschen   
    if(GetOwner(pCounter)==-1)
    {
      RemoveObject(pCounter);
      continue;
    }
    
    // Und anzeigen
    CustomMessage(szMessage, pCounter, GetOwner(pCounter), -7,51, 0xff0000);
    // Blinken
    if(EffectVar(1, pTarget, iNumber)) Message("", pCounter);
  }
}
