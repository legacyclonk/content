/*-- Geist des Lebens --*/

#strict
#include FIRG

private func GhostEffect() {
  CreateParticle("GhostSpark",RandomX(-10,10),RandomX(14,18),0,0,RandomX(80,140),BleachClr(RGBa(Random(4)*127,Random(4)*127,Random(4)*127,iAlpha),128));
}

private func BleachClr(dwordRGB,iStr) {
 var iR,iG,iB;
 iR=BoundBy(GetRGBValue(dwordRGB,1)+iStr,0,255);
 iG=BoundBy(GetRGBValue(dwordRGB,2)+iStr,0,255);
 iB=BoundBy(GetRGBValue(dwordRGB,3)+iStr,0,255);
 return(RGBa(iR,iG,iB,GetRGBValue(dwordRGB,0)));
}

/* Gespräche */

private func Speaking() {
  var iDelay;
  iDelay=GetActMapVal("Delay","Exist")*GetActMapVal("Length","Exist");
  if(iWait=Max(0,iWait-iDelay)) return();
  Call(Format("Msg%d",iMsg));
  if(iMsg!=4) ++iMsg;
}

private func Msg1() {
  Speak("$TxtIgreetyouchildoflife$");
}

private func Msg2() {
  Speak("$TxtProvetomethefertilit$");
}

private func Msg3() {
  Speak("$TxtIwillgrantyouareward$",GetPlayerCount()*6);
  var k;
  for(var i,j=GetPlayerCount();j;++i) 
    if(GetPlayerName(i)) {
      k+=GetCrewCount(i);
      --j;
    }
  if(k >= GetPlayerCount()*6) Msg5();
}

private func Msg4() {
  if(FindObject(0,-40,-40,80,80,OCF_CrewMember(),0,0,NoContainer())) return(Msg3());
}

private func Msg5() {
  Speak("$TxtWhatapleasureYouhave$");
  iMsg=6;
}

private func Msg6() {
  Speak("$TxtTakeoffyourcoatsaban$");
}

private func Msg7() {
  Speak("$TxtFromnowonyouwilllive$");
}

private func Msg8() {
  Speak("$TxtFarewell$");
  CreateObject(_FIN,0,0,GetOwner());
  if(Global()==5) ++Global();
  Leave();
}