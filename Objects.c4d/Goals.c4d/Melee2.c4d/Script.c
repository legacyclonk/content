/*-- Melee (Veraltet) --*/

#strict

// Seit Einführung der Teams.txt werden "Teams" und "Melee" getrennt verwaltet
// Das Spielziel "Teamwork-Melee" ist damit überflüssig und kann durch Melee
// mit entsprechenden Teams im Szenario ersetzt werden.

#include MELE

// Alte Szenarien automatisch anpassen
protected func Initialize()
  {
  DebugLog("$MsgDeprecated$");
  UpdateDeprecated();
  }
  
protected func UpdateTransferZone()
  {
  DebugLog("$MsgDeprecated$");
  Schedule("this()->~UpdateDeprecated", 1, 1, this());
  }

public func UpdateDeprecated()
  {
  SetCategory(C4D_Goal | C4D_StaticBack);
  ChangeDef(MELE);
  return true;
  }
