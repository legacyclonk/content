/* Dialog: Jack */

#strict
#appendto _TLK

static g_fJackSpoken;

public func DlgJackStart()
  {
  if (g_fJackSpoken==1)
  {
    if(GetWealth(GetOwner(pPlayer))>=100) return(DlgJack8(), iTalkIndex=8);
    return(DlgJack7(), iTalkIndex=7);
  }
  if (g_fJackSpoken==2)
  {
    if(GetWealth(GetOwner(pPlayer))>=200) return(DlgJack10(), iTalkIndex=10);
    return(DlgJack9(), iTalkIndex=9);
  }
  if (g_fJackSpoken==3)
  {
    if(GetWealth(GetOwner(pPlayer))>=300) return(DlgJack12(), iTalkIndex=12);
    return(DlgJack11(), iTalkIndex=11);
  }
  if (g_fJackSpoken==4) return(DlgJack13(), iTalkIndex=13);
  return(1);
  }

private func DlgJack1()
  {
  MessageEx("$Dlg_Jack1$", pPlayer);
  }

private func DlgJack2()
  {
  MessageEx("$Dlg_Jack2$", pNPC);
  }

private func DlgJack3()
  {
  MessageEx("$Dlg_Jack3$", pPlayer);
  }

private func DlgJack4()
  {
  MessageEx("$Dlg_Jack4$", pNPC);
  }

private func DlgJack5()
  {
  MessageEx("$Dlg_Jack5$", pPlayer);
  }

private func DlgJack6()
  {
  MessageEx("$Dlg_Jack6$", pNPC);
  g_fJackSpoken=1;
  DoHomebaseMaterial(GetOwner(pPlayer), CNKT, 2);
  DoHomebaseMaterial(GetOwner(pPlayer), LNKT, 1);
  DoHomebaseMaterial(GetOwner(pPlayer), WOOD, 5);
  DoHomebaseMaterial(GetOwner(pPlayer), METL, 8);
  DoHomebaseMaterial(GetOwner(pPlayer), LOAM, 5);
  DoHomebaseMaterial(GetOwner(pPlayer), FLNT, 4);
  DoHomebaseMaterial(GetOwner(pPlayer), LORY, 1);
  StopDialog();
  }
  
private func DlgJack7()
  {
  MessageEx("$Dlg_Jack7$", pNPC);
  StopDialog();
  }

private func DlgJack8()
  {
  MessageEx("$Dlg_Jack8$", pNPC);
  g_fJackSpoken = 2;
  DoHomebaseMaterial(GetOwner(pPlayer), CNKT, 2);
  DoHomebaseMaterial(GetOwner(pPlayer), LNKT, 1);
  DoHomebaseMaterial(GetOwner(pPlayer), WOOD, 4);
  DoHomebaseMaterial(GetOwner(pPlayer), METL, 6);
  DoHomebaseMaterial(GetOwner(pPlayer), LOAM, 5);
  DoHomebaseMaterial(GetOwner(pPlayer), FLNT, 3);
  DoHomebaseMaterial(GetOwner(pPlayer), WINC, 1);
  DoHomebaseMaterial(GetOwner(pPlayer), REVR, 2);
  DoHomebaseMaterial(GetOwner(pPlayer), AMBO, 10);
  DoHomebaseMaterial(GetOwner(pPlayer), TRPR, 1);
  SetPlrKnowledge(GetOwner(pPlayer), FDRS);
  StopDialog();
  }

private func DlgJack9()
  {
  MessageEx("$Dlg_Jack9$", pNPC);
  StopDialog();
  }

private func DlgJack10()
  {
  MessageEx("$Dlg_Jack10$", pNPC);
  g_fJackSpoken=3;
  DoHomebaseMaterial(GetOwner(pPlayer), CNKT, 2);
  DoHomebaseMaterial(GetOwner(pPlayer), LNKT, 1);
  DoHomebaseMaterial(GetOwner(pPlayer), WOOD, 3);
  DoHomebaseMaterial(GetOwner(pPlayer), METL, 2);
  DoHomebaseMaterial(GetOwner(pPlayer), LOAM, 3);
  DoHomebaseMaterial(GetOwner(pPlayer), WINC, 1);
  DoHomebaseMaterial(GetOwner(pPlayer), REVR, 1);
  DoHomebaseMaterial(GetOwner(pPlayer), AMBO, 8);
  DoHomebaseMaterial(GetOwner(pPlayer), TRPR, 2);
  DoHomebaseMaterial(GetOwner(pPlayer), IGNB, 1);
  DoHomebaseMaterial(GetOwner(pPlayer), DYNB, 3);
  SetPlrKnowledge(GetOwner(pPlayer), AMBO);
  SetPlrKnowledge(GetOwner(pPlayer), GUNP);
  StopDialog();
  }

private func DlgJack11()
  {
  MessageEx("$Dlg_Jack11$", pNPC);
  StopDialog();
  }

private func DlgJack12()
  {
  MessageEx("$Dlg_Jack12$", pNPC);
  if(g_fJackSpoken<4)
  {
    DoHomebaseMaterial(GetOwner(pPlayer), CNKT, 1);
    DoHomebaseMaterial(GetOwner(pPlayer), WOOD, 2);
    DoHomebaseMaterial(GetOwner(pPlayer), METL, 1);
    DoHomebaseMaterial(GetOwner(pPlayer), AMBO, 5);
    DoHomebaseMaterial(GetOwner(pPlayer), GUNP, 6);
    DoHomebaseMaterial(GetOwner(pPlayer), IGNB, 1);
    DoHomebaseMaterial(GetOwner(pPlayer), DYNB, 4);
    DoHomebaseMaterial(GetOwner(pPlayer), TRPR, 2);
    SetPlrKnowledge(GetOwner(pPlayer), DYNB);
    SetPlrKnowledge(GetOwner(pPlayer), WINC);
    SetPlrKnowledge(GetOwner(pPlayer), REVR);
  }
  g_fJackSpoken=4;
  StopDialog();
  }

private func DlgJack13()
  {
  MessageEx("$Dlg_Jack13$", pNPC);
  StopDialog();
  }
