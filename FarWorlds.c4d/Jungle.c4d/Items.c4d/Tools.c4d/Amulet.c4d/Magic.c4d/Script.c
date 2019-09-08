/*-- Magieamulett --*/

#strict

local iSelection;

public func IsAmulet() { return(1); }

/* Initialisierung */

public func Initialize() { SetAction("Be"); iSelection=0; }

/* aktivieren: Zaubern */

public func Activate(object pClonk) {
  [$TxtMagic$]
  // nicht bereit
  if(!(pClonk->~IsWeaponReady())) return();
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),pClonk);
  if(ObjectCount(NMGE)) iExtra=0;
  // in richtige Richtung drehen
  SetDir(GetDir(pClonk));
  // Menü öffnen
  var iExtra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  CreateMenu(MCMS,pClonk,this(),iExtra,"$MnuNoSpells$",GetMagicEnergy(pClonk));
  var idMagic, i = 0;
  while (idMagic = GetPlrMagic(GetOwner(pClonk), 0, i++)) 
    AddMenuItem("$TxtMagic$: %s", "DoMagic", idMagic,pClonk,0,pClonk);
  // Letzten Spruch auswählen
  SelectMenuItem(iSelection,pClonk);
  return(1);
}

protected func OnMenuSelection(int iMenuIndex)
{
  // Nur Zaubermenü: Menüindex speichern (Für Wiederauswahl beim Schließen und Öffnen des Menüs)
  if(GetMenu(Contained())==MCMS) iSelection = iMenuIndex;
}

protected func DoMagic(id idSpell,object pClonk)
{

  // Nicht genug Zauberenergie
  if(!CheckMana(idSpell,pClonk))
  {
    Sound("Error");
    Message("$MsgNotEnoughEnergy$",this());
    return();
  }

  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(idSpell,pClonk))
  {
    Sound("Error");
    Message("$MsgNotEnoughIngredients$",this(),GetNeededAlcStr(idSpell,pClonk));
    return();
  }
  
  // Zauberobjekt erschaffen und aktivieren
  if (!CreateObject(idSpell,0,0,GetOwner())->~Activate(this(),pClonk))
    // Zauber fehlgeschlagen
    return(0);

  // Zauberaktion durchführen
  if(pClonk->~IsRiding()) ObjectSetAction(pClonk,"RideMagic");
  else  ObjectSetAction(pClonk,"Magic");

  // Zauberenergie abziehen
  DoMagicEnergy(-Value(idSpell),pClonk);

  // alchemische Zutaten abziehen
  if(ObjectCount(ALCO)) ReduceAlchem(idSpell,pClonk);

  // Fertig
  return(1);
}

/* Timer */

protected func GainEnergy() { DoMagicEnergy(+1,Contained()); }