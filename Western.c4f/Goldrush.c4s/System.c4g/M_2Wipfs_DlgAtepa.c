/* Dialog: Atepa */

#strict
#appendto _TLK

static g_fAtepaSpoken;

public func DlgAtepaStart()
  {
  if (g_fAtepaSpoken==1)return(DlgAtepa5(), iTalkIndex=7);
  MessageEx("$Dlg_Atepa0$", pPlayer);
  return(1);
  }

private func DlgAtepa1()
  {
  MessageEx("$Dlg_Atepa1$", pNPC);
  }

private func DlgAtepa2()
  {
  MessageEx("$Dlg_Atepa2$", pPlayer);
  }

private func DlgAtepa3()
  {
  MessageEx("$Dlg_Atepa3$", pNPC);
  var pChaska = FindCrewByName(INDI, "Chaska");
  SetCommand(pChaska, "MoveTo", 0, 2178, 350);
  AddCommand(pChaska, "Call", LocalN("pGrab",pNPC->FindObject(WGTW, 0, 0, -1,-1)), 0, 0, 0, 0, "ControlLeft");
  AddCommand(pChaska, "Grab", LocalN("pGrab",pNPC->FindObject(WGTW, 0, 0, -1,-1)));
  }

private func DlgAtepa4()
  {
  MessageEx("$Dlg_Atepa4$", pPlayer);
  g_fAtepaSpoken=1;
  StopDialog();
  }

private func DlgAtepa5()
  {
  MessageEx("$Dlg_Atepa5$", pNPC);
  StopDialog();
  }
