/*-- Monsterjagd --*/

#strict
#include GOAL

protected func Initialize()
{
  var iCount;
  // Keine Monster da?
  if (Not(ObjectCount(MONS)))
    if (Not(ObjectCount(FMNS)))
      // Welche erzeugen
      iCount = Random(5) + 2;
  while (iCount-- > 0) 
    PlaceAnimal(MONS);
  return(inherited());  
}
    
public func IsFulfilled()
{
  return(!FindObject(MONS) && !FindObject(MEGG) && !FindObject(FMNS) && !FindObject(FMEG));
}

protected func Activate(iPlayer)
{
  // Erfüllungsstatus ermitteln
  var iMonsters = ObjectCount(MONS) + ObjectCount(FMNS); 
  var iEggs = ObjectCount(MEGG) + ObjectCount(FMEG);
  // Nachricht nach Erfüllung ausgeben
  return(MessageWindow(GetFulfillText(iMonsters, iEggs), iPlayer));
}

private func GetFulfillText(iMonsters, iEggs)
{
  // alle Monster tot?
  if (!iMonsters)
    {
    // alle Eier verkauft
    if (!iEggs) return("$MsgMonstersKilled$");
    // nur noch ein Ei
    if (iEggs == 1) return("$MsgEggLeft$");
    // mehrere Eier
    return(Format("$MsgEggsLeft$", iEggs));
    }
  // alle Eier verkauft?
  if (!iEggs)
    {
    // nur noch ein Monster?
    if (iMonsters == 1) return("$MsgMonsterLeft$");
    // mehrere Monster
    return(Format("$MsgMonstersLeft$", iMonsters));
    }
  // nur noch ein Ei?
  if (iEggs == 1) 
    return(Format("$MsgEggMonstersLeft$", iMonsters));
  // Standardnachricht
  return(Format("$MsgEggsMonstersLeft$", iMonsters, iEggs));
}
