/*-- Geist des Todes --*/

#strict
#include FIRG

private func GhostEffect() {
  for(var i=0;i<2;++i)
  CreateParticle("GhostSpark",RandomX(-50,50),RandomX(10,12),RandomX(-10,10),RandomX(-15,-35),RandomX(200,500),RGBa(64,32,32,iAlpha));
}

/* Gespräche */

private func Msg1() {
  Speak("$TxtLookatyoulittlemorta$");
}

private func Msg2() {
  Speak("$TxtYouhavedisturbedmyre$");
  while(FindObject(FISH) && ObjectCount(QFSH)<10)
    FindObject(FISH)->~Sick();
}

private func Msg3() {
  Speak("$TxtKillalltheblackfishI$",ObjectCount(QFSH));
  if(!FindObject(QFSH)) Msg5();
}

private func Msg4() {
  if(FindObject(0,-40,-40,80,80,OCF_CrewMember(),0,0,NoContainer())) return(Msg3());
}

private func Msg5() {
  Speak("$TxtIcantbelieveyouhavem$");
  if(Global()==4) ++Global();
  Leave();
}