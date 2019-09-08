/*-- Forschungsamulett --*/

#strict 2

#include AMBR

// Men¸ zum Ablegen
public func AttachContextUnWear(object pCaller, object pClonk)
{
  [$TxtUnWear$|Image=AMKL|Condition=CheckCaller]
  return(_inherited(pCaller,pClonk));
}

// Dieses Amulett beinhaltet keine Zauber
public func GetSpell(int iIndex) { return(0); }

// Hiermit kann man weder zaubern noch Knochen zerkleinern
public func AmuletCanMagic() { return(0); }

// Dieses Amulett eignet sich zum forschen
public func AttachContextUse(object pClonk, object pObj, int iSelection)
{
  [$TxtResearch$|Image=RSR1|Condition=CheckCaller]
  // kann nur mit Meditationsanimation benutzt werden
  if(!GetActMapVal("Length","Meditate",GetID(pClonk))) {
    Message("$TxtCantUse$",this());
    Sound("Error",false,pClonk,100);
    return(0);
  }
  return(ResearchMenu(pClonk));
}

/* Forschung */

protected func ResearchObjects(pClonk)    { return(SelectResearchCategory(pClonk, C4D_Object)); }
protected func ResearchVehicles(pClonk)   { return(SelectResearchCategory(pClonk, C4D_Vehicle)); }
protected func ResearchStructures(pClonk) { return(SelectResearchCategory(pClonk, C4D_Structure)); }

protected func ResearchMenu(object pClonk)
{
  // Clonk soll anhalten
  SetComDir(COMD_Stop,pClonk);
  // Auswahlmen¸ erzeugen
  CreateMenu(RSR1,pClonk,this(),0,"$TxtNoResearch$");
  // Forschungsmˆglichkeiten aufdecken 
  ResearchObjects(pClonk);
  ResearchVehicles(pClonk);
  ResearchStructures(pClonk);
}

protected func SelectResearchCategory(pClonk, dwCategory)
{
  // Alle geladenen Baupl‰ne ¸berpr¸fen
  var i, idDef;
  var fHasBase, idBase, j;
  var iPlr = GetOwner(pClonk);
  for(i = 0; idDef = GetDefinition(i,dwCategory); i++) {
    // Kann erforscht werden
    if(!(GetCategory(0,idDef) & C4D_Knowledge)) continue;
    // Dem Spieler noch nicht bekannt
    if(GetPlrKnowledge(iPlr,idDef)) continue;
    // eine Indianerforschung
    if(!DefinitionCall(idDef,"IsIndianResearch")) continue;
    // Keine Forschungsgrundlage nˆtig oder Forschungsgrundlage vorhanden
    if(idBase = DefinitionCall(idDef,"GetResearchBase"))
      if(!GetPlrKnowledge(iPlr,idBase)) continue;
    // Erweiterte Forschungsgrundlagen
    fHasBase = 1;
    for(j = 2; idBase = DefinitionCall(idDef,Format("GetResearchBase%d",j)); j++) {
      if(GetPlrKnowledge(iPlr,idBase)) continue;
      fHasBase = 0;
      break;
    }
    if(!fHasBase) continue;
    // Ist schon durch etwas anderes erforschbar? (Zugbr¸cke rechts -> Zugbr¸cke links etc.)
    if(idBase = DefinitionCall(idDef,"GetResearchLink"))
      if(GetName(0,idBase)) // Linkdefinition auch tats‰chlich geladen?
        continue;
    // Blockierte Erforschung
    if(DefinitionCall(idDef,"RejectResearch",pClonk)) continue;
    // Alle anforderungen erf¸llt: Men¸eintrag hinzuf¸gen
    AddMenuItem("$TxtResearchs$","SelectResearch",idDef,pClonk,0,pClonk);
  }
  return(1); 
}

public func SelectResearch(id idDef, object pClonk)
{
  // Gesamtzeit setzen (durchschnittlich 20 Sekunden)
  var iDelay = DefinitionCall(idDef,"GetResearchTime",this(),pClonk) / 3;
  if(!iDelay) iDelay = 33;
  // Forschung starten
  AddEffect("AmuletResearch",pClonk,1,30,0,GetID(),iDelay,idDef,this());
}

protected func FxAmuletResearchStart(object pClonk, int iEffectNumber, int iTemp, int iDelay, id idDef, object pAmulet)
{
  // Tempor‰re Aufrufe ignorieren
  if(iTemp) return(0);
  // Steht der Clonk gerade auch?
  if(!WildcardMatch(GetAction(pClonk), "*Walk")) return(-1);
  // Parameter speichern
  EffectVar(0,pClonk,iEffectNumber) = Max(3,iDelay);	// Forschdauer
  EffectVar(1,pClonk,iEffectNumber) = idDef;		// Was geforscht wird
  EffectVar(2,pClonk,iEffectNumber) = pAmulet;		// Amulett
  // Clonk muss meditieren
  ObjectSetAction(pClonk,"Meditate");
}

protected func FxAmuletResearchTimer(object pClonk, int iEffectNumber, int iEffectTime)
{
  // Zeit abgelaufen?
  if(!EffectVar(0,pClonk,iEffectNumber)) {
    var idResearched = EffectVar(1,pClonk,iEffectNumber);
    // Geforschtes ¸berreichen
    SetPlrKnowledge(GetOwner(pClonk),idResearched);
    // Gelinkte Forschungsziele sind automatisch auch erforscht
    var i, idDef;
    for(i = 0; idDef = GetDefinition(i); i++)
      if(DefinitionCall(idDef, "GetResearchLink") == idResearched)
        SetPlrKnowledge(GetOwner(pClonk), idDef);
    // Nachricht & Ger‰usch
    Message("$TxtDeveloped$",pClonk,GetName(0,idResearched));
    Sound("Magic1",false,pClonk,100);
    // Weg mit dem Effekt
    return(-1);
  }
  // Zeit ablaufen lassen
  EffectVar(0,pClonk,iEffectNumber)--;
//  if(!DoMagicEnergy(-1, pClonk)) return(-1, Message("$MsgNoMoreMana$", pClonk));
  // meditiert nicht mehr? (Fling, k‰mpft mit Clonk usw) -> abbrechen
  if(GetAction(pClonk) != "Meditate") return(-1);
}

protected func FxAmuletResearchStop(object pClonk, int iEffectNumber)
{
  // wieder stehen
  ObjectSetAction(pClonk,"Walk");
  // Fertig
  return(1);
}

/* Verbesserungseffekt */

protected func FxWearBlessStart(object pTarget, int iEffectNumber, int iTemp)
{
  // Tempor‰re Aufrufe ignorieren
  if(iTemp) return(0);
  // Auﬂer Forschung keine Verbesserungen vorhanden
  return(1);
}

protected func FxWearBlessStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  // Bei Tod Amulett verlieren
  var pAmulet = FindObject(0,0,0,0,0,0,"WearAmulet",pTarget);
  if(pAmulet && iReason==4) ObjectSetAction(pAmulet,"Idle");
  SetCategory(C4D_Object,pAmulet);
  return(1);
}