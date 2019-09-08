/*-- Amulett --*/

#strict

private func UpgradeNeedsComponents() { return(false); }

/* aktivieren */

public func Activate(object pClonk) {
  [$TxtSelect$]
  var iExtra=0;
  if(UpgradeNeedsComponents()) iExtra=1;
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),pClonk);
  // Menü öffnen
  CreateMenu(AMUL,pClonk,this(),iExtra);
  // Amulette zum aufwerten anbieten
  for(var i=0,idAmulet;idAmulet=GetDefinition(i,C4D_Object());++i)
    if(DefinitionCall(idAmulet,"IsAmulet"))
      AddMenuItem(Format("$TxtUpgrade$ %s",GetName(0,idAmulet)),"Upgrade",idAmulet,pClonk,0,pClonk);
  return(1);
}

protected func Upgrade(id idAmulet,object pClonk)
{
  var pObj;
  // Amulett benötigt Komponente
  // Achtung, nur die erste Komponente vom Amulett wird untersucht
  if(UpgradeNeedsComponents())
    // Objektkomponente nicht vorhanden? abbrechen.
    if(!( pObj = FindContents(GetComponent(0,0,0,idAmulet),pClonk)))
    {
      Message("$UpgradeError$",this());
      Sound("Error");
      return(0);
    }
    else { RemoveObject(pObj); }

  // Zauberaktion durchführen
  if (pClonk->~IsRiding()) ObjectSetAction(pClonk,"RideMagic");
  else ObjectSetAction(pClonk,"Magic");
  Sound("Magic1");

  // Amulett upgraden
  ChangeDef(idAmulet);
  this()->~Initialize();
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }
