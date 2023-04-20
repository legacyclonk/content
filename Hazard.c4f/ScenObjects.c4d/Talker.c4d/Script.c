/*-- Talker --*/

#strict
#include _DIA

local talkto, greet, answer;

func Initialized()
{
  SetVisibility(VIS_None);
}

public func Set(object target, string greeting, string ok)
{
  SetAction("Attach",target);
  SetName(GetName(target));
  greet = greeting;
  answer = ok;
  talkto = target;
}

/* Chatfunktionalität */

public func Greeting() { return(greet); }
public func GetChatCount() { return(1); }

public func ChatHeader(int i)
{
  return(answer);
}

public func GetMenuPortrait()
{
  return(Format("Portrait:%i::%x::%s", GetID(talkto), GetColorDw(talkto), "1"));
}

public func StartChat(int i)
{
  EndChat();
}

public func ChatFinished(int i)
{
  GameCall("ChatFinished",talkto,i,this());
}
