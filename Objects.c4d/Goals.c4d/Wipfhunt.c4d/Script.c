/*-- Wipfjagd --*/

#strict
#include OREM

// Anzahl Wipfe die Sterben dürfen
local AllowedDeaths;
// Anzahl Toter Wipfe
local DeadWipfs;

func Initialize() {
  
  // 1x WPHT = 0 AllowedDeaths
  AllowedDeaths = 0;
  
  var pGoal;
  if (pGoal = FindObject(WPHT)) 
  {
    // 2x WPHT = 2 AllowedDeaths
    if (LocalN("AllowedDeaths", pGoal) == 0) 
      LocalN("AllowedDeaths", pGoal) = 1;
    // 3x WPHT = 3 AllowedDeaths usw.  
    ++LocalN("AllowedDeaths", pGoal);    
    // Nicht genug Wipfe da?
    var i = LocalN("AllowedDeaths", pGoal) - ObjectCount(WIPF) + 1 + Random(3);
    // Noch welche erzeugen
    while (i > 0)
    {
      PlaceAnimal(WIPF);
      i--;
    }    
    // Kontrolle an das erste Spielzielobjekt übergeben
    return(RemoveObject());
  }  
       
  return(inherited());
}  
    
public func IsFulfilled() 
{
  //Ob es wohl besser wäre, wenn das Wipf seinen Tod per Scriptfunktion an das Spielziel meldete?
  var pWipf;
  while (pWipf = FindObject(DWPF,0,0,0,0,OCF_Fullcon(),0,0,0,pWipf))
    if (!Local(0, pWipf)) 
    {
      SetLocal(0, 1, pWipf);
      ++DeadWipfs;
    }

  if (DeadWipfs > AllowedDeaths) 
  {
    var i = GetPlayerCount(); 
    while (i--) 
      EliminatePlayer(GetPlayerByIndex(i));
    return (0);
  }
  return(!FindObject(WIPF));
}

public func SellID() { return (WIPF); }

protected func Activate(plr) 
{
  if (DeadWipfs>AllowedDeaths) return(MessageWindow(Format("$MsgFailed$"),plr));
  if (IsFulfilled()) return(MessageWindow(Format("$MsgSuccess$"),plr));
  MessageWindow(Format("$MsgTodo$",ObjectCount(WIPF),DeadWipfs,AllowedDeaths),plr);
}
