/* Dialog: Luyu */

#strict
#appendto _TLK

static g_fLuyuSpoken;

public func DlgLuyuStart()
  {
  if (g_fLuyuSpoken==1) return(DlgLuyu5(), iTalkIndex=5);
  if (g_fLuyuSpoken==2) return(DlgLuyu10(), iTalkIndex=10);
  MessageEx("$Dlg_Luyu0$", pNPC);
  return(1);
  }

private func DlgLuyu1()
  {
  if(!g_fWillySpoken)
  {
    MessageEx("$Dlg_Luyu1b$", pPlayer);
    StopDialog();
  }	  
  MessageEx("$Dlg_Luyu1$", pPlayer);
  }

private func DlgLuyu2()
  {
  MessageEx("$Dlg_Luyu2$", pNPC);
  }

private func DlgLuyu3()
  {
  MessageEx("$Dlg_Luyu3$", pPlayer);
  }

private func DlgLuyu4()
  {
  MessageEx("$Dlg_Luyu4$", pNPC);
  g_fLuyuSpoken=1;
  StopDialog();
  }

private func DlgLuyu5()
  {
  MessageEx("$Dlg_Luyu5$", pNPC);
  }

private func DlgLuyu6()
  {
  var pBook = FindContents(_BOK, pPlayer);	  
  if(!pBook)
  {
    MessageEx("$Dlg_Luyu6b$", pPlayer);	  
    StopDialog();
    return();
  }
  MessageEx("$Dlg_Luyu6$", pPlayer);
  RemoveObject(pBook);
  }

private func DlgLuyu7()
  {
  MessageEx("$Dlg_Luyu7$", pNPC);
  SetGraphics("B", CreateContents(BCRW, pPlayer));
  }

private func DlgLuyu8()
  {
  MessageEx("$Dlg_Luyu8$", pNPC);
  }

private func DlgLuyu9()
  {
  MessageEx("$Dlg_Luyu9$", pPlayer);
  g_fLuyuSpoken=2;
  }

private func DlgLuyu10()
  {
  MessageEx("$Dlg_Luyu10$", pNPC);
  }

private func DlgLuyu11()
  {
  MessageEx("$Dlg_Luyu11$", pPlayer);
  }

private func DlgLuyu12()
  {
  MessageEx("$Dlg_Luyu12$", pNPC);
  StopDialog();
  }
