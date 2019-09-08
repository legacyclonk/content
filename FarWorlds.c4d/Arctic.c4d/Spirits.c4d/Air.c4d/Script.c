/*-- Geist der Luft --*/

#strict
#include FIRG

protected func Initialize() {
  SetPosition(GetX(),GetY()-20);
  iAlpha=255;
  iWait=0;
  iMsg=1;
  SetClrModulation(RGBa(255-iAlpha,255-iAlpha,255-iAlpha,iAlpha));
}

private func GhostEffect() {
  if(!Random(2))
    CreateParticle("GhostSpark",RandomX(-16,16),RandomX(0,8),0,RandomX(8,10),RandomX(100,180),RGBa(255,255,196,BoundBy(iAlpha+32,0,255)));
}

/* Gespräch */

private func Msg1() {
  Speak("$TxtTheSpiritofAirgreets$");
}

private func Msg2() {
  Speak("$TxtSomeofmystarshavefal$");
  for(var i=0;i<6;++i) CreateObject(QSTR,Random(LandscapeWidth())-GetX(),-GetY(),-1);
}

private func Msg3() {
  var pStar;
  Speak("$TxtFindthedloststarsand$",ObjectCount(QSTR));
  if(pStar=FindObject(QSTR,-40,-20,80,80)) return(RemoveStar());
  if(!FindObject(QSTR)) return(Msg5());
}

private func Msg4() {
  if(FindObject(0,-40,-20,80,80,OCF_CrewMember(),0,0,NoContainer())) return(Msg3());
}

private func Msg5() {
  Speak("$TxtYouhavecompletedyour$");
  if(Global()==1) ++Global();
  CreateContents(RLKT,GetCursor(GetOwner(pTarget)));
  Leave();
}

private func RemoveStar() {
  var pStar;
  while(pStar=FindObject(QSTR,-40,-20,80,80)) RemoveObject(pStar);
  if(ObjectCount(QSTR)>1)
    Speak("$TxtAhThankyouNowtheress$",ObjectCount(QSTR));
  else if(ObjectCount(QSTR)==1)
    Speak("$TxtAhThankyouNowtherone$");
  else
    Speak("$TxtFinallyIhaveallmysta$");
}