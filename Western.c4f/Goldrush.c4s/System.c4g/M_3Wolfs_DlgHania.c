/* Dialog: Hania */

#strict
#appendto _TLK

static g_fHaniaSpoken;

public func DlgHaniaStart()
  {
  if(GetID(pPlayer)==INDI) 
  {
    MessageEx("$Dlg_Hania0b$", pNPC);
    StopDialog();
    return();
  }
  if(GetID(pPlayer)==TRPR) 
  {
    MessageEx("$Dlg_Hania0c$", pNPC);
    StopDialog();
    return();
  }
  if(g_fHaniaSpoken==1) return(DlgHania7(), iTalkIndex=7);
  if(g_fHaniaSpoken==2) return(DlgHania14(), iTalkIndex=14);
  MessageEx("$Dlg_Hania0$", pNPC);
  return(1);
  }

private func DlgHania1()
  {
  MessageEx("$Dlg_Hania1$", pPlayer);
  }

private func DlgHania2()
  {
  MessageEx("$Dlg_Hania2$", pNPC);
  }

private func DlgHania3()
  {
  MessageEx("$Dlg_Hania3$", pPlayer);
  }

private func DlgHania4()
  {
  MessageEx("$Dlg_Hania4$", pNPC);
  }

private func DlgHania5()
  {
  MessageEx("$Dlg_Hania5$", pNPC);
  }

private func DlgHania6()
  {
  MessageEx("$Dlg_Hania6$", pPlayer);
  g_fHaniaSpoken=1;
  StopDialog();
  }

private func DlgHania7()
  {
  MessageEx("$Dlg_Hania7$", pNPC);
  }

private func DlgHania8()
  {
  var pCrystal = FindContents(CRYS, pPlayer);	  
  if(pCrystal)
  {
    MessageEx("$Dlg_Hania8$", pPlayer);
    RemoveObject(pCrystal);
  }
  else
  {
    MessageEx("$Dlg_Hania8b$", pPlayer);
    StopDialog();	  
  }
  }

private func DlgHania9()
  {
  MessageEx("$Dlg_Hania9$", pNPC);
  }

private func DlgHania10()
  {
  MessageEx("$Dlg_Hania10$", pNPC);
  AddEffect("Feather", pPlayer, 200, 0, 0, GetID(), GetColorDw(pNPC));
  }

public func FxFeatherStart(object pTarget, int iEffectNumber, int iTemp, int iColor)
{
  if(iTemp) return(0);
  var iOverlay = GetUnusedOverlayID(10,pTarget);
  SetGraphics(0,pTarget,_MFR,iOverlay,6);
  SetClrModulation(iColor,pTarget,iOverlay);
  return(1);
}

// Die Feder schütz einbisschen
public func FxFeatherDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(iDmgEngy>0) return(iDmgEngy);
  return(iDmgEngy*2/3);
}

private func DlgHania11()
  {
  MessageEx("$Dlg_Hania11$", pPlayer);
  }

private func DlgHania12()
  {
  MessageEx("$Dlg_Hania12$", pNPC);
  }

private func DlgHania13()
  {
  MessageEx("$Dlg_Hania13$", pPlayer);
  g_fHaniaSpoken=2;
  StopDialog();
  }

private func DlgHania14()
  {
  MessageEx("$Dlg_Hania14$", pNPC);
  StopDialog();
  }
