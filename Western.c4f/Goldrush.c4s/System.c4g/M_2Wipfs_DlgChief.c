/* Dialog: Chief */

#strict
#appendto _TLK

static g_fChiefSpoken;

public func DlgChiefStart()
  {
  if (g_fChiefSpoken==1) return(DlgChief10(), iTalkIndex=10);
  MessageEx("$Dlg_Chief0$", pPlayer);
  return(1);
  }

private func DlgChief1()
  {
  MessageEx("$Dlg_Chief1$", pNPC);
  }

private func DlgChief2()
  {
  MessageEx("$Dlg_Chief2$", pPlayer);
  }

private func DlgChief3()
  {
  MessageEx("$Dlg_Chief3$", pNPC);
  }

private func DlgChief4()
  {
  MessageEx("$Dlg_Chief4$", pNPC);
  }

private func DlgChief5()
  {
  MessageEx("$Dlg_Chief5$", pPlayer);
  }

private func DlgChief6()
  {
  MessageEx("$Dlg_Chief6$", pNPC);
  }

private func DlgChief7()
  {
  MessageEx("$Dlg_Chief7$", pPlayer);
  }

private func DlgChief8()
  {
  MessageEx("$Dlg_Chief8$", pNPC);
  }

private func DlgChief9()
  {
  MessageEx("$Dlg_Chief9$", pNPC);
  Local(2, FindObject(MTIP, 0, 0, -1,-1)) = 1;
  g_fChiefSpoken=1;
  StopDialog();
  }

private func DlgChief10()
  {
  MessageEx("$Dlg_Chief10$", pNPC);
  StopDialog();
  }
