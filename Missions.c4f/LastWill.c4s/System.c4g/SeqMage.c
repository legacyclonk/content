
/* Magus-Sequenz */

#strict 2

#appendto SQ69

private func BeamClonks(iOwner, iToX, iToY)
{
  var pClonk;
  while(pClonk = FindObject(CLNK,0,0,0,0,0,0,0,0,pClonk)) if(GetOwner(pClonk) == iOwner) SetPosition(iToX += 7, iToY, pClonk);
  return(1);
}
private func CreateStartStuff(iPlr)
{
  SetWealth(iPlr,Random(11));
  CreateContents(ROCK,GetCrew(iPlr,0));
  CreateContents(FLAG,GetCrew(iPlr,1));
  CreateContents(WOOD,GetCrew(iPlr,2));
  return(1);
}
protected func ObjectJump(idID, cnt, iX, iY, iStrength, iPosOffs, iPosRand, iOwner)  
{
  while(cnt--) SetYDir(Random(iStrength) -iStrength, CreateObject(idID, iX + iPosOffs + Random(iPosRand)-GetX(), iY + iPosOffs + Random(iPosRand)-GetY(), iOwner));
  return(1);
}
protected func ObjectLine(idID, cnt, iX, iY, iMaxLength, iAngle, iOwner)
{                                                
  var iLength;
  while(cnt--) CreateObject(idID, iX + Cos(iAngle, iLength = Random(iMaxLength))-GetX(), iY + Sin(iAngle, iLength)-GetY(), iOwner);
  return(1);
}


protected func SeqMageStart()
  {
  pMage = CreateObject(SCLK,GetX(FindObject(TWR2)),0,-1);
  SetDir(DIR_Left, pMage); 
  SetDir(DIR_Right,pStoryClonk);
  SetComDir(COMD_Stop, pStoryClonk);
  // Shift view to the active clonk
  SeqSetView(pStoryClonk);
  }

protected func SeqMage1()
  {
  ObjectJump(SPRK,60,GetX(pMage),GetY(pMage),90,-10,21,-1);
  Sound("Magic1");
  }

protected func SeqMage5()
  {
  SeqMsg("$MsgDia1_1$",pMage);
  }

protected func SeqMage9()
  {
  Sound("Thunder2");
  ObjectJump(SPRK,20,GetX(pStoryClonk),GetY(pStoryClonk),50,-20,41,-1);
  ObjectLine(SPRK,20,GetX(pMage),GetY(pMage),Abs(ObjectDistance(pMage,pStoryClonk)),Angle(GetX(pMage),GetY(pMage),GetX(pStoryClonk),GetY(pStoryClonk))-90,-1 );
  Fling(pStoryClonk,0,-3);
  }

protected func SeqMage15()
  {
  SetDir(DIR_Right,pStoryClonk);
  SeqMsg("$MsgDia1_2$",pStoryClonk);
  }

protected func SeqMage16()
  {
  SetCommand(pMage,"MoveTo",0,GetX(pMage)-15,GetY(pMage));
  }

protected func SeqMage29()
  {
  SeqMsg("$MsgDia1_3$",pMage);
  }

protected func SeqMage36()
  {
  SeqMsg("$MsgDia1_4$",pStoryClonk);
  }

protected func SeqMage47()
  {
  SeqMsg("$MsgDia1_5$",pMage);
  }

protected func SeqMage60()
  {
  SetCommand(pMage,"MoveTo",0,Sum(GetX(pMage),30),GetY(pMage));
  }

protected func SeqMage62()
  {
  SeqMsg("$MsgDia1_6$",pStoryClonk);
  }

protected func SeqMage77()
  {
  SetCommand(pMage,"MoveTo",0,GetX(pMage)-30,GetY(pMage));
  }

protected func SeqMage80()
  {
  SeqMsg("$MsgDia1_7$",pMage);
  }

protected func SeqMage87()
  {
  SeqMsg("$MsgDia1_8$",pStoryClonk);
  }

protected func SeqMage97()
  {
  SeqMsg("$MsgDia1_9$",pMage);
  }

protected func SeqMage109()
  {
  SeqMsg("$MsgDia1_10$",pStoryClonk);
  }

protected func SeqMage119()
  {
  LaunchLightning(GetX(pMage),GetY(pMage),-40,81,-20,11);
  LaunchLightning(GetX(pMage),GetY(pMage),-20,41,-30,11);
  LaunchLightning(GetX(pMage),GetY(pMage),-10,21,-40,11);
  RemoveObject(pMage);
  }

protected func SeqMage122()
  {
  pMage = CreateObject(SCLK,GetX(FindObject(TWR2))-35,0,-1);
  SetDir(DIR_Left,pMage);
  Fling(pStoryClonk,0,200);
  }

protected func SeqMage126()
  {
  ObjectJump(SPRK,20,GetX(pMage),GetY(pMage),50,-10,21,-1);
  }

protected func SeqMage127()
  {
  SeqMsg("$MsgDia1_11$",pMage);
  }

protected func SeqMage140()
  {
  SeqMsg("$MsgDia1_12$",pStoryClonk);
  }

protected func SeqMage148()
  {
  SeqMsg("$MsgDia1_13$",pMage);
  }

protected func SeqMage180()
  {
  SeqMsg("$MsgDia1_14$",pMage);
  }

protected func SeqMage216()
  {
  Fling(pMage,0,-4);
  SetComDir(COMD_Stop,pStoryClonk);
  SetDir(DIR_Right,pStoryClonk);
  }

protected func SeqMage217()
{
  ObjectJump(SPRK,60,GetX(pMage),GetY(pMage),90,-10,21,-1);
  Sound("Magic1");
  LaunchLightning(GetX(pMage),GetY(pMage),-40,81,-20,11);
  LaunchLightning(GetX(pMage),GetY(pMage),-20,41,-30,11);
  LaunchLightning(GetX(pMage),GetY(pMage),-10,21,-40,11);
  RemoveObject(pMage);
}

protected func SeqMage222()
  {
  FindObject(TWR2)->ControlLeft();
  ObjectJump(SPRK,20,GetX(FindObject(GATL)),GetY(FindObject(GATL)),30,-20,41,-1);
  }

protected func SeqMage231()
  {
  NewGoal("$MsgGoal2$");
  RemoveObject(FindObject(GATL));RemoveObject(FindObject(GATR));
  SeqStop(true);
  ScriptGo(1);
  }

protected func SeqMageAllowSkip() { return "$MsgSkipSeq$"; }

protected func SeqMageSkip()
  {
  // End Message
  NewGoal("$MsgGoal2$");
  // Open Guardpower
  RemoveObject(FindObject(GATL));RemoveObject(FindObject(GATR));
  // Remove Mageclonk
  RemoveObject(FindObject(SCLK));
  // Make sure they aren't moving any more
  SeqStopClonks();
  // End the sequence
  SeqStop(true);
  ScriptGo(true);
  }