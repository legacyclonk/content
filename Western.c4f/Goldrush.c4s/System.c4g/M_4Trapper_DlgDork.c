/* Dialog: Dork */

#strict
#appendto _TLK

static g_fDorkSpoken;

public func DlgDorkStart()
  {
  if(!GetEffect("IntNoFood",this())) if(g_fDorkSpoken==1) return(DlgDork2(), iTalkIndex=2);
  MessageEx("$Dlg_Dork0$", pPlayer);
  return(1);
  }

private func DlgDork1()
  {
  MessageEx("$Dlg_Dork1$", pNPC);
  if(GetEffect("IntNoFood",this())) StopDialog();
  }

private func DlgDork2()
  {
  g_fDorkSpoken=1;
  MessageEx("$Dlg_Dork2$", pNPC);
  }

private func DlgDork3()
  {
  if(FindObject(NDFO))
  {
    if(LocalN("iHunger", pPlayer)<=pPlayer->MaximumHunger()/3)
    {
      MessageEx("$Dlg_Dork3c$", pPlayer);
      StopDialog();
      return();
    }
    if(LocalN("iHunger", pPlayer)<=pPlayer->MaximumHunger()-pPlayer->MaximumHunger()/3)
      MessageEx("$Dlg_Dork3b$", pPlayer);
    else MessageEx("$Dlg_Dork3$", pPlayer);
    return();
  }
  if(GetEnergy(pPlayer)==GetPhysical("Energy", 0, pPlayer)/1000)
  {
    MessageEx("$Dlg_Dork3c$", pPlayer);
    StopDialog();
    return();
  }
  if(GetEnergy(pPlayer)>GetPhysical("Energy", 0, pPlayer)/2000)
    MessageEx("$Dlg_Dork3b$", pPlayer);
  else MessageEx("$Dlg_Dork3$", pPlayer);
  }

private func DlgDork4()
  {
  MessageEx("$Dlg_Dork4$", pNPC);
  CreateContents(RMET, pPlayer);
  AddEffect("IntNoFood",this(),1,10500,this());
  }

private func DlgDork5()
  {
  MessageEx("$Dlg_Dork5$", pPlayer);
  StopDialog();
  }
