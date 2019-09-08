/* Dialog: Chaska */

#strict
#appendto _TLK

static g_fChaskaSpoken;

public func DlgChaskaStart()
  {
  if (g_fChaskaSpoken==1) return(DlgChaska6(), iTalkIndex=6);
  if (g_fChaskaSpoken==2) return(DlgChaska13(), iTalkIndex=13);
  MessageEx("$Dlg_Chaska0$", pPlayer);
  return(1);
  }

private func DlgChaska1()
  {
  MessageEx("$Dlg_Chaska1$", pNPC);
  }

private func DlgChaska2()
  {
  MessageEx("$Dlg_Chaska2$", pPlayer);
  }

private func DlgChaska3()
  {
  MessageEx("$Dlg_Chaska3$", pNPC);
  }

private func DlgChaska4()
  {
  MessageEx("$Dlg_Chaska4$", pNPC);
  }

private func DlgChaska5()
  {
  MessageEx("$Dlg_Chaska5$", pPlayer);
  g_fChaskaSpoken=1;
  StopDialog();
  }

private func DlgChaska6()
  {
  MessageEx("$Dlg_Chaska6$", pNPC);
  }

private func DlgChaska7()
  {
  if(ChaskaFindMeat())
  {
    MessageEx("$Dlg_Chaska7$", pPlayer);
  }
  else
  {
    MessageEx("$Dlg_Chaska7b$", pPlayer);
    StopDialog();
  }
  }

private func ChaskaFindMeat()
{
  var i, obj;
  while(obj = Contents(i++, pPlayer)) if(obj->~GetFeedValue()) return(1);
}

private func DlgChaska8()
  {
  if(FindContents(RMET, pPlayer))
  {
    MessageEx("$Dlg_Chaska8b$", pNPC);
    return(StopDialog());
  }
  var pMeat = FindContents(MEAC, pPlayer);
  if(!pMeat)
  {
    MessageEx("$Dlg_Chaska8c$", pNPC);
    return(StopDialog());
  }
  RemoveObject(pMeat);
  MessageEx("$Dlg_Chaska8$", pNPC);
  }

private func DlgChaska9()
  {
  MessageEx("$Dlg_Chaska9$", pPlayer);
  }

private func DlgChaska10()
  {
  MessageEx("$Dlg_Chaska10$", pNPC);
  var pBowl = CreateObject(BOWL);
  for(var i = 9; i; i--)
  { 
    AddPotionAlchem(pBowl, GetRndAlchem(), 1);
  }
  pBowl->~PotionFilled(this());
  Enter(pPlayer, pBowl);
  }

private func DlgChaska11()
  {
  MessageEx("$Dlg_Chaska11$", pPlayer);
  }

private func DlgChaska12()
  {
  MessageEx("$Dlg_Chaska12$", pNPC);
  g_fChaskaSpoken=2;
  StopDialog();
  }

private func DlgChaska13()
  {
  MessageEx("$Dlg_Chaska13$", pNPC);
  }

private func DlgChaska14()
  {
  MessageEx("$Dlg_Chaska14$", pNPC);
  }

private func DlgChaska15()
  {
  if(ChaskaFindMeat())
  {
    MessageEx("$Dlg_Chaska15$", pPlayer);
  }
  else
  {
    MessageEx("$Dlg_Chaska15b$", pPlayer);
    StopDialog();
  }
  }

private func DlgChaska16()
  {
  if(FindContents(RMET, pPlayer))
  {
    MessageEx("$Dlg_Chaska8b$", pNPC);
    return(StopDialog());
  }
  var pMeat = FindContents(MEAC, pPlayer);
  if(!pMeat)
  {
    MessageEx("$Dlg_Chaska8c$", pNPC);
    return(StopDialog());
  }
  RemoveObject(pMeat);
  MessageEx("$Dlg_Chaska16$", pNPC);
  var pBowl = CreateObject(BOWL);
  for(var i = 9; i; i--)
  { 
    AddPotionAlchem(pBowl, GetRndAlchem(), 1);
  }
  pBowl->~PotionFilled(this());
  Enter(pPlayer, pBowl);
  StopDialog();
  }

func GetRndAlchem()
{
  var iIndex = Random(6);
  if(!iIndex--) return(IBLO);
  if(!iIndex--) return(IBON);
  if(!iIndex--) return(IHAI);
  if(!iIndex--) return(ILOA);
  if(!iIndex--) return(IPOI);
  else return(IROC);
}
