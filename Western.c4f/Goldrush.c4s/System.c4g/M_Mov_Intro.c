/* Dialog: Intro */

#strict
#appendto _TLK

static g_pIntroHorse;

public func MovIntroStart()
  {
//  SetCommand(pPlayer, "MoveTo", 0, 130, 878);
  MessageEx("$Mov_Intro0$", pPlayer);
  if(!WildcardMatch(GetAction(pPlayer),"Ride*")) {
    Exit(pPlayer);
    ObjectSetAction(pPlayer,"Ride",pWagon);
  }
  var g_pIntroHorse = pPlayer->FindObject(HORS, 0, 0, -1, -1);
  ObjectSetAction(g_pIntroHorse,"Stand");
  SetDir(1,g_pIntroHorse);
  g_pIntroHorse->SetGait(3);
  SetColorDw(RGB(255,255,255), pNPC);
  iWait=41;
  return(1);
  }

private func MovIntro1()
  {
  if(GetX(pPlayer)<278)
  {
    iWait=41; // Warten
    return(iTalkIndex--);
  }
  if(!g_pIntroHorse) g_pIntroHorse = pPlayer->FindObject(HORS, 0, 0, -1, -1);
  g_pIntroHorse->~DisconnectWagon();
  iWait=41;
  }

private func MovIntro2()
  {
  if(!g_pIntroHorse) g_pIntroHorse = pPlayer->FindObject(HORS, 0, 0, -1, -1);
  g_pIntroHorse->~DoDamage(1000);
  MessageEx("$Mov_Intro2$", pPlayer);
  iWait=36;
  }

private func MovIntro3()
  {
  MessageEx("$Mov_Intro3$", pNPC);
  iWait=36;
  }

private func MovIntro4()
  {
  MessageEx("$Mov_Intro4$", pPlayer);
  iWait=74;
  }

private func MovIntro5()
  {
  MessageEx("$Mov_Intro5$", pNPC);
  iWait=214;
  }

private func MovIntro6()
  {
  MessageEx("$Mov_Intro6$", pPlayer);
  iWait=182;
  }

private func MovIntro7()
  {
  MessageEx("$Mov_Intro7$", pNPC);
  iWait=168;
  }

private func MovIntro8()
  {
  MessageEx("$Mov_Intro8$", pPlayer);
  iWait=102;
  }

private func MovIntro9()
  {
  SetColorDw(GetPlrColorDw(GetOwner(pNPC)), pNPC);
  Schedule("CreateDifficultyMenu()", 1, 1, pPlayer);
  StopDialog();
  }
