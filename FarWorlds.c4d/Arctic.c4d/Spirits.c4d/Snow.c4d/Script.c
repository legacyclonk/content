/*-- Geist des Schnees --*/

#strict
#include FIRG

private func GhostEffect() {
  for(var i=0;i<3;++i)
  CreateParticle("GhostSpark",RandomX(-14,14),RandomX(18,22),0,RandomX(-15,-30),RandomX(15,45),RGBa(196,255,255,iAlpha));
}

/* Gespräche */

private func Msg1() {
  Speak("$TxtKneelbeforetheSpirit$");
}

private func Msg2() {
  Speak("$TxtProveyoursubmissiont$");
 // Baupläne für alle Spieler
  for(var i,j=GetPlayerCount();j;++i)
    if(GetPlayerName(i)) {
      SetPlrKnowledge(i,QBII);
      SetPlrKnowledge(i,QIID);
      --j;
    }
}

private func Msg3() {
  var pStatue,pBig,pLeft,pRight;
  // Große Statue auf dieser Insel
  pBig=FindObject(QBII,-750,-1000,1500,2000,OCF_Fullcon());

  // Kleine Statue links
  while(pStatue=FindObject(QIID,0,0,0,0,OCF_Fullcon(),0,0,0,pStatue))
    if(GetX(pStatue)<GetX()-500 ||  (GetX()<1000 && GetX(pStatue)<GetX()))    
      pLeft=pStatue;

  // Kleine Statue rechts
  while(pStatue=FindObject(QIID,0,0,0,0,OCF_Fullcon(),0,0,0,pStatue))
    if(GetX(pStatue)>GetX()+500 || (GetX()>LandscapeWidth()-1000 && GetX(pStatue)>GetX()))
      pRight=pStatue;

  // Noch nicht alle drei vorhanden
  Speak("$TxtBuildformealargesnow$");

  if(pBig && pLeft && pRight) Msg5();
}

private func Msg4() {
  if(FindObject(0,-40,-40,80,80,OCF_CrewMember(),0,0,NoContainer())) return(Msg3());
}

private func Msg5() {
  Speak("$TxtYouhaveprovenworthyo$");
  if(Global()==2) ++Global();
  Leave();
}