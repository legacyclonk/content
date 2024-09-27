/*-- Console Control --*/

#strict
#appendto CONS

func Initialize() {
    target =CreateObject(_HN2,-240,35,-1);
    CreateObject(HNG2,-490,35,-1)->Open();
    return(_inherited());
}

protected func ControlDig(pClonk)
{
  
  [$Control$]
  if(smashed || GetEffect("EMPShock", this()))
    {
    Sound("Electric");
    Fling(pClonk, RandomX(-2,2), -2);
    return();
    }
    
  if(console1_busy) return(0);
  if(!target) return(Sound("CatapultSet"));
  
  CreateMenu(GetID(target), pClonk, target, 0, Format("$Control$: %s", GetName(target)), 0, 1);
  for(var i = 1, desc ; desc = target ->~ConsoleControl(i) ; i++)
  {
    //Aufrufenden Clonk und uns selbst dem Zielobjekt übergeben. Dies kann es in ConsoleController abrufen
//    target->~ConsoleController(this(), pClonk);
    // Zielobjekt muss für ConsoleControl(i) jeweils den Namen der Aktion zurückgeben, wichtig: i beginnt bei 1
    AddMenuItem(desc, Format("ConsoleControlled(%d, %d, %d)", i, ObjectNumber(pClonk), ObjectNumber(this())), GetID(), pClonk, 0, 0, "$Control$");
    // Zielobjekt bekommt Aufruf: ConsoleControll(i) und muss das dann verarbeiten
  }
  return(1);
}
