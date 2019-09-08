/*-- Spielerobjekt --*/

#strict

local iPlr;

/* Spielstart */

protected func InitPlayer(int player, int iTeam)
{
 iPlr = player;
 for (var i; GetCrew(iPlr, i); i++) Enter(this(), GetCrew(iPlr, i));
 if (iTeam==1) return(ContainedLeft());
 if (iTeam==2) return(ContainedRight());
 SetPlrShowControl(iPlr, "______6_8_______6_8___________");
}

protected func ContainedLeft()
{
 [Links|Image=KTM1:5]
 if (Global(iPlr)) return(1);
 Global(iPlr) = -1;
 DoPlrLaunch();
 Log("$MsgJoinShadow$", GetPlayerName(iPlr));
 SetPlrShowControl(iPlr, "______________________________");
 return(1);
}

protected func ContainedRight()
{
 [Rechts|Image=KTM1:1|Condition=IsStart]
 if (Global(iPlr)) return(1);
 Global(iPlr) = 1;
 DoPlrLaunch();
 Log("$MsgJoinMantis$", GetPlayerName(iPlr));
 SetPlrShowControl(iPlr, "______________________________");
 return(1);
}

private func DoPlrLaunch()
{
 // Sound abspielen
 Sound("Ding");

 // Den Spieler bei seiner Position rauswerfen
 var x = RandomX(130,220); if (Global(iPlr) == 1) var x = RandomX(1480,1570);
 var y = 375; if (Global(iPlr) == 1) var y = 375;
 while (Contents()) Exit(Contents(), x-GetX(), y-GetY());

 // Bei einem Relaunch ist es jetzt fertig.
 if (GetAction() eq "Relaunch") return(RemoveObject());
 
 // Ansonsten die Verfeindung setzen.
 SetHostilities();

 // Alle Spieler durchgehen, falls dieser der Erste in seinem Team ist, wird er Burgherr.
 var i = GetPlayerCount();
 while (i--)
  if (GetPlayerByIndex(i) != iPlr && Global(GetPlayerByIndex(i)) == Global(iPlr))
   return(RemoveObject());

 if (Global(iPlr) == 1) AcquireBase(iPlr, 1270,70,430,460);
 else AcquireBase(iPlr, 10,65,430,460);

 return(RemoveObject());
}

private func SetHostilities()
{
 // Den Spieler mit Spielern des anderen Teams verfeinden.
 var i = GetPlayerCount();
 while (i--)
 {
  SetHostility(iPlr, GetPlayerByIndex(i), Global(iPlr) != Global(GetPlayerByIndex(i)), 1);
  SetHostility(GetPlayerByIndex(i), iPlr, Global(iPlr) != Global(GetPlayerByIndex(i)), 1);
 }
}

public func AcquireBase(iPlr, iPosx, iPosy, iWidth, iHeight)
{
 var obj;
 while (obj = FindObject(0, iPosx-GetX(), iPosy-GetY(), iWidth, iHeight, 0, 0,0, 0, obj))
  if(!GetAlive(obj))
  if(GetID(obj)!=FLAG)
    SetOwner(iPlr, obj);
}

/* Relaunch */

protected func RelaunchPlayer(int player)
{
 iPlr = player;
 for (var i; GetCrew(iPlr, i); i++) Enter(this(), GetCrew(iPlr, i));
 SetAction("Relaunch");
}

private func Execute()
{
 if (!Contents()) return(RemoveObject());
 if (GetActTime() >= 2000) return(DoPlrLaunch());
 PlrMessage("$MsgRelaunchCountdown$", iPlr, (2000-GetActTime()) / 25 / 60, (2000-GetActTime()) / 25 % 60);
}

/* Sonstiges */

protected func RejectCollect() { return(1); } // Damit man nichts in das Objekt ablegen kann
