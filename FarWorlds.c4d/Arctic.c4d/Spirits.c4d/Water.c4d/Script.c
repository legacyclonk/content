/*-- Geist des Wassers --*/

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
  CreateParticle("GhostSpark",RandomX(-16,16),RandomX(10,12),0,0,RandomX(50,100),RGBa(64,64,255,iAlpha));
}

/* Gespräche */

private func Msg1() {
  Speak("$TxtIgreetyouclonkIamthe$");
}

private func Msg2() {
  Speak("$TxtIhavelosttenofbymost$");
  CreateContents(QSCE,GetCursor(GetOwner(pTarget)));
  while(ObjectCount(QPRL)<10) CastPearls();
}

private func Msg3() {
  Speak("$TxtFindthedpearlsMakego$",ObjectCount(QPRL));
  if(FindObject(QPRL,-40,-20,80,80)) return(RemovePearl());
}

private func Msg4() {
  if(FindObject(0,-40,-20,80,80,OCF_CrewMember(),0,0,NoContainer())) return(Msg3());
}

private func Msg5() {
  var pSceptre;
  Speak("$TxtThankyouverymuchNowc$");
  if(Global()==3) ++Global();
  if(pSceptre=FindObject(QSCE)) RemoveObject(pSceptre);
  Leave();
}

private func CastPearls() {	// Die Perlen werden nur im Wasser erschaffen
  var iX,iY;
  iX=Random(LandscapeWidth())-GetX();
  while(iY<LandscapeHeight() && !GBackSemiSolid(iX,iY)) iY+=20;
  if(!GBackLiquid(iX,iY)) return(0);
  CreateObject(QPRL,iX,iY,GetOwner());
}

private func RemovePearl() {
  var pPearl; 
  while(pPearl=FindObject(QPRL,-40,-20,80,80)) RemoveObject(pPearl);
  if(ObjectCount(QPRL)>1)
    Speak("$TxtThanksNowIamstillmis$",ObjectCount(QPRL));
  else if(ObjectCount(QPRL)==1)
    Speak("$TxtThanksNowIamstillone$");
  else
    Msg5();
}