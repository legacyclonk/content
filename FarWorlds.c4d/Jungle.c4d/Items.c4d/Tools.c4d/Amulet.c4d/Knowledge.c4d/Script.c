/*-- Wissensamulett --*/

#strict

public func IsAmulet() { return(1); }

/* aktivieren: Forschen */

public func Activate(object pClonk) {
  [$TxtResearch$]
  // nicht bereit
  if(!(pClonk->~IsWeaponReady())) return();
  // kann nur vom Urwaldclonk benutzt werden (wegen der Meditationsanimation)
  if(GetID(pClonk)==JCLK) ResearchMenu(pClonk);
  else { Message("$TxtNoJCLK$",this()); Sound("Error"); }
  return(1);
}


/* Forschung */
// nur kleine Änderungen von dem Script aus der Forschungsstation

protected func ResearchObjects(pClonk)    { return(SelectResearchCategory(pClonk, C4D_Object())); }
protected func ResearchVehicles(pClonk)   { return(SelectResearchCategory(pClonk, C4D_Vehicle())); }
protected func ResearchStructures(pClonk) { return(SelectResearchCategory(pClonk, C4D_Structure())); }

protected func ResearchMenu(object pClonk)
{
  // Auswahlmenü erzeugen
  CreateMenu(RSR1,pClonk,this(),0,"$TxtNoResearch$");
  ResearchObjects(pClonk);
  ResearchVehicles(pClonk);
  ResearchStructures(pClonk);
}

protected func SelectResearchCategory(pClonk, dwCategory)
  {
  // Alle geladenen Baupläne überprüfen
  var i,idDef,iPlr=GetOwner(pClonk), link;;
  while (idDef = GetDefinition(i++, dwCategory))
    // Passende Kategorie
    if (GetCategory(0, idDef) & C4D_Knowledge())
      // Dem Spieler noch nicht bekannt
      if (!GetPlrKnowledge(iPlr,idDef))
        // eine Dschungelforschung :)
        if(DefinitionCall(idDef, "IsJungleResearch"))
        {
          // Keine Forschungsgrundlage nötig oder Forschungsgrundlage vorhanden
          var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
          fHasBase = !idBase || GetPlrKnowledge(iPlr,idBase); j=1;
          // Erweiterte Forschungsgrundlagen
          while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", ++j)))
            if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=0;
          // Ist schon durch etwas anderes erforschbar? (Zugbrücke rechts -> Zugbrücke links etc.)
          if (link = DefinitionCall(idDef, "GetResearchLink"))
            if (GetName(0, link)) // Linkdefinition auch tatsächlich geladen?
              continue;
          // Menüeintrag hinzufügen
          if(fHasBase) AddMenuItem( "$TxtResearchs$", "SelectResearch", idDef, pClonk, 0, pClonk );
        }
  return(1); 
  }

public func SelectResearch(id idDef, object pClonk)
  {
  // Gesamtzeit setzen (durchschnittlich 20 Sekunden)
  var iDelay = DefinitionCall(idDef, "GetResearchTime", this(), pClonk) / 3;
  if(!iDelay) iDelay = 33;
  // Forschung starten
  AddEffect("AmuletResearch",pClonk,1,30,0,GetID(),iDelay,idDef);
  }

protected func FxAmuletResearchStart(object pClonk, int iEffectNumber, int iTemp, int iDelay, id idDef)
{
  // temporäre Aufrufe ignorieren
  if(iTemp) return();
  // Forschdauer speichern
  EffectVar(0,pClonk,iEffectNumber)=iDelay;
  // speichern, was geforscht wird
  EffectVar(1,pClonk,iEffectNumber)=idDef;
  // Clonk muss meditieren
  ObjectSetAction(pClonk,"Meditate");
}

protected func FxAmuletResearchTimer(object pClonk, int iEffectNumber, int iEffectTime)
{
  // Zeit abgelaufen?
  if(iEffectTime >= 30*EffectVar(0,pClonk,iEffectNumber))
  {
    // jetzt kriegst du die Tech! :DDDdd
    SetPlrKnowledge(GetOwner(pClonk),EffectVar(1,pClonk,iEffectNumber));
    // Gelinkte Forschungsziele sind automatisch auch erforscht
    for(var i,def,link; def=GetDefinition(i); ++i) 
    {
      if(link = DefinitionCall(def, "GetResearchLink"))
        if(link == EffectVar(1,pClonk,iEffectNumber))
          SetPlrKnowledge(GetOwner(pClonk), def);
    }
    // Nachricht
    Message("$Txtsdeveloped$",pClonk,GetName(0,EffectVar(1,pClonk,iEffectNumber)));
    // Sound
    Sound("Magic1");
    // weg mit dem Effekt
    return(-1);
  }
  // meditiert nicht mehr? (Fling, kämpft mit Clonk usw) -> abbrechen
  if(GetAction(pClonk) ne "Meditate") return(-1);
}

protected func FxAmuletResearchDamage(object pClonk, int iEffectNumber, int iDmgEngy, int iCause)
{
  // Bei Damage aufwachen
  RemoveEffect(0,pClonk,iEffectNumber);
  return(iDmgEngy);
}

protected func FxAmuletResearchStop(object pClonk, int iEffectNumber)
{
  // wieder stehen
  ObjectSetAction(pClonk,"Walk");
  // Fertig
  return(1);
}