/*-- Mehr Totempfähle --*/

#strict 2

#appendto LHPT
#appendto SKLT
#appendto TTPL
#appendto NTIP

protected func Initialize()
{
  if(!this()->~GetMystic()) return(_inherited(...));

  if(ObjectCount2(Find_Or(Find_ID(GetID()),Find_ID(GetDefBurnTo(GetID()))),Find_Clan(GetClan()),Find_OCF(OCF_Fullcon)) <= this()->~GetBuildLimit())
    return(_inherited(...));
  else
    return(SetAction("Mystic"));
}

protected func Activate(int iByPlayer)
{
  if(GetID(this()) != MT4V) return(_inherited(iByPlayer));
  MessageWindow(GetDesc(),iByPlayer);
  return(1);
}

public func RejectResearch()
{
  if(FindObject(MT4V)) return(0);
  return(_inherited(...));
}

global func GetClanMystic(int iClan)
{
  var iClanMystic, pObj, iMystic, i;
  for(i = 0; i < 3; i++) {
    while(pObj = FindObjectClan([TTPL,SKLT,LHPT][i],iClan,0,0,0,0,0,"Mystic",0,pObj))
      iMystic += pObj->~GetMystic();
    iClanMystic += Min(iMystic,[50,25,25][i]);
    iMystic = 0;
  }
  return(iClanMystic);
}