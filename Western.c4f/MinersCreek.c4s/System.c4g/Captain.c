/*-- Captains können ausgebildet werden --*/

#strict
#appendto AMRY

protected func ContainedDig(object pClonk)
{
  [$TxtRecruitCaptain$|Image=CVRM]
  if(GetID(pClonk)==BNDT) return(BossMenu(pClonk));
  if(GetID(pClonk)!=CVRM) return();
  CreateMenu (CVRM, pClonk, this(), 2, "", 0, 0, 0);
  var pIcon = CreateObject(CVRM);
  SetGraphics("Captain", pIcon, CVRM);
  AddMenuItem ("$TxtRecruitCaptain$", "RecruitCaptain", CVRM, pClonk, 0, pClonk, 0,4+128,pIcon,35);
  RemoveObject(pIcon);
  AddMenuItem ("$TxtCancel$", "Cancel", MCMX, pClonk, 0, pClonk);
}

public func RecruitCaptain(id, pClonk)
{
  var obj;
   while(obj = FindObject(CVRM, 0, 0, 0, 0, OCF_Alive, 0, 0, 0, obj)) if(obj->~IsCaptain())
	 if(!Hostile(GetOwner(obj), GetOwner(pClonk))) return(Message("$OnlyOneCaptain$", pClonk));
  if(GetWealth(GetOwner(pClonk))<35) return(Message("$NotEnoughMoney$", pClonk));
  DoWealth(GetOwner(pClonk),-35);
  pClonk->~MakeCaptain();
  PlayerMessage(GetOwner(pClonk), "$RecruitCaptain$", pClonk);
  Sound("Cash", 1, 0,0, GetOwner(pClonk));
  return(1);
}

protected func BossMenu(object pClonk)
{
  if(GetID(pClonk)!=BNDT) return();
  CreateMenu (BNDT, pClonk, this(), 2, "", 0, 0, 0);
  var pIcon = CreateObject(BNDT);
  SetGraphics("Boss", pIcon, BNDT);
  AddMenuItem ("$TxtRecruitBoss$", "RecruitBoss", BNDT, pClonk, 0, pClonk, 0,4+128,pIcon,35);
  RemoveObject(pIcon);
  AddMenuItem ("$TxtCancel$", "Cancel", MCMX, pClonk, 0, pClonk);
}

public func RecruitBoss(id, pClonk)
{
  var obj;
  while(obj = FindObject(BNDT, 0, 0, 0, 0, OCF_Alive, 0, 0, 0, obj)) if(obj->~IsCaptain())
	 if(!Hostile(GetOwner(obj), GetOwner(pClonk))) return(Message("$OnlyOneBoss$", pClonk));
  if(GetWealth(GetOwner(pClonk))<35) return(Message("$NotEnoughMoney$", pClonk));
  DoWealth(GetOwner(pClonk),-35);
  pClonk->~MakeBoss();
  PlayerMessage(GetOwner(pClonk), "$RecruitBoss$", pClonk);
  Sound("Cash", 1, 0,0, GetOwner(pClonk));
  return(1);
}

public func Cancel() { }
