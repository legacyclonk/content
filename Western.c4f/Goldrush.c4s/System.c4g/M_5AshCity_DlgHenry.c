/* Dialog: Henry */

#strict
#appendto _TLK

static g_fHenrySpoken;

public func DlgHenryStart()
  {
  if (g_fHenrySpoken==1) return(DlgHenry1(), iTalkIndex=1);
  if (g_fHenrySpoken==2) return(DlgHenry4(), iTalkIndex=4);
  MessageEx("$Dlg_Henry0$", pNPC);
  return(1);
  }

private func DlgHenry1()
  {
  MessageEx("$Dlg_Henry1$", pPlayer);
  g_fHenrySpoken=1;
  }

private func DlgHenry2()
  {
  if(!FindObject(SWWB, 1203-GetX(), 287-GetY())) return(DlgHenry3(), iTalkIndex=3);
  MessageEx("$Dlg_Henry2$", pNPC);
  StopDialog();
  }

private func DlgHenry3()
  {
  MessageEx("$Dlg_Henry3$", pNPC);
  g_fHenrySpoken=2;
  }

private func DlgHenry4()
  {
  MessageEx("$Dlg_Henry4$", pNPC);
  }

private func DlgHenry5()
  {
  var pGold = FindContents(GOLD, pPlayer);	  
  if(!pGold)
  {
    MessageEx("$Dlg_Henry5b$", pPlayer);	  
    StopDialog();
    return();
  }
  MessageEx("$Dlg_Henry5$", pPlayer);
  RemoveObject(pGold);
  Sound("Cash");
  }

private func DlgHenry6()
  {
  MessageEx("$Dlg_Henry6$", pNPC);
  for(var i=0;i<5;i++)
    CreateContents(WOOD, FindObject(SAWW, 0, 0, -1, -1));
  StopDialog();
  }
