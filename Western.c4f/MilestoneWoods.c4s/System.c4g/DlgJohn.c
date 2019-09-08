/* Dialog: John */

#strict
#appendto _TLK

static g_fJohnSpoken;

public func DlgJohnStart()
  {
  if (g_fJohnSpoken==1) return(DlgJohn10(), iTalkIndex=10);
  return(1);
  }

private func DlgJohn1()
  {
  MessageEx("$Dlg_John1$", pPlayer);
  }

private func DlgJohn2()
  {
  MessageEx("$Dlg_John2$", pNPC);
  }

private func DlgJohn3()
  {
  MessageEx("$Dlg_John3$", pPlayer);
  }

private func DlgJohn4()
  {
  MessageEx("$Dlg_John4$", pNPC);
  }

private func DlgJohn5()
  {
  MessageEx("$Dlg_John5$", pPlayer);
  }

private func DlgJohn6()
  {
  MessageEx("$Dlg_John6$", pNPC);
  }
  
private func DlgJohn7()
  {
  MessageEx("$Dlg_John7$", pPlayer);
  }

private func DlgJohn8()
  {
  MessageEx("$Dlg_John8$", pNPC);
  }

private func DlgJohn9()
  {
  MessageEx("$Dlg_John9$", pPlayer);
  g_fJohnSpoken=1;
  StopDialog();
  }

private func DlgJohn10()
  {
  MessageEx("$Dlg_John10$", pNPC);
  }

private func DlgJohn11()
  {
  var pHorse = pNPC->FindObject(HORS, -40, -40, 80, 80);
  if(!pHorse) pHorse = pNPC->FindObject(HRSS, -40, -40, 80, 80);
  if(!pHorse)
    {
    MessageEx("$Dlg_John11b$", pPlayer);
    return(StopDialog());
    }
  SetObjectLayer(pHorse, pHorse);
  MessageEx("$Dlg_John11$", pPlayer);
  }

private func DlgJohn12()
  {
  MessageEx("$Dlg_John12$", pNPC);
  FindObject(DYNM, 0, 0, -1, -1)->StartWork(pNPC);
  StopDialog();
  RemoveObject();
  }
