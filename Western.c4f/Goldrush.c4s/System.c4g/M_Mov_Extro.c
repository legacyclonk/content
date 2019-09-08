/* Dialog: Extro */

#strict
#appendto _TLK

public func MovExtroStart()
  {
  MessageEx("$Mov_Extro0$", pPlayer);
  iWait=76;
  return(1);
  }

private func MovExtro1()
  {
  MessageEx("$Mov_Extro1$", pPlayer);
  iWait=88;
  }

private func MovExtro2()
  {
  StopDialog();
  GainMissionAccess("WestGR");
  GameOver();
  }
