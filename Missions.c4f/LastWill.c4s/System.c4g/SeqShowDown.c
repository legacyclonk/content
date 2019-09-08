
/* Showdown-Sequenz */

#strict 2

#appendto SQ69

protected func SeqShowDownStart()
  {
  RemoveObject(FindObject(0,0,0,0,0,0,"SeaSound"));
  fShowdown = 1;
  // Shift view to the active clonk
  SeqSetView(pStoryClonk);
  }

protected func SeqShowDown10()
  {
  SetDir(DIR_Right,pStoryClonk);
  var pOldFarmer = pFarmer;
  pFarmer = CreateObject(_LEI,3680,176,-1);
  ObjectSetAction(pFarmer,"Ghost");
  ObjectLine(SPRK,40,GetX(pFarmer),GetY(pFarmer),Abs(ObjectDistance(pFarmer,pOldFarmer)),Angle(GetX(pFarmer),GetY(pFarmer),GetX(pOldFarmer),GetY(pOldFarmer))-90,-1 );
  RemoveObject(pOldFarmer);
  }

protected func SeqShowDown15()
  {
  SeqMsg("$MsgDia4_1$",pStoryClonk);
  }

protected func SeqShowDown25()
  {
  SeqMsg("$MsgDia4_2$",pFarmer);
  }

protected func SeqShowDown33()
  {
  SeqMsg("$MsgDia4_3$",pStoryClonk);
  }

protected func SeqShowDown45()
  {
  SeqMsg("$MsgDia4_4$",pFarmer);
  }

protected func SeqShowDown59()
  {
  SeqMsg("$MsgDia4_5$",pStoryClonk);
  }

protected func SeqShowDown70()
  {
  SeqMsg("$MsgDia4_6$",pFarmer);
  }

protected func SeqShowDown90()
  {
  SeqMsg("$MsgDia4_7$",pStoryClonk);
  }

protected func SeqShowDown100()
  {
  SeqMsg("$MsgDia4_8$",pFarmer);
  }

protected func SeqShowDown115()
  {
  SeqMsg("$MsgDia4_9$",pStoryClonk);
  }

protected func SeqShowDown122()
  {
  SeqMsg("$MsgDia4_10$",pFarmer);
  }

protected func SeqShowDown160()
  {
  ObjectSetAction(pFarmer,"GhostMove");
  SetComDir(COMD_Stop,pStoryClonk);
  SetDir(DIR_Right,pStoryClonk);
  }

protected func SeqShowDown162()
  {
  SeqMsg("$MsgDia4_11$",pStoryClonk);
  }

protected func SeqShowDown200()
  {
  Log("$MsgEnd$");
  FindObject(SCRG)->Fulfill();
  }