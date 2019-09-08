/* Dialog: Extro */

#strict
#appendto _TLK

private func MovExtro0()
  {
  MessageEx("$Mov_Extro0$", pPlayer);
  iWait=126;
  }

private func MovExtro1()
  {
  MessageEx("$Mov_Extro1$", pPlayer);
  iWait=174;
  }

private func MovExtro3()
  {
  StopDialog();
  GameOver();
  }
