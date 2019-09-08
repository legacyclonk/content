/*-- Scriptdefiniertes Spielziel --*/

#strict
#include GOAL
        
local fTalked;

protected func Initialize()
{
  return(_inherited());
}          
        
public func IsFulfilled()
{
  if(GetWealth(GetPlayerByIndex())<400) return();
  if(g_fEzhnoSpoken<2) return();
  if(!fTalked){ fTalked=1; return(0, StartMovie("Ontro", GetCrew(GetPlayerByIndex()), GetCrew(GetPlayerByIndex(), 1))); }
  if(!FindObject(SALO, -GetX(), -GetY(), 590, 240, OCF_Fullcon)) return();
  return(1);
}

protected func Activate(iPlayer)
{
  // Erfüllt
  if (IsFulfilled()) 
    return(MessageWindow(Format("$MsgGoalFulfilled$"), iPlayer));
  // Noch nicht erfüllt
  else
  {
    var szMessage = "";
    if(GetWealth(iPlayer)<400) szMessage = Format("%s$MsgGoalUnfulfilled$", szMessage, 400-GetWealth(iPlayer));
    if(g_fEzhnoSpoken<2) szMessage = Format("%s$MsgGoalUnfulfilledB$", szMessage);
    if(fTalked && !FindObject(SALO, -GetX(), -GetY(), 590, 240, OCF_Fullcon)) szMessage = Format("%s$MsgGoalUnfulfilledC$", szMessage);
    return(MessageWindow(szMessage, iPlayer));
  }
}
