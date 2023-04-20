/*-- Doc --*/

#strict

#include _DIA

func Greeting() {
  if(process > 2)
    return("$egg$");
  return("$Greet$");
}

func GetChatCount() { return(1); }

func ChatHeader() {
  return("$Weapon$");
}

func StartChat() {
  CreateDialogBox(0,"$Doc$");
  AddAnswer(0,"$Thx$","EndChat",PIWP,0);
  CreateContents(PIWP,pPartner);
  CloseChat();
}

public func ChatFinished(int i)
{
  if(i == 0) {
    process++;
  }
}
