/*-- Wandnachricht --*/

#strict

local sText; // Text string

protected func Initialize()
  {
  SetAction("Standing");
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

global func SignpostText(string sScriptName, string sToText)
  {
  var obj;
  while (obj = FindObject(SGNP, 0,0,0,0, 0, 0,0, 0, obj))
    if (obj->GetName() eq sScriptName)
      {
      obj->SetText(sToText);
      obj->SetName();
      return(true);
      }
  }
