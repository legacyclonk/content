/*-- Startmenüs nicht abbrechbar --*/

#strict
#appendto CLNK

protected func MenuQueryCancel()
{
 // Die Relaunchmessage verschwindet nach dem Schließen des Relaunchmenüs
 if (GetMenu() == WIPF) return(1);
}