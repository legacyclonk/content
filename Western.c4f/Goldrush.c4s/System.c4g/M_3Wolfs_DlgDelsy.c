/* Dialog: Delsy */

#strict
#appendto _TLK

static g_fDelsySpoken;

public func DlgDelsyStart()
  {
  if(g_fDelsySpoken>5) return(DlgDelsy11(), iTalkIndex=11);
  if(g_fDelsySpoken) return(DlgDelsy6(), iTalkIndex=6);
  MessageEx("$Dlg_Delsy0$", pPlayer);
  return(1);
  }

private func DlgDelsy1()
  {
  MessageEx("$Dlg_Delsy1$", pNPC);
  }

private func DlgDelsy2()
  {
  MessageEx("$Dlg_Delsy2$", pPlayer);
  }

private func DlgDelsy3()
  {
  MessageEx("$Dlg_Delsy3$", pNPC);
  }

private func DlgDelsy4()
  {
  MessageEx("$Dlg_Delsy4$", pPlayer);
  }

private func DlgDelsy5()
  {
  MessageEx("$Dlg_Delsy5$", pPlayer);
  }

private func DlgDelsy6()
  {
  MessageEx("$Dlg_Delsy6$", pNPC);
  g_fDelsySpoken=1;
  }

private func DlgDelsy7()
  {
  // Nach einer vollen Whiskeyflasche suchen
  var pWhisky = FindObject(WHKY, 0, 0, 0, 0, 0, "Full", 0, pPlayer);
  if(pWhisky)
  {
    MessageEx("$Dlg_Delsy7$", pPlayer);
    RemoveObject(pWhisky);
  }
  else
  {
    MessageEx("$Dlg_Delsy7b$", pPlayer);
    StopDialog();
  }
  }

private func DlgDelsy8()
  {
//  CreateContents(WHKY, pNPC)->Activate(pNPC);	  
  ObjectSetAction(pNPC, "Drink");
  MessageEx("$Dlg_Delsy8$", pNPC);
  g_fDelsySpoken++;
  }

private func DlgDelsy9()
  {
  MessageEx("$Dlg_Delsy9$", pNPC);
  }

private func DlgDelsy10()
  {
  // Besoffen: Dann nimmt er auch leere Flaschen?
  var pWhisky = FindContents(WHKY, pPlayer);
  if(pWhisky)
  {
    MessageEx("$Dlg_Delsy10$", pPlayer);
    RemoveObject(pWhisky);
    if(g_fDelsySpoken<=5) iTalkIndex=7;
  }
  else
  {
    MessageEx("$Dlg_Delsy10b$", pPlayer);
    StopDialog();
  }
  }

private func DlgDelsy11()
  {
  MessageEx("$Dlg_Delsy11$", pNPC);
  }

private func DlgDelsy12()
  {
  MessageEx("$Dlg_Delsy12$", pPlayer);
  StopDialog();
  }
