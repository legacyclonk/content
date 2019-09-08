/*-- Spielerobjekt --*/

#strict

local iPlr;

private func DoPlrLaunch()
{
 // Den Spieler bei seiner Position rauswerfen
 DoPlace(GetHiRank(iPlr), 1, 1, 1);

 // Relaunch ist jetzt fertig.
 return(RemoveObject());
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
 if (GetActTime() >= 25*1) return(DoPlrLaunch());
}

/* Sonstiges */

protected func RejectCollect() { return(1); } // Damit man nichts in das Objekt ablegen kann
