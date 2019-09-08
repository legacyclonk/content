/* Dialog: Smith */

#strict
#appendto _TLK

static g_fSmithSpoken;

public func DlgSmithStart()
  {
  if (g_fSmithSpoken==1) return(DlgSmith2(), iTalkIndex=2);
  if (g_fSmithSpoken==2) return(DlgSmith6(), iTalkIndex=6);
  MessageEx("$Dlg_Smith0$", pPlayer);
  return(1);
  }

private func DlgSmith1()
  {
  MessageEx("$Dlg_Smith1$", pNPC);
  g_fSmithSpoken=1;
  }

private func DlgSmith2()
  {
  if(!FindObject(WRWB, 1599-GetX(), 410-GetY())) return();//DlgSmith3(), iTalkIndex=4);
  MessageEx("$Dlg_Smith2$", pNPC);
  StopDialog();
  }

private func DlgSmith3()
  {
  MessageEx("$Dlg_Smith3$", pNPC);
  }

private func DlgSmith4()
  {
  MessageEx("$Dlg_Smith4$", pNPC);
  SetPlrKnowledge(GetOwner(pPlayer), DYNM);
  }

private func DlgSmith5()
  {
  MessageEx("$Dlg_Smith5$", pNPC);
  g_fSmithSpoken=2;
  }

private func DlgSmith6()
  {
  MessageEx("$Dlg_Smith6$", pNPC);
  }

private func DlgSmith7()
  {
  var pBag = FindContents(MBAG, pPlayer);	  
  if(!pBag)
  {
    MessageEx("$Dlg_Smith7b$", pPlayer);	  
    StopDialog();
    return();
  }
  MessageEx("$Dlg_Smith7$", pPlayer);
  }

private func DlgSmith8()
  {
  var pBag = FindContents(MBAG, pPlayer);	  
  if(LocalN("m_val", pBag)<50)
  {
    MessageEx("$Dlg_Smith8b$", pPlayer);
    StopDialog();
    return();
  }
  RemoveObject(pBag);
  MessageEx("$Dlg_Smith8$", pNPC);
  var pCannon = CreateObject(CCAN);
  Enter(FindObject(WRKW, 1599-GetX(), 410-GetY()), pCannon);
// Wenn man Munition extra erhält so macht man beim Kaufen einen Gewinn von 62 Clunkern
/*
  for(var i=0;i<4;i++)
  {
    CreateContents(GUNP, pCannon);
    CreateContents(CABL, pCannon);
  }
*/
  SetPlrKnowledge(GetOwner(pPlayer), CABL);
  SetCommand(pCannon, "Exit");
  StopDialog();
  }
