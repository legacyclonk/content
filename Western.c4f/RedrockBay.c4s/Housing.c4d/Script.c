/*-- Objekte erschaffen --*/

#strict
#include GOAL

/* Array für zu erschaffende Objekte */

local createTypes;
local currentTypes;

local createType0, createCount0, currentCount0;
local createType1, createCount1, currentCount1;
local createType2, createCount2, currentCount2;
local createType3, createCount3, currentCount3;
local createType4, createCount4, currentCount4;
local createType5, createCount5, currentCount5;
local createType6, createCount6, currentCount6;
local createType7, createCount7, currentCount7;
local createType8, createCount8, currentCount8;
local createType9, createCount9, currentCount9;

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

//! Fügt einen zu erzeugenden Objekttyp samt zu erzeugender Anzahl diesem Spielziel hinzu.

private func AddType(idType, iCount)
{
  SetArrayId("createType", createTypes, idType);
  SetArrayInt("createCount", createTypes, iCount);
  createTypes++;
}
        
public func IsFulfilled()
{
  // Typen überprüfen
  currentTypes = 0;
  for (var i = 0; i < createTypes; i++)
  {
    // Derzeitige Anzahl mit Zielanzahl vergleichen
    var targetCount = GetArray("createCount", i);
    var currentCount = ObjectCount(GetArray("createType", i), 0,0,0,0, OCF_Fullcon());
    if (currentCount < targetCount)
      currentTypes++;
    // Derzeitige Anzahl zwischenspeichern
    SetArrayInt("currentCount", i, currentCount);
  }
  // Es sind noch Objekte zu erzeugen
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
  for (var i = 0; i < createTypes; i++)
  {
    var idType = GetArray("createType", i);
    var iCount = GetArray("createCount", i) - GetArray("currentCount", i);
    if (iCount > 0)
      AddMenuItem(GetName(), "Noop", idType, menuObject, iCount, 0, Format("$MsgGoalTodo$", iCount, GetName(0, idType)));
  }
  // Fertig
  return(1);
}

/* Macht nichts, ist da, damit das Menu keinen Fehler erzeugt (#930) */
private func Noop(id type)
{
}

