/* Dialog: Harry */

#strict
#appendto _TLK

static g_fHarrySpoken;

public func DlgHarryStart()
  {
  if (g_fHarrySpoken==2) return(DlgHarry12(), iTalkIndex=12);
  if(ObjectDistance(FindObject(BCOA))<100) return(DlgHarry14(), iTalkIndex=14);
  if (g_fHarrySpoken==1) return(DlgHarry13(), iTalkIndex=13);
  MessageEx("$Dlg_Harry0$", pPlayer);
  return(1);
  }

private func DlgHarry1()
  {
  MessageEx("$Dlg_Harry1$", pNPC);
  }

private func DlgHarry2()
  {
  MessageEx("$Dlg_Harry2$", pPlayer);
  }

private func DlgHarry3()
  {
  MessageEx("$Dlg_Harry3$", pNPC);
  }

private func DlgHarry4()
  {
  MessageEx("$Dlg_Harry4$", pPlayer);
  }

private func DlgHarry5()
  {
  MessageEx("$Dlg_Harry5$", pNPC);
  }

private func DlgHarry6()
  {
  MessageEx("$Dlg_Harry6$", pNPC);
  }

private func DlgHarry7()
  {
  MessageEx("$Dlg_Harry7$", pPlayer);
  }

private func DlgHarry8()
  {
  MessageEx("$Dlg_Harry8$", pNPC);
  }

private func DlgHarry9()
  {
  MessageEx("$Dlg_Harry9$", pPlayer);
  }

private func DlgHarry10()
  {
  MessageEx("$Dlg_Harry10$", pNPC);
  }

private func DlgHarry11()
  {
  MessageEx("$Dlg_Harry11$", pNPC);
  g_fHarrySpoken=1;
  }

private func DlgHarry12()
  {
  MessageEx("$Dlg_Harry12$", pPlayer);
  StopDialog();
  }

private func DlgHarry13()
  {
  MessageEx("$Dlg_Harry13$", pNPC);
  StopDialog();
  }

private func DlgHarry14()
  {
  MessageEx("$Dlg_Harry14$", pPlayer);
  SetObjectLayer(this(), FindObject(BCOA));
  }

private func DlgHarry15()
  {
  MessageEx("$Dlg_Harry15$", pNPC);
  }

private func DlgHarry16()
  {
  MessageEx("$Dlg_Harry16$", pNPC);
  var pKey = CreateObject(_KEY);
  LocalN("iRefID", pKey) = 3;
  SetClrModulation(RGB(0,255), pKey);
  Enter(pPlayer, pKey);
  g_fHarrySpoken=2;
  }

private func DlgHarry17()
  {
  MessageEx("$Dlg_Harry17$", pPlayer);
  StopDialog();
  }
