
#strict

local fPlayerWasThere, fGotBrokenMsg;

protected func Initialize()
{
  SetAction("GetBuild");
  return(1);
}

protected func CheckForParts()
{
  if (!fPlayerWasThere)
    if (FindObject(0, -30,-30, 60,60, OCF_CrewMember))
      { Sound("Applause"); Message("$MsgPart0$"); fPlayerWasThere=true; return(1); }
  var pPart;
  if(GetPhase() == 0)
   if(pPart = FindObject(_PA1,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 1)
   if(pPart = FindObject(_PA2,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 2)
   if(pPart = FindObject(_PA3,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 3)
   if(pPart = FindObject(_PA4,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 3 && !fGotBrokenMsg)
   if(pPart = FindObject(_PB4,-30,-30,60,60))
      { Message("$MsgPart4B$"); fGotBrokenMsg=true; return(1); }
  if(GetPhase() == 4)
   if(pPart = FindObject(_PA5,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 5)
   if(pPart = FindObject(_PA6,-30,-30,60,60)) return(AddPart(pPart));
  return(0);
}

private func AddPart(pPart)
{
  SetPhase(GetPhase() + 1);
  Sound("Applause");

  var clonk;
  while (clonk = FindObject(0, -300,-150,600,300, OCF_Alive, 0,0, 0, clonk))
    CreateObject(MGHL)->Activate(clonk);

  if(GetID(pPart) == _PA1)
  {
    Message("$MsgPart1$");
  }

  if(GetID(pPart) == _PA2)
  {
    Message("$MsgPart2$");
  }

  if(GetID(pPart) == _PA3)
  {
    Message("$MsgPart3$");
  }

  if(GetID(pPart) == _PA4)
  {
    Message("$MsgPart4$");
  }

  if(GetID(pPart) == _PA5)
  {
    Message("$MsgPart5$");
  }

  if(GetID(pPart) == _PA6)
    {
    Message("$MsgPart6$");
    if (ObjectCount(SCRG)) FindObject(SCRG)->Fulfill();
    }

  RemoveObject(pPart);
  return(1);        
}
