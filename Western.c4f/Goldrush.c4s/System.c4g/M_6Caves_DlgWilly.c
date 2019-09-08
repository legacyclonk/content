/* Dialog: Willy */

#strict
#appendto _TLK

static g_fWillySpoken;
static g_fWillyBookGiven;

public func DlgWillyStart()
  {
  if (g_fWillySpoken==6) return(DlgWilly11(), iTalkIndex=11);
  if (g_fLuyuSpoken==1 && !g_fWillyBookGiven) return(DlgWilly12(), iTalkIndex=12);
  if (g_fWillySpoken==5) return(DlgWilly11(), iTalkIndex=11);
  if (g_fWillySpoken>=1) return(DlgWilly7(), iTalkIndex=7);
  MessageEx("$Dlg_Willy0$", pNPC);
  return(1);
  }

private func DlgWilly1()
  {
  MessageEx("$Dlg_Willy1$", pPlayer);
  }

private func DlgWilly2()
  {
  MessageEx("$Dlg_Willy2$", pNPC);
  }

private func DlgWilly3()
  {
  MessageEx("$Dlg_Willy3$", pPlayer);
  }

private func DlgWilly4()
  {
  MessageEx("$Dlg_Willy4$", pNPC);
  }

private func DlgWilly5()
  {
  MessageEx("$Dlg_Willy5$", pPlayer);
  }

private func DlgWilly6()
  {
  MessageEx("$Dlg_Willy6$", pNPC);
  g_fWillySpoken=1;
  }

private func DlgWilly7()
  {
  MessageEx("$Dlg_Willy7$", pNPC);	  
  }

private func DlgWilly8()
  {
  var pWhisky = FindContents(WHKY, pPlayer);	  
  if(!pWhisky)
  {
    MessageEx("$Dlg_Willy8b$", pPlayer);	  
    StopDialog();
    return();
  }
  MessageEx("$Dlg_Willy8$", pPlayer);
  RemoveObject(pWhisky);
  g_fWillySpoken++;
  if(g_fWillySpoken>=4) iTalkIndex=9;
  }

private func DlgWilly9()
  {
  MessageEx("$Dlg_Willy9$", pNPC);
  ObjectSetAction(pNPC, "Drink");
  Sound("Drink");
  iTalkIndex=7;
  }

private func DlgWilly10()
  {
  MessageEx("$Dlg_Willy10$", pNPC);
  ObjectSetAction(pNPC, "Drink");
  Sound("Drink");
  var pCrown = CreateObject(BCRW);
  Enter(pPlayer, pCrown);
  g_fWillySpoken=5;
  StopDialog();
  }

private func DlgWilly11()
  {
  MessageEx("$Dlg_Willy11$", pPlayer);
  StopDialog();
  }

private func DlgWilly12()
  {
  MessageEx("$Dlg_Willy12$", pPlayer);
  }

private func DlgWilly13()
  {
  MessageEx("$Dlg_Willy13$", pNPC);
  CreateContents(_BOK, pPlayer);
  g_fWillyBookGiven=1;
  }

private func DlgWilly14()
  {
  MessageEx("$Dlg_Willy14$", pPlayer);
  StopDialog();
  }
