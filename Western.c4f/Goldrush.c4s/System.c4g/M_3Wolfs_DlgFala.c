/* Dialog: Fala */

#strict
#appendto _TLK

static g_fFalaSpoken;

public func DlgFalaStart()
  {
  if (g_fFalaSpoken==1) return(DlgFala11(), iTalkIndex=11);
  if (g_fFalaSpoken==2) return(DlgFala13(), iTalkIndex=13);
  MessageEx("$Dlg_Fala0$", pPlayer);
  return(1);
  }

private func DlgFala1()
  {
  MessageEx("$Dlg_Fala1$", pNPC);
  }

private func DlgFala2()
  {
  MessageEx("$Dlg_Fala2$", pPlayer);
  }

private func DlgFala3()
  {
  MessageEx("$Dlg_Fala3$", pNPC);
  }

private func DlgFala4()
  {
  MessageEx("$Dlg_Fala4$", pPlayer);
  }

private func DlgFala5()
  {
  MessageEx("$Dlg_Fala5$", pNPC);
  }

private func DlgFala6()
  {
  MessageEx("$Dlg_Fala6$", pPlayer);
  }

private func DlgFala7()
  {
  MessageEx("$Dlg_Fala7$", pNPC);
  }

private func DlgFala8()
  {
  MessageEx("$Dlg_Fala8$", pPlayer);
  }

private func DlgFala9()
  {
  MessageEx("$Dlg_Fala9$", pNPC);
  }

private func DlgFala10()
  {
  MessageEx("$Dlg_Fala10$", pPlayer);
  g_fFalaSpoken=1;
  }

private func DlgFala11()
  {
  if(ContentsCount(GOLD, FindObject(_GCS))>29 && FindObject(COAC, -200, -200, 300, 400))
    {
    MessageEx("$Dlg_Fala11$", pNPC);
    var pChest = FindObject(_GCS);
    pChest->SetAction("Closed");
    LocalN("fLocked", pChest) = 1;
    // Dem dummen Delsy werden einige Commandos erteilt
    var pDelsy = FindCrewByName(INDI, "Delsy");
    SetCommand(pDelsy, "MoveTo", 0, 3637, 278);
    AddCommand(pDelsy, "Wait", 0, 5);
    AddCommand(pDelsy, "Enter", pDelsy->FindObject(MTIP, 0, 0, -1, -1));
    AddCommand(pDelsy, "Call", pChest, 0, 0, 0, 0, "Empty");
    AddCommand(pDelsy, "Wait", 0, 2);
    AddCommand(pDelsy, "Grab", pChest);
    AddCommand(pDelsy, "Call", LocalN("pGrab",pNPC->FindObject(WGTW, 0, 0, -1,-1)), 0, 0, 0, 0, "ControlLeft");
    AddCommand(pDelsy, "Grab", LocalN("pGrab",pNPC->FindObject(WGTW, 0, 0, -1,-1)));
    }
  else
    {
    var szString = "";
    if(!FindObject(COAC, -200, -200, 300, 400)) szString = "$Dlg_Fala11b$";
    if(ContentsCount(GOLD, FindObject(_GCS))<30) szString = Format("%s$Dlg_Fala11c$", szString, 30-ContentsCount(GOLD, FindObject(_GCS)));
    MessageEx(szString, pNPC);
    StopDialog();
    }
  }

private func DlgFala12()
  {
  MessageEx("$Dlg_Fala12$", pPlayer);
  g_fFalaSpoken=2;
  StopDialog();
  }

private func DlgFala13()
  {
  MessageEx("$Dlg_Fala13$", pNPC);
  }

private func DlgFala14()
  {
  MessageEx("$Dlg_Fala14$", pPlayer);
  StopDialog();
  }
