/* Dialog: Gaho */

#strict
#appendto _TLK

static g_fGahoSpoken;

public func DlgGahoStart()
  {
  if (g_fGahoSpoken==1) return(DlgGaho8(), iTalkIndex=8);
  if (g_fGahoSpoken==2) return(DlgGaho9(), iTalkIndex=9);
  if (g_fGahoSpoken==3) return(DlgGaho17(), iTalkIndex=17);
  MessageEx("$Dlg_Gaho0$", pPlayer);
  return(1);
  }

private func DlgGaho1()
  {
  MessageEx("$Dlg_Gaho1$", pPlayer);
  }

private func DlgGaho2()
  {
  MessageEx("$Dlg_Gaho2$", pNPC);
  }

private func DlgGaho3()
  {
  MessageEx("$Dlg_Gaho3$", pNPC);
  }

private func DlgGaho4()
  {
  MessageEx("$Dlg_Gaho4$", pNPC);
  }

private func DlgGaho5()
  {
  MessageEx("$Dlg_Gaho5$", pPlayer);
  }

private func DlgGaho6()
  {
  MessageEx("$Dlg_Gaho6$", pNPC);
  }

private func DlgGaho7()
  {
  MessageEx("$Dlg_Gaho7$", pPlayer);
  g_fGahoSpoken=1;
  }

private func DlgGaho8()
  {
  if(g_fEteniaSpoken<2)
  {
      MessageEx("$Dlg_Gaho8b$", pNPC);
      StopDialog();
      return();
  }
  MessageEx("$Dlg_Gaho8$", pNPC);
  g_fGahoSpoken=2;
  }

private func DlgGaho9()
  {
  MessageEx("$Dlg_Gaho9$", pNPC);
  if(!FindObject2(Find_Or(Find_ID(GBLT),Find_ID(EGBL)),Find_Container(pPlayer))) StopDialog();
  }

private func DlgGaho10()
  {
  var pGoblet = FindObject2(Find_Or(Find_ID(GBLT),Find_ID(EGBL)),Find_Container(pPlayer));
  if(!pGoblet) return(StopDialog());
  RemoveObject(pGoblet);
  MessageEx("$Dlg_Gaho10$", pNPC);
  }

private func DlgGaho11()
  {
  var iEffect = GetEffect("StayThere",pNPC);
  EffectVar(0,pNPC,iEffect) = 4120;
  EffectVar(1,pNPC,iEffect) =  310;
  SetCommand(pNPC,"MoveTo",0,4120,310);
  AddCommand(pNPC,"MoveTo",0,3830,280);
  AddCommand(pNPC,"Wait",0,0,0,0,0,40);
  AddCommand(pNPC,"Enter",pNPC->~FindObject(CAVE,AbsX(3515),AbsY(474),-1,-1));
  SetCommand(pPlayer,"MoveTo",0,4100,310);
  AddCommand(pPlayer,"MoveTo",0,3830,280);
  AddCommand(pPlayer,"Wait",0,0,0,0,0,60);
  AddCommand(pPlayer,"Enter",pNPC->~FindObject(CAVE,AbsX(3515),AbsY(474),-1,-1));
  AddCommand(pPlayer,"Wait",0,0,0,0,0,20);
  }

private func DlgGaho12()
  {
  if(Distance(GetX(pNPC), GetY(pNPC), 4124, 306)>10)
  {       // Bei zu langer Wartezeit den NPC gleich an die Position setzen
    if(Local(10,pNPC)++>15) {
      SetPosition(4120,306,pNPC); SetPosition(4100,306,pPlayer);
//      SetCommand(pNPC,"None"); SetCommand(pPlayer,"None");
//      SetComDir(COMD_None,pNPC); SetComDir(COMD_None,pPlayer);
    }
    else return(iTalkIndex--, iWait=35);
  }
  SetCommand(pNPC,"None"); SetCommand(pPlayer,"None");
  SetComDir(COMD_None,pNPC); SetComDir(COMD_None,pPlayer);
  MessageEx("$Dlg_Gaho11$", pNPC);
  ObjectSetAction(pNPC,"Magic");
  }

private func DlgGaho14()
  {
  Schedule("RemoveObject(FindObject(_FWS))",5,9);
  iWait = 35 + 4*9;
  g_fGahoSpoken = 3;
  }

private func DlgGaho15()
  {
  MessageEx("$Dlg_Gaho12$", pNPC);
  }

private func DlgGaho16()
  {
  var iEffect = GetEffect("StayThere",pNPC);
  EffectVar(0,pNPC,iEffect) = 3280;
  EffectVar(1,pNPC,iEffect) =  500;
  SetCommand(pNPC,"MoveTo",0,3280,500);
  AddCommand(pNPC,"Wait",0,0,0,0,0,40);
  AddCommand(pNPC,"Enter",pNPC->~FindObject(CAVE,AbsX(3522),AbsY(275),-1,-1));
  AddCommand(pNPC,"Wait",0,0,0,0,0,70);
  }

private func DlgGaho17()
  {
  MessageEx("$Dlg_Gaho13$", pPlayer);
  StopDialog();
  }