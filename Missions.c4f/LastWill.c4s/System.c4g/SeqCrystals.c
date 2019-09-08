/* Crystal-Sequenz */

#strict 2

#appendto SQ69

protected func SeqCrystalStart()
  {
  pMage = CreateObject(SCLK,GetX(FindObject(CST3)),0,-1);
  Fling(pMage,0,200);
  SetDir(DIR_Left,pStoryClonk);
  var cnt = 30;
  while(cnt--) RemoveObject(FindContents(CRYS,FindObject(CST3)));
  // Shift view to the active clonk
  SeqSetView(pStoryClonk);
  }

protected func SeqCrystal1()
  {
  SetDir(DIR_Right,pMage);
  ObjectJump(SPRK,60,GetX(pMage),GetY(pMage),90,-10,21,-1);
  Sound("Magic1");
  }

protected func SeqCrystal5()
  {
  SeqMsg("$MsgDia2_1$", pMage);
  }

protected func SeqCrystal17()
  {
  ObjectSetAction(pMage,"Magic");
  ObjectJump(SPRK,60,GetX(pMage),GetY(pMage),90,-10,21,-1);
  Sound("Magic1");
  LaunchLightning(GetX(pMage),GetY(pMage),-20,2,-10,2);
  LaunchLightning(GetX(pMage),GetY(pMage),0,2,-10,2);
  LaunchLightning(GetX(pMage),GetY(pMage),20,2,-10,2);
  DoEnergy(+50,pMage);
  }

protected func SeqCrystal18()
  {
  LaunchLightning(GetX(pMage),GetY(pMage),-15,2,-10,2);
  LaunchLightning(GetX(pMage),GetY(pMage),0,2,-10,2);
  LaunchLightning(GetX(pMage),GetY(pMage),15,2,-10,2);
  DoEnergy(+50,pMage);
  }

protected func SeqCrystal19()
  {
  ObjectSetAction(pMage,"Magic");
  LaunchLightning(GetX(pMage),GetY(pMage),-10,2,-10,2);
  LaunchLightning(GetX(pMage),GetY(pMage),0,2,-10,2);
  LaunchLightning(GetX(pMage),GetY(pMage),10,2,-10,2);
  DoEnergy(+50,pMage);
  }

protected func SeqCrystal20()
  {
  LaunchLightning(GetX(pMage),GetY(pMage),-5,2,-10,2);
  LaunchLightning(GetX(pMage),GetY(pMage),0,2,-10,2);
  LaunchLightning(GetX(pMage),GetY(pMage),5,2,-10,2);
  DoEnergy(+50,pMage);
  }

protected func SeqCrystal25()
  {
  SeqMsg("$MsgDia2_2$", pMage);
  }

protected func SeqCrystal42()
  {
  SeqMsg("$MsgDia2_3$",pStoryClonk);
  }

protected func SeqCrystal52()
  {
  SeqMsg("$MsgDia2_4$",pMage);
  }

protected func SeqCrystal64()
  {
  Sound("Magic1");
  for(var i = 0; i < GetPlayerCount(); i++)
    SetPlrKnowledge(GetPlayerByIndex(i),BALN);
  ObjectLine(SPRK,20,GetX(pMage),GetY(pMage),Abs(ObjectDistance(pMage,pStoryClonk)),Angle(GetX(pMage),GetY(pMage),GetX(pStoryClonk),GetY(pStoryClonk)) - 90,-1 );
  }

protected func SeqCrystal67()
  {
  SeqMsg("$MsgDia2_5$",pMage);
  }

protected func SeqCrystal85()
  {
  ObjectSetAction(CreateObject(FXB1,GetX(pMage),GetY(pMage)),"Blast");
  var cnt = 11;
  while(cnt--) Smoke(Random(31)-15+GetX(pMage),Random(31)-15+GetY(pMage),Random(41));
  LaunchLightning(GetX(pMage),GetY(pMage),-40,81,-20,11);
  LaunchLightning(GetX(pMage),GetY(pMage),-20,41,-30,11);
  LaunchLightning(GetX(pMage),GetY(pMage),-10,21,-40,11);
  ObjectJump(SPRK,60,GetX(pMage),GetY(pMage),90,-10,21,-1);
  Sound("Magic1");
  RemoveObject(pMage);

  SetComDir(COMD_Stop,pStoryClonk);
  SetDir(DIR_Left,pStoryClonk);
  }

protected func SeqCrystal92()
  {
  NewGoal("$MsgGoal3$");
  SeqStop(true);
  ScriptGo(true);
  }

protected func SeqCrystalAllowSkip() { return "$MsgSkipSeq$"; }

protected func SeqCrystalSkip()
  {
  // End Message
  NewGoal("$MsgGoal3$");
  // Remove Mage
  RemoveObject(pMage);
  // Give the balloon plan
  for(var i = 0; i < GetPlayerCount(); i++)
    SetPlrKnowledge(GetPlayerByIndex(i),BALN);
  // Make sure they aren't moving any more
  SeqStopClonks();
  // End the sequence
  SeqStop(true);
  ScriptGo(true);
  }