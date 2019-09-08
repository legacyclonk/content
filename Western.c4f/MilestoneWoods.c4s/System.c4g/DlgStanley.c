/* Dialog: Stanley */

#strict
#appendto _TLK

static g_fStanleySpoken;

public func DlgStanleyStart()
  {
  if (g_fStanleySpoken==1) return(DlgStanley6(), iTalkIndex=6);
  if (g_fStanleySpoken==2) return(DlgStanley9(), iTalkIndex=9);
  return(1);
  }

private func DlgStanley1()
  {
  MessageEx("$Dlg_Stanley1$", pPlayer);
  }

private func DlgStanley2()
  {
  MessageEx("$Dlg_Stanley2$", pNPC);
  }

private func DlgStanley3()
  {
  MessageEx("$Dlg_Stanley3$", pPlayer);
  }

private func DlgStanley4()
  {
  MessageEx("$Dlg_Stanley4$", pNPC);
  }

private func DlgStanley5()
  {
  MessageEx("$Dlg_Stanley5$", pPlayer);
  g_fStanleySpoken=1;
  }

private func DlgStanley6()
  {
  MessageEx("$Dlg_Stanley6$", pNPC);
  }
  
private func DlgStanley7()
  {
  var pBag = FindContents(MBAG, pPlayer);	  
  if(pBag && LocalN("m_val", pBag)>=25)
    {
    MessageEx("$Dlg_Stanley7$", pPlayer);
    RemoveObject(pBag);
    }
  else
    {
    MessageEx("$Dlg_Stanley7b$", pPlayer);
    StopDialog();
    }
  }

private func DlgStanley8()
  {
  MessageEx("$Dlg_Stanley8$", pNPC);
  SetPlrKnowledge(GetOwner(pPlayer), ELEV);
  SetPlrKnowledge(GetOwner(pPlayer), WMLW);
  SetPlrKnowledge(GetOwner(pPlayer), DYNM);
  g_fStanleySpoken=2;
  StopDialog();
  }

private func DlgStanley9()
  {
  MessageEx("$Dlg_Stanley9$", pNPC);
  StopDialog();
  }
