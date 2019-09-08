/* Dialog: Captain */

#strict
#appendto _TLK

static g_fCaptainSpoken;

public func DlgCaptainStart()
  {
  if (g_fCaptainSpoken==1) return(DlgCaptain8(), iTalkIndex=8);
  if (g_fCaptainSpoken==2) return(DlgCaptain13(), iTalkIndex=13);
  MessageEx("$Dlg_Captain0$", pPlayer);
  return(1);
  }

private func DlgCaptain1()
  {
  MessageEx("$Dlg_Captain1$", pNPC);
  }

private func DlgCaptain2()
  {
  MessageEx("$Dlg_Captain2$", pNPC);
  }

private func DlgCaptain3()
  {
  MessageEx("$Dlg_Captain3$", pNPC);
  }

private func DlgCaptain4()
  {
  MessageEx("$Dlg_Captain4$", pPlayer);
  }

private func DlgCaptain5()
  {
  MessageEx("$Dlg_Captain5$", pNPC);
  return(1);
  }

private func DlgCaptain6()
  {
  MessageEx("$Dlg_Captain6$", pPlayer);
  }
  
private func DlgCaptain7()
  {
  MessageEx("$Dlg_Captain7$", pNPC);
  g_fCaptainSpoken=1;
  }

private func DlgCaptain8()
  {
  MessageEx("$Dlg_Captain8$", pNPC);
  }

private func DlgCaptain9()
  {
  var pBag = FindContents(MBAG, pPlayer);	  
  if(pBag)// && LocalN("m_val", pBag)>=50)
    {
    MessageEx("$Dlg_Captain9$", pPlayer);
    }
  else
    {
    MessageEx("$Dlg_Captain9b$", pPlayer);
    StopDialog();
    }
  }

private func DlgCaptain10()
  {
  var pBag = FindContents(MBAG, pPlayer);
  if(LocalN("m_val", pBag)<50)
    {
    MessageEx("$Dlg_Captain10b$", pPlayer);
    return(StopDialog());
    }
  RemoveObject(pBag);
  MessageEx("$Dlg_Captain10$", pNPC);
  var pCaval = FindCrewByName(CVRM, "Caval");
  SetCommand(pCaval, "MoveTo", 0, GetX(pCaval), GetY(pCaval));
  AddCommand(pCaval, "Call", LocalN("pGrab",FindObject(CTWR,1380-GetX(), 267-GetY(),-1,-1)), 0, 0, 0, 0, "ControlRight");
  AddCommand(pCaval, "MoveTo", 0, 1377, 305);
//  AddCommand(pCaval, "Grab", LocalN("pGrab", FindObject(CTWR,1380-GetX(), 267-GetY(),-1,-1)));
  AddCommand(pCaval, "Call", LocalN("pGrab", FindObject(CTWR,1174-GetX(), 268-GetY(),-1,-1)), 0, 0, 0, 0, "ControlLeft");
  AddCommand(pCaval, "MoveTo", 0, 1173, 305);
//  AddCommand(pCaval, "Grab", LocalN("pGrab", FindObject(CTWR,1174-GetX(), 268-GetY(),-1,-1)));
  }

private func DlgCaptain11()
  {
  MessageEx("$Dlg_Captain11$", pNPC);
  SetPlrKnowledge(GetOwner(pPlayer), AMRY);
  }

private func DlgCaptain12()
  {
  g_fCaptainSpoken=2;
  MessageEx("$Dlg_Captain12$", pPlayer);
  StopDialog();
  }

private func DlgCaptain13()
  {
  MessageEx("$Dlg_Captain13$", pNPC);
  StopDialog();
  }

