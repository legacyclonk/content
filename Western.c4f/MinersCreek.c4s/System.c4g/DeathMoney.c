/*-- Kopfgeld --*/

#strict
#appendto BNDT

protected func Recruitment()
{
  if(!GetEffect("DeathMoney", this()))
    AddEffect("DeathMoney", this(), 1, 0, this());
  return(_inherited());
}

// Kopfgeld
global func FxDeathMoneyStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();
  if(iReason!=4) return(-1);
  if(GetAlive(pTarget)) return(-1);
  var i = -1;
  while( i++ < GetPlayerCount()) { if(CheckTeam(GetPlayerByIndex(i))!=CheckTeam(GetOwner(pTarget))) break; }
  if(Random(30)) // Wahrscheinlichkeit normaler Banditen..
  {
    var rnd = Random(5);
    if(rnd==0) ExtraLog("$MsgDeath0$");
    else if(rnd==1) ExtraLog("$MsgDeath1$");
    else if(rnd==2) ExtraLog("$MsgDeath2$");
    else if(rnd==3) ExtraLog("$MsgDeath3$");
    else if(rnd==4) ExtraLog("$MsgDeath4$");
    DoWealth(GetPlayerByIndex(i),25);
    Sound("Cash",1);
  }
  else // ..und auf Hauptmann
  {
    var rnd = Random(2);
    if(rnd==0) ExtraLog("$MsgDeathChief0$");
    else if(rnd==1) ExtraLog("$MsgDeathChief1$");
    DoWealth(GetPlayerByIndex(i),75);
    Sound("Cash",1);
  }
}
