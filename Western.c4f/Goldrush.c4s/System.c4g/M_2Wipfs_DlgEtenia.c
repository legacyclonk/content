/* Dialog: Etenia */

#strict
#appendto _TLK

static g_fEteniaSpoken;

public func DlgEteniaStart()
  {
  if (FindContents(CALU, pPlayer)) return(DlgEtenia5(), iTalkIndex=5);
  if (g_fEteniaSpoken==1) return(DlgEtenia3(), iTalkIndex=3);
  if (g_fEteniaSpoken==2) return(DlgEtenia6(), iTalkIndex=6);
  MessageEx("$Dlg_Etenia0$", pPlayer);
  return(1);
  }

private func DlgEtenia1()
  {
  MessageEx("$Dlg_Etenia1$", pNPC);
  }

private func DlgEtenia2()
  {
  MessageEx("$Dlg_Etenia2$", pPlayer);
  g_fEteniaSpoken=1;
  }

private func DlgEtenia3()
  {
  MessageEx("$Dlg_Etenia3$", pNPC);
  }

private func DlgEtenia4()
  {
  MessageEx("$Dlg_Etenia4$", pPlayer);
  StopDialog();
  }

private func DlgEtenia5()
  {
  var pCalumet = FindContents(CALU, pPlayer);
  if(pCalumet) RemoveObject(pCalumet);
  MessageEx("$Dlg_Etenia5$", pNPC);
  Local(2, FindObject(NTIP, 0, 0, -1,-1)) = 1;
  SetPlrKnowledge(GetOwner(pPlayer), AMBR);
  SetPlrKnowledge(GetOwner(pPlayer), AMBI);
  SetPlrKnowledge(GetOwner(pPlayer), AMFH);
  SetPlrKnowledge(GetOwner(pPlayer), AMSN);
  SetPlrKnowledge(GetOwner(pPlayer), AMWI);
  g_fEteniaSpoken=2;
  }

private func DlgEtenia6()
  {
  MessageEx("$Dlg_Etenia6$", pNPC);
  StopDialog();
  }
