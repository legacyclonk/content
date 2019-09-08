/*-- Objekte entfernen --*/

#strict
#include GOAL

/* Array für zu erschaffende Objekte */

local removeTypes;
local currentTypes;

local removeType0, removeCount0, currentCount0;
local removeType1, removeCount1, currentCount1;
local removeType2, removeCount2, currentCount2;
local removeType3, removeCount3, currentCount3;
local removeType4, removeCount4, currentCount4;
local removeType5, removeCount5, currentCount5;
local removeType6, removeCount6, currentCount6;
local removeType7, removeCount7, currentCount7;
local removeType8, removeCount8, currentCount8;
local removeType9, removeCount9, currentCount9;

/* Array-Funktionen */

private func GetArray(strVariable, iIndex)
{
  return (eval(Format("%s%d", strVariable, iIndex)));
}

private func SetArrayInt(strVariable, iIndex, iValue)
{
  return (eval(Format("%s%d = %d", strVariable, iIndex, iValue)));
}

private func SetArrayId(strVariable, iIndex, idValue)
{
  return (eval(Format("%s%d = %i", strVariable, iIndex, idValue)));
}

//! Fügt einen zu entfernenden Objekttyp samt zu Ziel-Anzahl diesem Spielziel hinzu.

private func AddType(idType, iCount)
{
  SetArrayId("removeType", removeTypes, idType);
  SetArrayInt("removeCount", removeTypes, iCount);
  removeTypes++;
}
        
public func IsFulfilled()
{
  // Typen überprüfen
  currentTypes = 0;
  for (var i = 0; i < removeTypes; i++)
  {
    // Derzeitige Anzahl mit Zielanzahl vergleichen
    var targetCount = GetArray("removeCount", i);
    var currentCount = ObjectCount(GetArray("removeType", i), 0,0,0,0, OCF_Fullcon());
    if (currentCount > targetCount)
      currentTypes++;
    // Derzeitige Anzahl zwischenspeichern
    SetArrayInt("currentCount", i, currentCount);
  }
  // Es sind noch Objekte zu entfernen
  return(currentTypes == 0);
}

protected func Activate(iPlayer)
{
  // Erfüllt
  if (IsFulfilled()) 
    return(MessageWindow(Format("$MsgGoalFulfilled$"), iPlayer));
  // Noch nicht erfüllt
  var menuObject = GetCursor(iPlayer);
  CreateMenu(GetID(), menuObject, this(), 0, GetName());
  for (var i = 0; i < removeTypes; i++)
  {
    var idType = GetArray("removeType", i);
    var iCount = GetArray("currentCount", i) - GetArray("removeCount", i);
    if (iCount > 0)
      AddMenuItem(GetName(), "MenuClosed", idType, menuObject, iCount, 0, Format("$MsgGoalTodo$", iCount, GetName(0, idType)));
  }
  // Fertig
  return(1);
}

public func MenuClosed()
{
  return(1);  
}