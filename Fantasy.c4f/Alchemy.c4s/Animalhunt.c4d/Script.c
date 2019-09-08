/*-- Tierjagd --*/

#strict
#include GOAL

// ganz gemein von Monsterjagd kopiert
    
public func IsFulfilled()
{
  var obj;
  while(obj = FindObject(0,0,0,0,0,0,0,0,0,obj)) {
    if(GetCategory(obj) & 4096 && GetOCF(obj) & OCF_Alive) return(false);
    if(GetCategory(obj) & 8192) return(false);
  }
  return(true);
}

protected func Activate(iPlayer)
{
  var iAnimals=0, iEggs=0, obj;
  // Erfüllungsstatus ermitteln
  while(obj = FindObject(0,0,0,0,0,0,0,0,0,obj)) {
    if(GetCategory(obj) & 4096 && GetOCF(obj) & OCF_Alive) ++iAnimals;
    if(GetCategory(obj) & 8192) ++iEggs;
  }
  // Nachricht nach Erfüllung ausgeben
  return(MessageWindow(GetFulfillText(iAnimals, iEggs), iPlayer));
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
