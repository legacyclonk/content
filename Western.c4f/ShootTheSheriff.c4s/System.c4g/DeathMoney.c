/*-- Kopfgeld --*/

#strict
#appendto COWB

static killleft, killright;

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
  while( i++ < GetPlayerCount()) { if(GetPlayerTeam(GetPlayerByIndex(i))!=GetPlayerTeam(GetOwner(pTarget))) break; }
  DoWealth(GetPlayerByIndex(i),25);
  Sound("Cash",1);
  if(GetPlayerTeam(GetPlayerByIndex(i))==2)
  {
    var rnd = Random(5);
    if(rnd==0) ExtraLog("$MsgCoals1$");
    else if(rnd==1) ExtraLog("$MsgCoals2$");
    else if(rnd==2) ExtraLog("$MsgCoals3$");
    else if(rnd==3) ExtraLog("$MsgCoals4$");
    else if(rnd==4) ExtraLog("$MsgCoals5$");
    killright++;
    ActualizeScoreboard();
  }
  else
  {
    var rnd = Random(5);
    if(rnd==0) ExtraLog("$MsgFort1$");
    else if(rnd==1) ExtraLog("$MsgFort2$");
    else if(rnd==2) ExtraLog("$MsgFort3$");
    else if(rnd==3) ExtraLog("$MsgFort4$");
    else if(rnd==4) ExtraLog("$MsgFort5$");
    killleft++;
    ActualizeScoreboard();
  }
}
