/* Dialog: Woff */

#strict
#appendto _TLK

static g_fWoffSpoken;

public func DlgWoffStart()
  {
  if (g_fWoffSpoken==1) return(DlgWoff4(), iTalkIndex=4);
  MessageEx("$Dlg_Woff0$", pPlayer);
  return(1);
  }

private func DlgWoff1()
  {
  MessageEx("$Dlg_Woff1$", pNPC);
  }

private func DlgWoff2()
  {
  MessageEx("$Dlg_Woff2$", pPlayer);
  }

private func DlgWoff3()
  {
  MessageEx("$Dlg_Woff3$", pNPC);
  Local(3, FindObject(MTIP, 0, 0, -1,-1)) = 1;
  g_fWoffSpoken=1;
  }

private func DlgWoff4()
  {
  MessageEx("$Dlg_Woff4$", pNPC);
  }

private func DlgWoff5()
  {
  MessageEx("$Dlg_Woff5$", pPlayer);
  StopDialog();
  }
