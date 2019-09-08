/* Suicide-Sequenz */

#strict 2

#appendto SQ69

protected func SeqSuicideStart()
  {
  pMage = FindObject(MCLK); 
  SetDir(DIR_Right, pStoryClonk);
  // Shift view to the active clonk
  SeqSetView(pStoryClonk);
  }

protected func SeqSuicide2()
  {
  SeqMsg("$MsgDia3_1$",pStoryClonk);
  }

protected func SeqSuicide12()
  {
  SeqMsg("$MsgDia3_2$",pMage);
  }

protected func SeqSuicide22()
  {
  SeqMsg("$MsgDia3_3$",pStoryClonk);
  }

protected func SeqSuicide40()
  {
  SetDir(DIR_Left,pMage);
  SeqMsg("$MsgDia3_4$",pMage);
  }

protected func SeqSuicide70()
  {
  SeqMsg("$MsgDia3_5$",pStoryClonk);
  }

protected func SeqSuicide82()
  {
  SeqMsg("$MsgDia3_6$",pMage);
  }

protected func SeqSuicide92()
  {
  SeqMsg("$MsgDia3_7$",pStoryClonk);
  }

protected func SeqSuicide100()
  {
  SeqMsg("$MsgDia3_8$",pMage);
  }

protected func SeqSuicide110()
  {
  SeqMsg("$MsgDia3_9$",pStoryClonk);
  }

protected func SeqSuicide120()
  {
  SeqMsg("$MsgDia3_10$",pMage);
  }

protected func SeqSuicide140()
  {
  SeqMsg("$MsgDia3_11$",pStoryClonk);
  }

protected func SeqSuicide150()
  {
  SeqMsg("$MsgDia3_12$",pMage);
  }

protected func SeqSuicide160()
  {
  SetDir(DIR_Right,pMage);
  }

protected func SeqSuicide163()
  {
  SeqMsg("$MsgDia3_13$",pStoryClonk);
  }

protected func SeqSuicide165()
  {
  SetCommand(pMage,"Jump");
  }

protected func SeqSuicide169()
  {
  Sound("Scream");
  SetComDir(COMD_Stop,pStoryClonk);
  }

protected func SeqSuicide175()
  {
  Kill(pMage);
  }

protected func SeqSuicide180()
  {
  SeqMsg("$MsgDia3_14$",pStoryClonk);
  RemoveObject(pMage);
  }

protected func SeqSuicide190()
  {
  NewGoal("$MsgGoal5$");
  SeqStop(true);
  ScriptGo(1);
  }

protected func SeqSuicideAllowSkip() { return "$MsgSkipSeq$"; }

protected func SeqSuicideSkip()
  {
  // End SeqMsg
  NewGoal("$MsgGoal5$");
  // Remove Suicide
  RemoveObject(FindObject(MCLK));
  // Make sure they aren't moving any more
  SeqStopClonks();
  // End the sequence
  SeqStop(true);
  ScriptGo(true);
  }