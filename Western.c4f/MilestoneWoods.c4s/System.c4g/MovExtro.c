/* Dialog: Extro */

#strict
#appendto _TLK

private func MovExtro0()
  {
  MessageEx("$Dlg_Trapper2$", pPlayer);
  FindObject(SALO, -GetX(), -GetY(), 590, 240, OCF_Fullcon)->Fight();
  iWait=196;
  }

private func MovExtro1()
  {
  GainMissionAccess("WestMW");
  GameOver();
  }
