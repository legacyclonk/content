/* Dialog: Dorkan */

#strict
#appendto _TLK

static g_fDorkanSpoken;

public func DlgDorkanStart()
  {
  if (g_fDorkanSpoken==1) return(DlgDorkan7(), iTalkIndex=7);
  MessageEx("$Dlg_Dorkan0$", pPlayer);
  return(1);
  }

private func DlgDorkan1()
  {
  if(!FindObject(DRSB, 1612-GetX(), 242-GetY())) return(DlgDorkan2(), iTalkIndex=2);
  MessageEx("$Dlg_Dorkan1$", pNPC);
  StopDialog();
  }

private func DlgDorkan2()
  {
  MessageEx("$Dlg_Dorkan2$", pNPC);
  }

private func DlgDorkan3()
  {
  MessageEx("$Dlg_Dorkan3$", pNPC);
  for(var i=0;i<5;i++)
    CreateContents(WHKY, pPlayer);	  
  }

private func DlgDorkan4()
  {
  MessageEx("$Dlg_Dorkan4$", pNPC);
  }

private func DlgDorkan5()
  {
  MessageEx("$Dlg_Dorkan5$", pNPC);
  g_fDorkanSpoken=1;
  }

private func DlgDorkan6()
  {
  MessageEx("$Dlg_Dorkan6$", pPlayer);
  StopDialog();
  }

private func DlgDorkan7()
  {
  MessageEx("$Dlg_Dorkan7$", pNPC);
  StopDialog();
  }
