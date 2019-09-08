/*-- Geist des Feuers --*/

#strict

local pTarget,iAlpha,iWait,iMsg;

protected func Initialize() {
  iAlpha=255;
  iWait=0;
  iMsg=1;
  SetClrModulation(RGBa(255-iAlpha,255-iAlpha,255-iAlpha,iAlpha));
}

public func Activate(pAngakok) {
  pTarget=pAngakok;

  SetAction("Arrival");
  // Meldung für alle Spieler
  Log("$TxtConjured$",GetName());

  // in die richtige Richtung schauen
  if(!GetDir(pTarget)) SetDir(DIR_Right());
  else                 SetDir(DIR_Left ());
}

public func Leave() {
  SetAction("Leave");
  // Meldung für alle Spieler
  Log("$TxtLeave$",GetName());
}

private func Arriving() {		// Der Geist erscheint
  GhostSound();
  GhostEffect();
  iAlpha=Max(iAlpha-GetActMapVal("Delay",GetAction())*85/51,0);
  SetClrModulation(RGBa(255-iAlpha,255-iAlpha,255-iAlpha,iAlpha));
  if(iAlpha<=0) SetAction("Exist");
}

private func GhostSound() {}

private func GhostEffect() {
  CreateParticle("GhostSpark",RandomX(-20,20),RandomX(18,22),0,0,RandomX(50,100),RGBa(255,64,0,iAlpha));
}

private func Leaving() {		// Der Geist verschwindet
  GhostSound();
  GhostEffect();
  iAlpha=Min(iAlpha+GetActMapVal("Delay",GetAction())*85/51,255);
  SetClrModulation(RGBa(255-iAlpha,255-iAlpha,255-iAlpha,iAlpha));
  if(iAlpha>=255) RemoveObject();
}

/* Gespräch */

private func Speaking() {
  var iDelay;
  iDelay=GetActMapVal("Delay","Exist")*GetActMapVal("Length","Exist");
  if(iWait=Max(0,iWait-iDelay)) return();
  Call(Format("Msg%d",iMsg));
  if(iMsg<4) ++iMsg;
}

private func SpeakDelay() {
  var iDelay;
  iDelay=GetActMapVal("Delay","Speak")*GetActMapVal("Length","Speak");
  if(GetActTime()+iDelay>=iWait) {
    iWait=Max(0,iWait-GetActTime());
    SetAction("Exist");
  }
}

private func Speak(strText) {
  Message(strText,this(),Par(1),Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8));
  SetAction("Speak");
  iWait=Max(Length(strText)*2,80);
}

private func Msg1() {
  Speak("$TxtIamtheSpiritofFireCo$");
}

private func Msg2() {
  Speak("$TxtThisisyourtask$");
  // Lagerfeuerbaupläne für alle Spieler
  for(var i,j=GetPlayerCount();j;++i)
    if(GetPlayerName(i)) {
      SetPlrKnowledge(i,CPFR);
      --j;
    }
}

private func Msg3() {
  Speak("$TxtBuildalargefireIwill$");
}

private func Msg4() {
  if(FindObject(0,-40,-40,80,80,OCF_CrewMember(),0,0,NoContainer())) return(Msg3());
}

private func Msg5() {
  Speak("$TxtYouhavecompletedyour$");
  if(Global()==0) ++Global();
  CreateContents(RFRL,GetCursor(GetOwner(pTarget)));
  CreateContents(RFRL,GetCursor(GetOwner(pTarget)));
  Leave();
}

public func Build(pFire) { if(!iAlpha) Msg5(); }

// Berechnet die Länge eines Strings (von Clonk-Karl)
private func Length(string str) {
   for(var i=0;;++i)
     if(!GetChar(str,i)) break;
   return(i);
}