/* Dialog: Ezhno */

#strict
#appendto _TLK

static g_fEzhnoSpoken;
static g_fEzhnoSpokenB;

public func DlgEzhnoStart()
  {
  if( FindContents(CRYS, pPlayer) && !g_fEzhnoSpokenB ) return(DlgEzhno15(), iTalkIndex=15); 

  if (g_fEzhnoSpoken==1) return(DlgEzhno11(), iTalkIndex=11);
  if (g_fEzhnoSpoken==2) return(DlgEzhno14(), iTalkIndex=14);
  return(1);
  }

private func DlgEzhno1()
  {
  MessageEx("$Dlg_Ezhno1$", pNPC);
  }

private func DlgEzhno2()
  {
  MessageEx("$Dlg_Ezhno2$", pPlayer);
  }

private func DlgEzhno3()
  {
  MessageEx("$Dlg_Ezhno3$", pNPC);
  }

private func DlgEzhno4()
  {
  MessageEx("$Dlg_Ezhno4$", pPlayer);
  }

private func DlgEzhno5()
  {
  MessageEx("$Dlg_Ezhno5$", pNPC);
  }

private func DlgEzhno6()
  {
  MessageEx("$Dlg_Ezhno6$", pPlayer);
  }
  
private func DlgEzhno7()
  {
  MessageEx("$Dlg_Ezhno7$", pNPC);
  }

private func DlgEzhno8()
  {
  MessageEx("$Dlg_Ezhno8$", pPlayer);
  }

private func DlgEzhno9()
  {
  MessageEx("$Dlg_Ezhno9$", pNPC);
  }

private func DlgEzhno10()
  {
  MessageEx("$Dlg_Ezhno10$", pPlayer);
  g_fEzhnoSpoken=1;
  StopDialog();
  }

private func DlgEzhno11()
  {
  MessageEx("$Dlg_Ezhno11$", pNPC);
  }

private func DlgEzhno12()
  {
  var pGoblet = FindContents(GBLT, pPlayer);	  
  if(pGoblet)
    {
    MessageEx("$Dlg_Ezhno12$", pPlayer);
    RemoveObject(pGoblet);
    }
  else
    {
    MessageEx("$Dlg_Ezhno12b$", pPlayer);
    StopDialog();
    }
  }

private func DlgEzhno13()
  {
  MessageEx("$Dlg_Ezhno13$", pNPC);
  var pKey = CreateObject(_KEY);
  LocalN("iRefID", pKey) = 1;
  Enter(pPlayer, pKey);
  g_fEzhnoSpoken=2;
  StopDialog();
  }

private func DlgEzhno14()
  {
  MessageEx("$Dlg_Ezhno14$", pNPC);
  StopDialog();
  }

private func DlgEzhno15()
  {
  MessageEx("$Dlg_Ezhno15$", pNPC);
  }

private func DlgEzhno16()
  {
  MessageEx("$Dlg_Ezhno16$", pPlayer);
  }

private func DlgEzhno17()
  {
  MessageEx("$Dlg_Ezhno17$", pNPC);
  }

private func DlgEzhno18()
  {
  MessageEx("$Dlg_Ezhno18$", pPlayer);
  var pCrystal = FindContents(CRYS, pPlayer);
  if(!pCrystal) return(StopDialog());
  RemoveObject(pCrystal);
  CreateContents(CRWN, pPlayer);
  g_fEzhnoSpokenB=1;
  StopDialog();			  
  }
