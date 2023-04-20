/*-- Kommunikationsschirm --*/

#strict

#include _DIA

local state;
static process;

func Initialize() {
  SetAction("Off");

  SetName("Commander");

  AddFrame();
  return(_inherited());
}

func Greeting() {
  if(process == 0) return("$Intro1$");
  if(process == 2) return("$Tutor1$");
  if(process == 3) return("$Tutor2$");
  if(process == 4) return("$Tutor4$");
  if(process == 5) return("$Tutor7$");
  if(process == 6) return("$Tutor8$");
  if(process == 7) return("$Tutor10$");
  if(process == 8) return("$Tutor11$");
  if(process == 9) return("$Tutor12$");
  if(process == 10) return("$Tutor14$");
  if(process == 11) return("$Tutor17$");
  if(process == 12) return("$Tutor18$");
  if(process == 13) return("$Tutor19$");
  if(process == 14) return("$Tutor20$");
  if(process == 15) return("$Tutor25$");
  if(process == 16) return("$Tutor27$");
  if(process == 17) return("$Tutor28$");
  if(process == 18) return("$Tutor30$");
  if(process == 19) return(Format("$TutorEND$",GetName(GetCrew())));
}

func GetChatCount() { return(1); }

func GetMenuPortrait() {
  return("Portrait:CSCR::0000ff::1");
}

func ChatHeader() {
  if(process == 2 || process == 5 || process == 7 || process == 8 || process == 13 || process == 16 || process == 19)
    return("Okay");
  else
    return("$Continue$");
}

func StartChat(int i) {
  if(process == 0) {
    state = 0;
    CreateDialogBox(0,"$Intro2$");
    AddAnswer(0,"$Continue$","Dialog",CSCR);
  }
  else if(process == 3) {
    CreateDialogBox(0,"$Tutor3$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else if(process == 4) {
    state = 1;
    CreateDialogBox(0,"$Tutor5$");
    AddAnswer(0,"$Continue$","Dialog",CSCR);
  }
  else if(process == 6) {
    CreateDialogBox(0,"$Tutor9$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else if(process == 8) {
    CreateDialogBox(0,"$Tutor11b$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else if(process == 9) {
    CreateDialogBox(0,"$Tutor13$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else if(process == 10) {
    state = 2;
    CreateDialogBox(0,"$Tutor15$");
    AddAnswer(0,"$Continue$","Dialog",CSCR);
  }
  else if(process == 14) {
    state = 3;
    CreateDialogBox(0,"$Tutor21$");
    AddAnswer(0,"$Continue$","Dialog",CSCR);
  }
  else if(process == 15) {
    CreateDialogBox(0,"$Tutor26$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else if(process == 17) {
    CreateDialogBox(0,"$Tutor29$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else if(process == 18) {
    CreateDialogBox(0,"$Tutor31$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else
    EndChat();
}

func Dialog() {
  if(state == 0) {
    CreateDialogBox(0,"$Intro3$");
    AddAnswer(0,"Okay","EndChat",SCCK);
  }
  else if(state == 1) {
    CreateDialogBox(0,"$Tutor6$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else if(state == 2) {
    CreateDialogBox(0,"$Tutor16$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
  else if(state == 3) {
    CreateDialogBox(0,"$Tutor22$");
    AddAnswer(0,"Okay","Dialog",CSCR);
    state = 4;
  }
  else if(state == 4) {
    CreateDialogBox(0,"$Tutor23$");
    AddAnswer(0,"Okay","Dialog",CSCR);
    state = 5;
  }
  else if(state == 5) {
    CreateDialogBox(0,"$Tutor24$");
    AddAnswer(0,"Okay","EndChat",TARG);
  }
}

func ChatFinished() {
  process++;
  Sound("Ack",1);
}
