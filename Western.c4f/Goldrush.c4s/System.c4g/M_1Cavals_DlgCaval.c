/* Dialog: Caval */

#strict
#appendto _TLK

static g_fCavalSpoken;

public func DlgCavalStart()
  {
  if (g_fCaptainSpoken==2)return(DlgCaval3(), iTalkIndex=3);
  MessageEx("$Dlg_Caval0$", pPlayer);
  return(1);
  }

private func DlgCaval1()
  {
  MessageEx("$Dlg_Caval1$", pNPC);
  }

private func DlgCaval2()
  {
  MessageEx("$Dlg_Caval2$", pNPC);
  StopDialog();
  }

private func DlgCaval3()
  {
  MessageEx("$Dlg_Caval3$", pNPC);
  StopDialog();
  }
