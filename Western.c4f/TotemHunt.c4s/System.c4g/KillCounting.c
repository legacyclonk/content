/*-- Kopfgeld --*/

#strict
#appendto COWB

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
  if(iReason!=4) return(-1);
  if(GetAlive(pTarget)) return(-1);
  var i = -1;
  if(CheckTeam(GetOwner(pTarget))==1)
  {
    killWOLF++;
    ActualizeScoreboard();
  }
  else if(CheckTeam(GetOwner(pTarget))==2)
  {
    killTRPR++;
    ActualizeScoreboard();
  }
  else
  {
    killBEAR++;
    ActualizeScoreboard();
  }
}