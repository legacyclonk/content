/*-- Zauberturm --*/

#strict

#include DOOR
#include BAS2

// Initialisierung
protected func Initialize() 
{
  SetAction("Ready");
  // Nach rechts ausgerichtet, wenn in der linken Landschaftshälfte
  if (GetX() < LandscapeWidth()/2) 
    SetDir(DIR_Right());
  return(1);
}

// Beim Einsammeln von Objekten
protected func Collection2() 
{
  // Inhalt prüfen
  ContentsCheck();
  return(1);
}

// Inhalt prüfen: Kristalle verarbeiten
protected func ContentsCheck() 
{
  if (FindContents(CRYS)) 
    ConvertCrystal();
  return(1);
}

/* Energiegewinnung */

private func ConvertCrystal() 
{
  if (!IsConvertReady()) return(0);
  var crystal = FindContents(CRYS);
  if (!crystal) return(0);
  
  if (!crystal->~Apply(this()) )
    if(DoMagicEnergy(+10) )
      RemoveObject(crystal);
    else
      return(0);

  SetAction("Energize");
  Sound("Electric");
  return(1);
}

public func LightningStrike() 
{
  // Nur wenn fertig gebaut
  if (GetCon() < 100) return(0);
  // Energiegewinnung
  for(var i = 0; i < 10; ++ i)
    DoMagicEnergy(1);
  // Sound
  Sound("Electric");
  // Effekt-Aktion
  if (GetAction() ne "Ready") return(0);
  SetAction("Energize");
  return(1);
}

protected func RefillMagic() {  // alle 100 Frames
  // Nur wenn fertig gebaut
  if (GetCon() < 100) return(0);
  DoMagicEnergy(+1);
}

/* Zaubern */

public func DoMagicCall(object caster, id spell_id, int direction)
{
  DoMagic(spell_id, caster, direction);
}
 	 
protected func DoMagic(id spell_id, object caster, int direction)
{
  if (!caster) return();

  // Zaubernden Clonk in den Turm bewegen, wenn er nicht drin ist
  if (caster->Contained() != this())
  {
    SetCommand(caster, "None");
    AddCommand(caster, "Call", this(), spell_id, direction, 0,0, "DoMagicCall");
    AddCommand(caster, "Enter", this());
    return(1);
  }

  // Nicht bereit: etwas warten, dann erneut versuchen
  if (GetAction() ne "Ready")
  {
    SetCommand(caster, "None");
    AddCommand(caster, "Call", this(), spell_id, direction, 0,0, "DoMagicCall");
    AddCommand(caster, "Wait", 0,0,0,0,0, 10);
    return(1);
  }

  // Richtung setzen
  if (direction > 0)
    AimRight(caster);
  else
    AimLeft(caster);
  
  // Nicht genug Zauberenergie
  if (!CheckMana(spell_id))
  {
    Sound("Error");
    Message("$TxtNotEnoughEnergy$", this());
    return();
  }

  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(spell_id, this(),caster))
  {
    Sound("Error");
    Message("$TxtNotEnoughIngredients$",this(),GetNeededAlcStr(spell_id,this(),caster));
    return();
  }
  
  // Zauberobjekt erschaffen und aktivieren
  if (CreateObject(spell_id, 0, 0, GetController(caster))->~Activate(this(), caster)) 
  {
    // Energie abziehen
    DoMagicEnergy(-Value(spell_id));

    // alchemische Zutaten abziehen
    if(ObjectCount(ALCO)) ReduceAlchem(spell_id,this(),caster);

    // Zauberkrafttraining sowie szenarienspezifische Aufrufe
    OnClonkSucceededSpell(spell_id, caster);
      
    // Aktion setzen
    SetAction("Magic");
  }
  // Zauberobjekterschaffung fehlgeschlagen
  else 
  {
    SetAction("Ready");
    Sound("Error");
    return(0);
  }
  
  // Fertig
  return(1);
}

public func SpellFailed(id idSpell, object pByClonk)
{
  // Zauber abgebrochen:
  // Magieenergie zurückgeben
  DoMagicEnergy(Value(idSpell));
  // Alchemische Zutaten könnten aus dem Clonk oder dem Turm stammen: Dem Turm geben, wenn möglich
  if(ObjectCount(ALCO))
    if (GetAlchemBag()) IncreaseAlchem(idSpell); else IncreaseAlchem(idSpell, pByClonk);
  // k
  return(true);
}

