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
  if(iReason!=4) return(-1);
  if(GetAlive(pTarget)) return(-1);
  var i = -1;
  if(CheckTeam(GetOwner(pTarget))==1)
  {
    killright++;
    DoWealth(GetPlrByTeam(2), 10); 
    ActualizeScoreboard();
  }
  else
  {
    killleft++;
    DoWealth(GetPlrByTeam(1), 10); 
    ActualizeScoreboard();
  }
  AddPlayerClonkToList(GetOwner(pTarget), pTarget);
}

global func AddPlayerClonkToList(int iPlr, object pClonk)
{
  if(!pClonk) return();
  var iEffect;
  if(!GetEffect(Format("IntPlayerClonkList%s", GetPlayerName(iPlr)), 0))
    AddEffect(Format("IntPlayerClonkList%s", GetPlayerName(iPlr)), 0, 1, 0);
  iEffect = GetEffect(Format("IntPlayerClonkList%s", GetPlayerName(iPlr)), 0);
  var i=-1;
  while(EffectVar(++i, 0, iEffect));
  EffectVar(i, 0, iEffect) = pClonk;
}

global func GetPlayerClonkFromList(int iPlr)
{
  var iEffect;
  if(!GetEffect(Format("IntPlayerClonkList%s", GetPlayerName(iPlr)), 0))
    AddEffect(Format("IntPlayerClonkList%s", GetPlayerName(iPlr)), 0, 1, 0);
  iEffect = GetEffect(Format("IntPlayerClonkList%s", GetPlayerName(iPlr)), 0);
  var pClonk = EffectVar(0, 0, iEffect);
  if(!pClonk) return();
  var i = 0;
  while(EffectVar(++i, 0, iEffect)) { 
  EffectVar(i-1, 0, iEffect) = EffectVar(i, 0, iEffect);  EffectVar(i, 0, iEffect) = 0; }
  return(pClonk);
}
