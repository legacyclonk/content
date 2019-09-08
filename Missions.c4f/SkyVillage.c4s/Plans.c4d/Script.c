/*-- Bauplan --*/

#strict 2

local plan_id;

public func Activate(object clnk)
{ [$CtrlActivateDesc$]
  var plr = GetController(clnk);
  if (GetPlrKnowledge(plr, plan_id))
  {
    PlrMessage("$MsgKnown$", plr, GetName(0, plan_id));
  }
  else
  {
    SetPlrKnowledge(plr, plan_id);
    Sound("Ding", true, 0, 100, plr+1);
    PlrMessage("$MsgActivate$", plr, GetName(0, plan_id));
  }
  return true;
}