/* Zaubern */

public func SelectSpell(object caller, int direction)
{
  // Forschung abbrechen
  CancelResearch();
  // Menü erzeugen und mit den Zaubersprüchen des Turmbesitzers füllen
  // Gehört der Turm keinem gülötigen Besitzer, stattdessen Zauber des Clonks nehmen
  OpenEmptySpellMenu(MCMS, caller, this, this, "$TxtNoSpells$");
  var spell_num;
  var spell_id;
  var iPlr = GetOwner(); if (!GetPlayerName(iPlr)) iPlr = GetOwner(caller);
  while (spell_id = GetPlrMagic(iPlr, 0, spell_num++)) 
    if(!spell_id->~NoTowerMagic() )
      {
      //AddMenuItem("$TxtDoMagic$", "DoMagic", spell_id, caller, 0, caller);
      var strCommand = Format("Object(%d)->DoMagic(%i,Object(%d),%d)", ObjectNumber(this()), spell_id, ObjectNumber(caller), direction);
      AddMenuItem("$TxtDoMagic$", strCommand, spell_id, caller, 0, caller);
      }
  // Zauber erforschbar
  if (ObjectCount(WTRS) && (caller->Contained() == this())) 
  {
    // Neue Forschung starten
    AddMenuItem("$TxtResearchs$", "ResearchMenu", RSR6, caller, 0, caller);
    // Laufende Forschung fortsetzen
    if (target) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", target, caller, 0, caller);
  }
  return(1);
}

public func AimLeft() 
{
  if (GetDir() == DIR_Left()) return(0);
  SetDir(DIR_Left());
  Sound("Lever1");
  return(1);
}

public func AimRight() 
{
  if (GetDir() == DIR_Right()) return(0);
  SetDir(DIR_Right());
  Sound("Lever1");
  return(1);
}

/* Türsteuerung */

protected func ActivateEntrance() 
{
  if (GetAction() ne "Ready") return(1);
  SetAction("OpenDoor");  
  return(1);
}

private func MagicCloseEntrance() 
{
  SetEntrance(0);
  return(1);
}

/* Steuerung */

public func ContainedLeft(object pCaller) 
{
  [$TxtAimspellsleft$]
  CancelResearch();
  AimLeft(pCaller);
  SelectSpell(pCaller, 0);
  return(1);
}

public func ContainedRight(object pCaller) 
{
  [$TxtAimspellsright$]
  CancelResearch();
  AimRight(pCaller);
  SelectSpell(pCaller, 1);
  return(1);
}
  
public func ContainedDown(object pClonk) 
{
  [$TxtCancelresearch$|Image=RSR2]
  CancelResearch();
  return(0);
}

/* Status */

public func IsConvertReady() { return(GetAction()S="Ready"); }

/* Forschen */

local target;
local progress;

local restime; // Benötigte Gesamtzeit zum Forschen

protected func ResearchMenu(idImage, pClonk) 
{
  // Auswahlmenü erzeugen
  CreateMenu(idImage, pClonk, this(), 0, "$TxtNothingLeft$");
  // Alle geladenen Sprüche überprüfen
  var i,idDef,iPlr=GetOwner(pClonk);
  while (idDef = GetDefinition(i++, C4D_Magic()))
    // Dem Spieler noch nicht bekannt
    if (!(GetPlrMagic(iPlr,idDef))) {
      // Keine Forschungsgrundlage nötig oder Forschungsgrundlage vorhanden
      var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
      fHasBase = !idBase || GetPlrMagic(iPlr,idBase); j=1;
      // Erweiterte Forschungsgrundlagen
      while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", ++j)))
        if (!GetPlrMagic(iPlr,idBase)) fHasBase=0;
      // Ist schon durch etwas anderes erforschbar? (Zugbrücke rechts -> Zugbrücke links etc.)
      var link;
      if (link = DefinitionCall(idDef, "GetResearchLink"))
        if (GetName(0, link)) // Linkdefinition auch tatsächlich geladen?
          if(!GetPlrKnowledge(iPlr, link) ) // Linkdefinition noch nicht erforscht? (passiert durch SetPlrKnowledge-Aufrufe oder Szenariovorgaben)
            continue;
      // Menüeintrag hinzufügen
      if (fHasBase) AddMenuItem("$TxtResearchs$", "SelectResearch", idDef, pClonk, 0, pClonk);
    }
  return(1);  
}

