/*-- Scriptdefiniertes Spielziel --*/

#strict
#include GOAL

/* Lokale Variablen */

local goalFulfilled;
local messageFulfilled;
local messageUnfulfilled;
        
protected func Initialize()
{
  messageFulfilled = "$MsgGoalFulfilled$";
  messageUnfulfilled = "$MsgGoalUnfulfilled$";
  return(_inherited());
}          
        
public func IsFulfilled()
{
  var fFulfilled;
  if(GetClanCrew()>=6) fFulfilled++;
  if(GetClanMystic(GetClan(GetCursor(GetPlayerByIndex())))) fFulfilled++;
  return(fFulfilled==2);
}

public func GetClanCrew()
{
  var obj = FindMainTent(GetClan(GetCursor(GetPlayerByIndex())));
  if(obj) return(obj->~GetClanCount());
}

public func Fulfill()
{
  goalFulfilled = 1;
}

public func SetMessages(strFulfilled, strUnfulfilled)
{
  messageFulfilled = strFulfilled;
  messageUnfulfilled = strUnfulfilled;  
}

protected func Activate(iPlayer)
{
  // Erfüllt
  if (IsFulfilled()) 
    return(MessageWindow(Format(messageFulfilled), iPlayer));
  // Noch nicht erfüllt
  else
    return(MessageWindow(Format(messageUnfulfilled, GetClanCrew(),6,
			    GetClanMystic(GetClan(GetCursor(GetPlayerByIndex()))), 100), iPlayer));
}
