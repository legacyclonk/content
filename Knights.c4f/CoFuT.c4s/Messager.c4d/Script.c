/*-- Nachrichtenanzeiger --*/

#strict

local aMessages;

func Initialize() {
   SetPosition(0,0,this());
   
   Local()=0;
   Local(1)=0;
   
   aMessages=[];

  return(1);
}

public func Init()
{
 SetAction("Be");
}

protected func Check()
{
 
 if(!DeleteOld())return();
 Show();
}


protected func NewPosition(force)
{ 
 if(force)SetPosition(GetX(),GetY()+15);
 else
 SetPosition(120,80+15*GetLength(aMessages)/2);
 
 if(FindObject(GetID(),-1,-1,2,2))FindObject(GetID(),-1,-1,2,2)->NewPosition(1);
 
}

protected func Show()
{
 var sString="";
 for(var cnt=GetLength(aMessages)-2;cnt>=0;cnt-=2)
 {
  sString=Format("%s    <c %x>%s|",sString,RGBa(200,200,200,255-(GetActTime(FindObject(GetID(),0,0,0,0,0,"MainSaver"))-aMessages[cnt+1])/3),aMessages[cnt]);
 }
 CustomMessage(sString,this(),NO_OWNER,18,0,0,0,0,MSG_NoLinebreak);
}


protected func DeleteOld()
{
 if(aMessages[1]+550 < GetActTime(FindObject(GetID(),0,0,0,0,0,"MainSaver")))
 {RemoveObject(); return(0);}

return(1);
}

protected func DeleteSome()//raeumt auf
{
var counter;
 for(var cnt=0;cnt<GetLength(aMessages) && counter<1;cnt+=2)
 {

   DeleteArrayItem(cnt,aMessages);
   DeleteArrayItem(cnt,aMessages);
   counter++;
 }
}

public func AddMessage(string sMessage)
{
 var length=GetLength(aMessages);
 aMessages[length]=sMessage;
 aMessages[length+1]=GetActTime(FindObject(GetID(),0,0,0,0,0,"MainSaver"));
 if(GetLength(aMessages)/2>13) DeleteSome();
 
 Init();
 
 NewPosition();
}

global func ShowMessage(string sMessage)
{
 if(!ObjectCount(_MSG,0,0,0,0,0,"MainSaver"))return();
 
 var obj;
 obj=CreateObject(_MSG,0,0,NO_OWNER);
 obj->AddMessage(sMessage);
 
 return(1);
}
