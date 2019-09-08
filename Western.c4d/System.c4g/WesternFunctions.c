/*-- Western Funktionen --*/

#strict 2

/*
Bisher sind folgende Funktionen vorhanden:
  int    GetClanMystic(int iClan)
  object FindPullingHorse(object pHorse)

  array  Find_Clan(int iClan)
*/

// NTIP
global func GetClanMystic(int iClan)
{
  var iMystic, pObj;
  for(pObj in FindObjects(Find_Clan(iClan),Find_Action("Mystic"))) iMystic += pObj->~GetMystic();
  return(iMystic);
}

global func FindPullingHorse(object pHorse)
{
  var szAction;
  while(pHorse = FindObject(0,-100,-100,200,200,OCF_Grab,0,0,NoContainer(),pHorse)) {
    szAction = GetAction(pHorse);
    if(szAction == "Pull"  || szAction == "Pull2"  || szAction == "Pull3"  ||
       szAction == "Scale" || szAction == "Scale2" || szAction == "Scale3" ||
       szAction == "Turn"  || szAction == "Jump"   || szAction == "Rise")
      if(GetActionTarget(0,pHorse) == this())
        return(pHorse); 
  }
  return(0);
}

/*-- Find_*-Funktionen --*/

global func Find_Clan(int iClan) { return([C4FO_Func,"Find_FuncEqualCheck","GetClan()",iClan]); }