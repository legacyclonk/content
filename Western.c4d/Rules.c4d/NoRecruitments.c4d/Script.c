/*-- Keine Ausbildungen --*/

#strict

#appendto SHOF
#appendto AMRY
#appendto WTNT
#appendto NTIP

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

public func Recruit(object pClonk, id idClonk)
{
  if(FindObject(NR4V)) {
    if(idClonk == SHRF) return(PlayerMessage(GetOwner(pClonk),"$NoSheriff$",pClonk));
    if(idClonk == CVRM) return(PlayerMessage(GetOwner(pClonk),"$NoCaptain$",pClonk));
    if(idClonk == BNDT) return(PlayerMessage(GetOwner(pClonk),"$NoBoss$",   pClonk));
    if(idClonk == INDI) return(PlayerMessage(GetOwner(pClonk),"$NoChief$",  pClonk));
    return(true);
  }
  return(_inherited(pClonk, idClonk));
}

// Dies sind noch die alten Funktionen, doch damit es damit auch funktioniert lasse ich es noch hier

public func RecruitSherif(idType, pClonk)
{
  if(FindObject(NR4V)) return(Message("$NoSheriff$",pClonk));
  return(_inherited(idType, pClonk));
}

public func RecruitCaptain(idType, pClonk)
{
  if(FindObject(NR4V)) return(Message("$NoCaptain$",pClonk));
  return(_inherited(idType, pClonk));
}

public func RecruitBoss(idType, pClonk)
{
  if(FindObject(NR4V)) return(Message("$NoBoss$",pClonk));
  return(_inherited(idType, pClonk));
}

public func RecruitChief(idType, pClonk)
{
  if(FindObject(NR4V)) return(Message("$NoChief$",pClonk));
  return(_inherited(idType, pClonk));
}