public func SelectResearch(def, clonk)
{
  // Anfangen
  AddCommand(clonk, "Call", this(), def, 0, 0, 0, "StartResearch", 10, 1);
  // Vorher ins Labor gehen
  if (Contained(clonk) != this())
  {
    AddCommand(clonk, "Wait", 0, 0, 0, 0, 40);
    AddCommand(clonk, "Enter", this());
  } 
}
  
private func CancelResearch()
{
  if (WildcardMatch(GetAction(), "Research*")) 
    SetAction("Ready");
}

protected func StartResearch(clonk, def)
{
  // Spieler hat diesen Bauplan schon
  if (GetPlrMagic(clonk->GetOwner(), def))
  {
    Message("$TxtAlreadyDeveloped$", this(), GetName(0, def));
    Sound("Error");
    target = 0;
    return 0; 
  }
  // Bisherige Forschung abbrechen
  CancelResearch();
  // nur, wenn ansonsten inaktiv oder schon am Forschen
  if (GetAction() ne "Ready") return(0);
  // Besitzer anpassen (der Besitzer des forschenden Clonks erhält den Bauplan)
  SetOwner(GetOwner(clonk));
  // Neues Forschungsziel: Fortschritt zurücksetzen
  if (def != target) progress = 0;
  // Forschungsziel und Gesamtzeit setzen
  restime = DefinitionCall(target=def, "GetResearchTime", this(), clonk);
  // Forschungsaktion starten
  SetAction("Research");
  return (1);
}

private func Researching()
{
  // Kein Ziel
  if (!target) return(0);
  // Effekte TODO: Sparks
  //if (Random(3)) Smoke(5+8*Random(2),-23,3+Random(8));
  // Fortschritt
  var Process = Min(++progress*4 / restime, 3);
  Process = Format("Research%d", Process);
  if (Process ne GetAction()) SetAction(Process);
  // Forschung abbrechen, wenn irgendein Clonk im Inneren das Gebäude verlassen möchte
  var pObj;
  while (pObj = FindObject(0, 0,0,0,0, OCF_CrewMember, 0,0, this(), pObj))
    if (pObj->GetCommand() eq "Exit")
      return(CancelResearch());    
  // Weitermachen
  if (!restime) restime=100;
  if (progress < restime) return(1);
  // Forschung abgeschlossen
  Sound("ResearchDone");
  // Dem Spieler den neuen Bauplan verpassen
  SetPlrMagic(GetOwner(), target);  
  // Alle Clonks suchen, die ein Menü für dieses Labor geöffnet haben - noch nicht implementierbar, da kein GetMenuTarget()...
  /*var pClonk;
  while (pClonk = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, 0, pClonk))
    if ((pClonk->GetMenu() == RSR1) && (pClonk->GetMenuTarget() == this())
      // Menü neu öffnen, damit Forschung-Fortsetzen-Einträge aktualisiert werden
      ResearchMenu(pClonk);*/  
  // Gelinkte Forschungsziele sind automatisch auch erforscht
  // (Linke Zugbrücke -> Rechte Zugbrücke etc.)
  var def_num, def;
  while (def = GetDefinition(def_num++)) 
  {
    var link;
    if (link = DefinitionCall(def, "GetResearchLink"))
      if (link == target)
        SetPlrMagic(GetOwner(), def);
  }
  // Nachricht ausgeben
  Message("$Txtsdeveloped$", this(), GetName(0, target));
  // Forschung beenden
  CancelResearch();
  target = 0;
  // Fertig
  return(1);  
}

/* Kontext-Zaubern */

public func ContextResearch(clonk) 
{
  [$TxtResearch$|Image=RSR6|Condition=CanResearch]
  return (ResearchMenu (RSR6, clonk));
}

public func ContextMagicLeft(object caller)
{
  [$TxtAimspellsleft$|Image=MCMS|Condition=CanMagic]
  SelectSpell(caller, 0);
}

public func ContextMagicRight(object caller)
{
  [$TxtAimspellsright$|Image=MCMS|Condition=CanMagic]
  SelectSpell(caller, 1);
}

/* Status */

protected func CanResearch() 
{
  return (ObjectCount(WTRS) && GetCon () >= 100);
}

public func CanMagic(object caller)
{
  var iPlr = GetOwner(); 
  if (!GetPlayerName(iPlr)) iPlr = caller->GetOwner();
  return(GetPlrMagic(caller->GetOwner()) != 0);
}
