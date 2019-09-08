/* Dialog: Gerry */

#strict
#appendto _TLK

static g_fGerrySpoken;

public func DlgGerryStart()
  {
  if (g_fGerrySpoken==1) return(DlgGerry4(), iTalkIndex=4);
  MessageEx("$Dlg_Gerry0$", pPlayer);
  return(1);
  }

private func DlgGerry1()
  {
  if(!FindObject(BNKB, 1788-GetX(), 237-GetY())) return(DlgGerry3(), iTalkIndex=3);
  MessageEx("$Dlg_Gerry1$", pNPC);
  StopDialog();
  }

private func DlgGerry2()
  {
  MessageEx("$Dlg_Gerry2$", pNPC);
  }

private func DlgGerry3()
  {
  MessageEx("$Dlg_Gerry3$", pNPC);
  var pBag = CreateContents(MBAG, pPlayer);
  LocalN("m_val", pBag) = 50;
  g_fGerrySpoken=1;
  StopDialog();
  }

private func DlgGerry4()
  {
  MessageEx("$Dlg_Gerry4$", pNPC);
  }

private func DlgGerry5()
  {
  MessageEx("$Dlg_Gerry5$", pPlayer);
  StopDialog();
  }
