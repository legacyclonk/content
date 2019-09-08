/*-- Kopfgeld --*/

#strict
#appendto COWB

static killleft, killright;

protected func Recruitment()
{
  if(!GetEffect("DeathCounting", this()))
    AddEffect("DeathCounting", this(), 1, 0, this());
  return(_inherited());
}

// Kopfgeld
global func FxDeathCountingStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();
  if(iReason==4) if(GetAlive(pTarget)) return(-1);
  if(GetCrewCount(GetOwner(pTarget))>1)
    if(FindFreeBanditPlayer(GetOwner(pTarget))<=1)
      GameCall("RelaunchPlayer", GetOwner(pTarget), NO_OWNER);
  var i = -1;
  if(CheckTeam(GetOwner(pTarget))==1)
  {
    killright++;
    ActualizeScoreboard();
  }
  else
  {
    killleft++;
    ActualizeScoreboard();
  }
}

global func FindFreeBanditPlayer(iPlr)
{
  var bandit = 0;// = false;
  for(var j=0 ; j < GetCrewCount(iPlr) ; j++)
  {
    if(!Contained(GetCrew(iPlr,j)))
      bandit++;// = true;
    else
      if(! (Contained(GetCrew(iPlr,j))->~IsPrison()))
       bandit++;// = true;
  }
  return(bandit);
}
