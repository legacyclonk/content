/*-- Wandnachricht --*/

#strict

local sText; // Text string

protected func Initialize()
  {
  SetAction("Walling");
  }

protected func ControlUp(object pPlayer)
  { [$CtrlRead$]
  return(Message("\"%s\"", pPlayer, sText));
  }

public func SetText(string sToText)
  {
  sText = sToText;
  return(true);
  }

global func AssignWScriptText(string sScriptName, string sToText)
  {
  var obj;
  while (obj = FindObject(_WSC, 0,0,0,0, 0, 0,0, 0, obj))
    if (obj->GetName() eq sScriptName)
      {
      obj->SetText(sToText);
      obj->SetName();
      return(true);
      }
  }
