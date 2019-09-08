/* Dialog: Cowboy */

#strict
#appendto _TLK

static g_fCowboySpoken;

public func DlgCowboyStart()
  {
  if (g_fCowboySpoken==1) return(DlgCowboy9(), iTalkIndex=9);
  return(1);
  }

private func DlgCowboy1()
  {
  MessageEx("$Dlg_Cowboy1$", pPlayer);
  }

private func DlgCowboy2()
  {
  MessageEx("$Dlg_Cowboy2$", pNPC);
  }

private func DlgCowboy3()
  {
  MessageEx("$Dlg_Cowboy3$", pPlayer);
  }

private func DlgCowboy4()
  {
  MessageEx("$Dlg_Cowboy4$", pNPC);
  }

private func DlgCowboy5()
  {
  MessageEx("$Dlg_Cowboy5$", pNPC);
  }

private func DlgCowboy6()
  {
  MessageEx("$Dlg_Cowboy6$", pPlayer);
  }
  
private func DlgCowboy7()
  {
  MessageEx("$Dlg_Cowboy7$", pNPC);
  }

private func DlgCowboy8()
  {
  MessageEx("$Dlg_Cowboy8$", pPlayer);
  g_fCowboySpoken=1;
  StopDialog();
  }

private func DlgCowboy9()
  {
  MessageEx("$Dlg_Cowboy9$", pNPC);
  }

private func DlgCowboy10()
  {
  var pBag = FindContents(MBAG, pPlayer);	  
  if(pBag && LocalN("m_val", pBag)<=25)
    {
    MessageEx("$Dlg_Cowboy10$", pPlayer);
    }
  else
    {
    MessageEx("$Dlg_Cowboy10b$", pPlayer);
    StopDialog();
    }
  }

private func DlgCowboy11()
  {
  var pBag = FindContents(MBAG, pPlayer);
  if(LocalN("m_val", pBag)<25)
    {
    MessageEx("$Dlg_Cowboy11b$", pNPC);
    return(StopDialog());
    }
  RemoveObject(pBag);
  MessageEx("$Dlg_Cowboy11$", pNPC);
  pNPC->SetObjectLayer();
  SetOwner(GetOwner(pPlayer), pNPC);
  MakeCrewMember(pNPC, GetOwner(pPlayer));
  AddEffect("Life", pNPC, 1, 35, pNPC);
  StopDialog();
  RemoveObject();
  }
