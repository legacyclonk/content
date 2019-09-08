
#strict

protected func Initialize()
{
  SetAction("GetBuild");
  return(1);
}

protected func CheckForParts()
{
  var pPart;
  if(GetPhase() == 0)
   if(pPart = FindObject(_PA1,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 1)
   if(pPart = FindObject(_PA2,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 2)
   if(pPart = FindObject(_PA3,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 3)
   if(pPart = FindObject(_PA4,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 4)
   if(pPart = FindObject(_PA5,-30,-30,60,60)) return(AddPart(pPart));
  if(GetPhase() == 5)
   if(pPart = FindObject(_PA6,-30,-30,60,60)) return(AddPart(pPart));
  return(0);
}

private func Message(szMessage)
{
  CustomMessage(szMessage, 0, -1, 90,70, 0xffffff, MD69, Format("%i", _STA));
}

private func AddPart(pPart)
{
  SetPhase(GetPhase() + 1);
  Sound("Applause");

  if(GetID(pPart) == _PA1)
  {
    Message("$MsgPart1$");
    AddHomebaseMaterial(CLNK, 3);
  }

  if(GetID(pPart) == _PA2)
  {
    Message("$MsgPart2$");
    AddKnowledge(ELEV);
    AddKnowledge(LORY);
  }

  if(GetID(pPart) == _PA3)
  {
    Message("$MsgPart3$");
    AddKnowledge(_PA4);
    AddKnowledge(WRKS);
  }

  if(GetID(pPart) == _PA4)
  {
    Message("$MsgPart4$");
    AddHomebaseMaterial(BALN,2);
    AddKnowledge(BALN);
  }

  if(GetID(pPart) == _PA5)
  {
    Message("$MsgPart5$");
    AddHomebaseMaterial(FLAG, 2);
    AddHomebaseMaterial(BLMP, 1);
    AddKnowledge(BLMP);
  }

  if(GetID(pPart) == _PA6)
  {
    FindObject(SCRG)->Fulfill();
  }
  
  RemoveObject(pPart);
  return(1);        
}
