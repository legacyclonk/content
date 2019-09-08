/*-- Spielerobjekt --*/

#strict

local iPlr;

/* Spielstart */

protected func InitPlayer(int player, int iTeam)
{
 iPlr = player;
 for (var i; GetCrew(iPlr, i); i++) Enter(this(), GetCrew(iPlr, i));
 DoPlrLaunch();
}

private func DoPlrLaunch()
{
 // Sound abspielen
 Sound("Ding", true);

 // Den Spieler bei seiner Position rauswerfen
 var x; if (GetPlayerTeam(iPlr) == CTEAM_Left) x = 100; else x = 1400;
 while (Contents()) Exit(Contents(), x-GetX(), 230-GetY());

 // Bei einem Relaunch ist es jetzt fertig.
 if (GetAction() eq "Relaunch")
 {
	// Halt! Sprengpfeile noch...?
	// Mal ohne probieren
/*	x = 1400; if (Global(iPlr) == 1) x = 100;
	CreateObject(XARP, x-GetX(), 230-GetY(), -1);
	CreateObject(BOW1, x-GetX(), 230-GetY(), -1);*/
	//Sparkle();
	return(RemoveObject());
 }

 // Alle Spieler durchgehen, falls dieser der Erste in seinem Team ist, wird die Burg nach ihm gefärbt.
 var i = GetPlayerCount();
 while (i--)
  if (GetPlayerByIndex(i) != iPlr && GetPlayerTeam(GetPlayerByIndex(i)) == GetPlayerTeam(iPlr))
   return(RemoveObject());

 if (GetPlayerTeam(iPlr) == CTEAM_Right) SetCastleColor(iPlr, 1035,45, 405,205);
 else SetCastleColor(iPlr, 0,50, 400,200);

 return(RemoveObject());
}

public func SetCastleColor(iPlayer, iPosx, iPosy, iWidth, iHeight)
{
 // Alles umfärben.
 for (var obj in FindObjects(Find_InRect(iPosx-GetX(), iPosy-GetY(), iWidth, iHeight), Find_Not(Find_OCF(OCF_CrewMember)), Find_Not(Find_ID(FLAG))))
  SetColorDw(GetPlrColorDw(iPlayer), obj);

 // Fertig!
 return(1);
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
 PlrMessage("$MsgRelaunchTime$", iPlr, (2000-GetActTime()) / 25 / 60, (2000-GetActTime()) / 25 % 60);
}

/* Sonstiges */

protected func RejectCollect() { return(1); } // Damit man nichts in das Objekt ablegen kann
