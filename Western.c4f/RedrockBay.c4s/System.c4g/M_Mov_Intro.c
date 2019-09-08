/* Dialog: Intro */

#strict
#appendto _TLK

public func MovIntroStart()
  {
  pPlayer = FindObjectOwner(COWB, GetOwner(pPlayer));
  pNPC    = FindObjectOwner(COWB, GetOwner(pPlayer), 0, 0, 0, 0, 0, 0, 0, pPlayer);
  Local(10) = FindObjectOwner(TRPR, GetOwner(pPlayer));
  }

private func MovIntro0()
  {
  MessageEx("$Mov_Intro0$", pPlayer);
  iWait=42;
  return(1);
  }

private func MovIntro1()
  {
  MessageEx("$Mov_Intro1$", pNPC);
  iWait=110;
  }

private func MovIntro2()
  {
  MessageEx("$Mov_Intro2$", pPlayer);
  iWait=142;
  }

private func MovIntro3()
  {
  MessageEx("$Mov_Intro3$", Local(10));
  iWait=108;
  }

private func MovIntro4()
  {
  MessageEx("$Mov_Intro4$", pPlayer);
  iWait=128;
  }

private func MovIntro5()
  {
  MessageEx("$Mov_Intro5$", Local(10));
  iWait=120;
  }

private func MovIntro6()
  {
  MessageEx("$Mov_Intro6$", pPlayer);
  iWait=136;
  }

private func MovIntro7()
  {
  MessageEx("$Mov_Intro7$", pPlayer);
  iWait=254;
  }

private func MovIntro8()
  {
  MessageEx("$Mov_Intro8$", pNPC);
  iWait=82;
  }

private func MovIntro9()
  {
  MessageEx("$Mov_Intro9$", pPlayer);
  iWait=194;
  }

private func MovIntro10()
  {
  MessageEx("$Mov_Intro10$", pPlayer);
  iWait=250;
  }

private func MovIntro11()
  {
  MessageEx("$Mov_Intro11$", Local(10));
  iWait=300;
  }

private func MovIntro12()
  {
  MessageEx("$Mov_Intro12$", pPlayer);
  iWait=64;
  }

private func MovIntro13()
  {
  StopDialog();
  }
