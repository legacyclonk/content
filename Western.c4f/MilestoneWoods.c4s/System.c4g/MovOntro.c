/* Dialog: Ontro */

#strict
#appendto _TLK

private func MovOntro0()
  {
  MessageEx("$Dlg_Trapper1$", pPlayer);
  iWait=262;
  }

private func MovOntro1()
  {
  SetPlrKnowledge(0, SALO);	  
  StopDialog();
  }
