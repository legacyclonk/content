/*-- Zauberturm --*/

#strict
#appendto WZKP

// Aufstieg mit ControlUp
public func ContainedUp(pClonk)
{
  Ascend(pClonk);
  return(1);
}

/* Aufstieg ins Zauber-Menu */

public func SelectSpell(object caller)
{
  // Nicht bereit
  if (GetAction() ne "Ready") return(0);
  // Alchemieregel vorhanden?
  var iExtra;
  if (ObjectCount(ALCO)) 
    { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  
    { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  // Menü erzeugen und mit den Zaubersprüchen des Turmbesitzers füllen
  CreateMenu(MCMS, caller, this(), iExtra, "$TxtNoSpells$", GetMagicEnergy());  
  if(caller->Contained() == this() )
    AddMenuItem("$TxtAscend$", "Ascend", MASC, caller, 0, caller);
  var spell_num;
  var spell_id;
  var iPlr = GetOwner(); if (!GetPlayerName(iPlr)) iPlr = GetOwner(caller);
  while (spell_id = GetPlrMagic(iPlr, 0, spell_num++)) 
    if(!spell_id->~NoTowerMagic() )
      AddMenuItem("$TxtDoMagic$", "DoMagic", spell_id, caller, 0, caller);
  return(1);
}
