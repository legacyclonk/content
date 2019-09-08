/*-- Wandnachricht --*/

#strict

local sText; // Text string

protected func Initialize()
  {
  SetAction("Standing");
  }

protected func ControlUp(object pPlayer)
  { [$MsgRead$]
  if(Local()==1) return(Message("\"$TxtCoalstown$\"", pPlayer));
  if(Local()==2) return(Message("\"$TxtFrontier$\"", pPlayer));
  if(Local()==3) return(Message("\"$TxtAshCity$\"", pPlayer));
  if(Local()==4) return(Message("\"$TxtIndian$\"", pPlayer));
  if(Local()==5) return(Message("\"$TxtCaves$\"", pPlayer));
  if(Local()==6) return(Message("\"$TxtAshCity$\"", pPlayer));
  if(Local()==7) return(Message("\"$TxtCanyon$\"", pPlayer));
  if(Local()==8) return(Message("\"$TxtBandits$\"", pPlayer));
  return(Message("\"%s\"", pPlayer, sText));
  }

protected func ControlDig(pClonk)
{
  [$MsgTravel$|Image=COAC|Condition=IsTravelPoint]
  CreateMenu (COAC, pClonk, this(), 0, "$MsgTravelTo$", 0, 1);
  if(fTesting)
  {
      AddMenuItem("$MsgCoalstown$", "Travel(-1)", COAL, pClonk);
      AddMenuItem("$MsgIndian$", "Travel(0)", SPR1, pClonk);
      AddMenuItem("$MsgAshCity$", "Travel(1)", SKUL, pClonk);
      AddMenuItem("$MsgCaves$", "Travel(2)", _STA, pClonk);
      AddMenuItem("$MsgCanyon$", "Travel(3)", GOLD, pClonk);
      return();
  }
  if(Local()==1)
  {
    AddMenuItem("$MsgCoalstown$", "Travel(-1)", COAL, pClonk);
  }
  if(Local()==3)
  {
    AddMenuItem("$MsgAshCity$", "Travel(1)", SKUL, pClonk);
    if(g_iFarestSection==2) AddMenuItem("$MsgCaves$", "Travel(2)", _STA, pClonk);
  }
  if(Local()==4)
  {
    AddMenuItem("$MsgIndian$", "Travel(0)", SPR1, pClonk);
  }
  if(Local()==5)
  {
    AddMenuItem("$MsgCaves$", "Travel(2)", _STA, pClonk);
  }
  if(Local()==6)
  {
    AddMenuItem("$MsgIndian$", "Travel(0)", SPR1, pClonk);
    AddMenuItem("$MsgAshCity$", "Travel(1)", SKUL, pClonk);
  }
  if(Local()==7)
  {
    AddMenuItem("$MsgCanyon$", "Travel(3)", GOLD, pClonk);
  }
}

public func Travel(iIndex)
{
  if(iIndex==-1) return(Message("$NotBackToCoalsTown$", this()));	
  var pWagon = FindObject(COAC);
  var pClonk = FindObject(0, 0, 0, 0, 0, OCF_CrewMember, "Push", this());
  var pHorse = GetActionTarget(0, FindObject(CHBM,0,0,0,0,0,"Connect", pHorse));
  if(!pClonk) return();
  if(!pWagon) return(Message("$ErrNoWagon$", this()));
  if(!pHorse) return(Message("$ErrNoHorse$", this()));
  if(ObjectDistance(pWagon)>100) return(Message("$ErrNoWagon$", this()));
  var obj;
  while(obj = FindObject(0, 0, 0, 0, 0, OCF_Alive | OCF_CrewMember, 0, 0, 0, obj)) 
  {
    if(GetOwner(obj)<0) continue;
    if(obj->~IsRiding()) if(GetActionTarget(0,obj)==pWagon || GetActionTarget(0,obj)==pHorse) continue;
    if(obj->~IsRiding()) if(ObjectDistance(obj)<100) continue;
    if(obj != pClonk) if(Contained(obj)!=pWagon) return(Message("$ErrNoCrew$ %s", this(), GetName(obj)));
  }
  if(iIndex==0) return(ChangeSection("Main"));
  if(iIndex==1) return(ChangeSection("AshCity"));
  if(iIndex==2) return(ChangeSection("Caves"));
  if(iIndex==3) return(StartMovie("Extro", pClonk, this(), 1));
}

public func IsTravelPoint()
{
  if(Local()==2) return(0);
  return(1);
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
