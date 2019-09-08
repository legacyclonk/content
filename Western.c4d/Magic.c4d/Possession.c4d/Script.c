/*-- Besitznahme --*/

#strict 2

public func PossessionTime() { return(35 * 60 * 5); } // Dauer des Zaubers in Frames (5 Minuten)

/* Tierauswahl */

public func Activate(object pCaster, object pRealCaster)
{
  var pClonk, pObj, aList;
  pClonk = pCaster;
  if(pRealCaster) pClonk = pRealCaster;

  // Dieser Zauber kann nur im Freiem ausgeführt werden
  if(Contained(pClonk)) {
    NoPossession(pClonk,Format("$NotInFree$",GetName(pClonk)));
    return(0);
  }

  // Es kann nur gezaubert werden, wenn der Clonk steht
  if(GetAction(pClonk) != "Walk" && GetAction(pClonk) != "SpearWalk" && GetAction(pClonk) != "Magic") {
    NoPossession(pClonk,Format("$CanNotMagic$",GetName(pClonk)));
    return(0);
  }

  // Per Selektor auswählen, wenn möglich
  if(pClonk->~DoSpellSelect(this(), 250)) return(1);

  // Ansonsten automatisch das nächstliegende zu besetzende Lebewesen suchen
  aList = FindObjects(Sort_Distance(),Find_Distance(250),Find_OCF(OCF_Alive));
  for(pObj in aList) {
    if(!SelectorTarget(pObj)) continue;
    return(ActivateTarget(pClonk,pObj));
  }

  // Falls kein Tier gefunden worden ist, eine Fehlermeldung ausgeben
  NoPossession(pClonk,"$NoAnimals$");
  return(0);
}

protected func NoPossession(pCaster, szError) {
  Sound("Error",false,pCaster,100,GetController(pCaster)+1);
  PlayerMessage(GetController(pCaster),szError,pCaster);
  RemoveObject(this());
  return(1);
}

public func SelectorTarget(object pTarget)
{
  // Das Zielobjekt muss am Leben sein
  if(!(pTarget->~GetOCF() & OCF_Alive)) return(0);

  // Das Zielobjekt muss übernehmbar sein
  if(!pTarget->~IsPossessible()) return(0);

  // Das Zielobjekt muss sich im Freiem befinden
  if(Contained(pTarget)) return(0);

  // Das Zielobjekt darf nicht bereits von einem Clonk gesteuert werden
  if(GetEffect("PossessionSpell",pTarget)) return(0);

  // Das Zielobjekt muss für den Besitzer sichtbar sein
  if(!Visible(GetOwner(),pTarget)) return(0);

  return(1);
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Die Seele des Clonks in das Tier hineinversetzen
  AddEffect("PossessionSpell",pTarget,200,1,0,GetID(),pCaller);

  // Diesen Zauber löschen
  return(RemoveObject());
}

/* Tierübernahme */

/*
3,4 = Portrait
6=Physical Breath
5=Clon (Für Name, Sicht usw.)

0 = Clon
1 = Zauberer
2 = Aktion des Zauberers

7 = Damage-Effekt
8 = Sichtweite
*/

protected func FxPossessionSpellStart(object pTarget, int iEffectNumber, int iTemp, object pCaster)
{
  // Keine temporären Aufrufe
  if(iTemp) return(0);

  var i, iPlayer, pDouble, pClonk, iID, iSave;
  iPlayer = GetOwner(pCaster);

  // Portrait speichern
  EffectVar(3,pTarget,iEffectNumber) = GetPortrait(pCaster);
  EffectVar(4,pTarget,iEffectNumber) = GetPortrait(pCaster,1);

  // Sichtweite speichern
  EffectVar(8,pTarget,iEffectNumber) = GetObjPlrViewRange(pCaster);

  // Einen neuen Clonk machen, der das Lebewesen steuern wird
  pDouble = CreateObject(GetID(pCaster),0,0,iPlayer);

  // Kopie anpassen
  SetCrewStatus(iPlayer,1,pDouble);

  // Ist der Clonk irgendwie angewählt? (Speichern, damit die Clonkauswahl nicht durcheinander geht)
  for(i = 0; iSave = GetCursor(iPlayer,i); i++) {
    if(iSave != pCaster) continue;
    iSave = true;
    break;
  }
  i = 0;
  if(iSave) {
    i = 1;
    if(GetSelectCount(iPlayer) == 1) i += 1;
  }
  // Jegliche Objektinformationen übertragen
  GrabObjectInfo(pCaster,pDouble);

  // Physicals Energie und Atem an die des Tieres anpassen
  SetPhysical("Energy",GetPhysical("Energy",0,pTarget),2,pDouble);
  SetPhysical("Breath",GetPhysical("Breath",0,pTarget),2,pDouble);
  DoBreath(GetPhysical("Breath",0,pTarget)/1000+1,pDouble);
  // Um es nicht immer wieder abzufragen
  EffectVar(6,pTarget,iEffectNumber) = GetPhysical("Breath",0,pTarget)/1000+1;

  // Clonks passend anwählen (Geht bei der Datenübertragung etwas verloren)
  // War nur ein Clonk angewählt?
  if(i == 2) {
    pClonk = GetCursor(iPlayer);
    // Die Kopie zum Cursor machen
    SetCursor(iPlayer,pDouble,1,1);
    // Den derzeitig angewählten Clonk für die Steuerung deaktivieren
    SelectCrew(iPlayer,pClonk,0);
  }
  // Bei mehreren nur die Kopie zum Cursor machen
  else if(i == 1) SetCursor(iPlayer,pDouble,1,1);

  // Der Clonk darf dabei nicht gestört werden
  EffectVar(7,pTarget,iEffectNumber) = AddEffect("DmgCheck",pCaster,200,0,0,GetID(),pTarget);
//Vllt. mit SetCrewStatus?
  // Damit der Zauberer während dem Borgen überhaupt nicht benutzt werden kann
//  SetCrewStatus(GetOwner(pCaster),false,pCaster);
  SetCrewEnabled(0,pCaster);	// Deaktiviert das Steuern durch Kontrolle
//  SetAlive(0,pCaster);
  SetOwner(-1,pCaster);		// Deaktiviert das Steuern durch Maus
  // Zu beachten ist aber, dass der Clonk danach seinen darüberliegenden Namen verliert (Wenn aktiviert)
  // Desswegen wird eine Kopie erstellt, welche den Namen anzeigen wird
  pClonk = CreateObject(CLNK,GetX(pCaster),GetY(pCaster));
  // Damit sie ganz bestimmt nicht irgendwie anvisiert wird
  AddEffect("BanReplication",pClonk,250,0,0,GetID());
  ObjectSetAction(pClonk,"Dead");
  SetCategory(C4D_StaticBack,pClonk);
  SetGraphics(0,pClonk,TIM1);
  SetOwner(iPlayer,pClonk);
  SetName(GetName(pDouble),pClonk);
  SetObjectLayer(pClonk,pClonk);
  SetObjectOrder(pClonk,pTarget);
  SetVisibility(VIS_Owner|VIS_Allies,pClonk);
  EffectVar(5,pTarget,iEffectNumber) = pClonk;

  // Bei der Übertragung geht der Name verloren
  SetName(GetName(pDouble),pCaster);

  // Portrait usw. der Kopie anpassen
  iID = GetID(pTarget);
  SetPortrait("random",pDouble,iID);
  DoEnergy(GetEnergy2(pTarget)-GetEnergy2(pDouble),pDouble,1);
  SetGraphics(0,pDouble,iID);
  SetPicture(GetDefCoreVal("Picture","DefCore",iID,0),
             GetDefCoreVal("Picture","DefCore",iID,1),
             GetDefCoreVal("Picture","DefCore",iID,2),
             GetDefCoreVal("Picture","DefCore",iID,3),pDouble);
  // Die Kopie soll nicht mit Effekten versehbar sein
  AddEffect("BanDouble",pDouble,250,0,0,GetID());
  // Wenn es nicht lebt, kann es nicht aufeinmal Sterben oder so
  // Nebenbei bietet es noch andere Vorteile
  SetAlive(0,pDouble);

  // In das Lebewesen rein!
  Enter(pTarget,pDouble);

  // Jeweiligen Zauberer anhalten
  SetComDir(COMD_None, pCaster);
  // Er soll währenddessen Meditieren
  ObjectSetAction(pCaster, "Meditate");
  // Falls es kein Indianer ist...
  if(GetAction(pCaster) != "Meditate") {
    ObjectSetAction(pCaster,"Magic");
    if(GetAction(pCaster) != "Magic")
      ObjectSetAction(pCaster, "Walk");
    else {
      i = GetActMapVal("Length",GetAction(pCaster),GetID(pCaster));
      AddEffect("IntResetPhase",pCaster,1,i*GetActMapVal("Delay",GetAction(pCaster),GetID(pCaster))/2-2,0,GetID(),i/3);
    }
  }
  // Damit er nun wirklich Meditiert
//  Schedule(Format("SetAction(\"%s\")",GetAction(pCaster)),1,1,pCaster);

  // Nötige Variabeln speichern
  // Für den Wipf kommt pDouble an EffectVar(0)
  EffectVar(0,pTarget,iEffectNumber) = pDouble;
  EffectVar(1,pTarget,iEffectNumber) = pCaster;
  EffectVar(2,pTarget,iEffectNumber) = GetAction(pCaster);

  // Effekte
  DoParticleEffect(GetX(pCaster),GetY(pCaster));
  DoParticleEffect(GetX(pTarget),GetY(pTarget));
  DrawParticleLine("PxSpark",GetX(pCaster),GetY(pCaster),GetX(pTarget),GetY(pTarget),15,150,RGBa(255,50,50,50),RGBa(50,255,50,100));
  Sound("Magic1",false,pCaster);

  // Fertig
  return(1);
}

protected func FxPossessionSpellTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var pClonk, pCaster, pDouble, iBreath;
  pDouble = EffectVar(0,pTarget,iEffectNumber);
  pCaster = EffectVar(1,pTarget,iEffectNumber);
  iBreath = EffectVar(6,pTarget,iEffectNumber);

  // Falls er nicht mehr meditiert, abbrechen.
  if(GetAction(pCaster) != EffectVar(2,pTarget,iEffectNumber)) {
    // Vllt. eine Ausnahme?
    if(iEffectTime == 1)
      // Clonk nochmal zum meditieren bringen
      ObjectSetAction(pCaster,EffectVar(2,pTarget,iEffectNumber));
    // Andernfalls Abbruch
    else return(-1);
  }

  // Atemluft genau anzeigen
  if(!GetBreath(pTarget)) DoBreath(iBreath,pDouble);
  else DoBreath(GetBreath(pTarget)-GetBreath(pDouble),pDouble);

  // Für den Namenszeiger sorgen
  if(pClonk = EffectVar(5,pTarget,iEffectNumber))
    SetPosition(GetX(pCaster),GetY(pCaster),pClonk);

  // Sichtweite des Controllers anpassen
  // Um es effektmäßig anzupassen
  SetPlrViewRange(EffectVar(8,pTarget,iEffectNumber)+10,pDouble,1);
//  SetPlrViewRange(Min(iEffectTime*10,EffectVar(8,pTarget,iEffectNumber)+10),pDouble,1);
  // Damit der Zauberer nun wirklich nichts mehr sieht
  SetPlrViewRange(Max(EffectVar(8,pTarget,iEffectNumber)-iEffectTime*10,0),pCaster,1);
/*
  if(!EffectVar(9,pTarget,iEffectNumber)) {
    SetPlrViewRange(Max(EffectVar(8,pTarget,iEffectNumber)-iEffectTime*10,0),pCaster,1);
    if(EffectVar(8,pTarget,iEffectNumber)-iEffectTime*10 <= 0)
      EffectVar(9,pTarget,iEffectNumber) = true;
  }
*/

  // Nach einer festgelegten Zeit abbrechen
  if(iEffectTime >= PossessionTime()) return(-1);

  // Countdown für die letzten 10 Sekunden
  if(iEffectTime >= (PossessionTime()-10*35))
    PlayerMessage(GetOwner(pDouble),"@%d",pTarget,PossessionTime()/35-iEffectTime/35);
}

protected func FxPossessionSpellDamage(object pTarget, int iEffectNumber, int iDamage, int iCause)
{
//  if(-iDamage >= GetEnergy2(pTarget)) return(Kill(pTarget));
  DoEnergy(iDamage,EffectVar(0,pTarget,iEffectNumber),1);
  return(iDamage);
}

protected func FxPossessionSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Keine temporären Aufrufe
  if(fTemp) return(0);

  var i, iPlayer, pCaster, pDouble, pClonk, iSave;
  pDouble = EffectVar(0,pTarget,iEffectNumber);
  pCaster = EffectVar(1,pTarget,iEffectNumber);
  iPlayer = GetOwner(pDouble);

  // Das Tier verliert die Countdown-Message
  PlayerMessage(GetOwner(pCaster),"",pTarget);

  // Portrait wieder richtig setzen
  SetPortrait(EffectVar(3,pTarget,iEffectNumber),pDouble,EffectVar(4,pTarget,iEffectNumber));

  // Ist der Clonk irgendwie angewählt? (Speichern, damit die Clonkauswahl nicht durcheinander geht)
  for(i = 0; iSave = GetCursor(iPlayer,i); i++) {
    if(iSave != pDouble) continue;
    iSave = true;
    break;
  }
  i = 0;
  if(iSave) {
    i = 1;
    if(GetSelectCount(iPlayer) == 1) i += 1;
  }
  // Hat sich während der Zeit die Sichtweite des Casters verändert?
//  iSave = GetPlayerViewRange(pCaster);
  // Wird dann vllt. später eingebaut
  // Daten sollen wieder zurückübertragen werden
  GrabObjectInfo(pDouble,pCaster);

  // Zauberer wieder aktivieren
  iSave = GetColorDw(pCaster); // (Wenn er zuvor eine andere Farbe hatte)
  SetOwner(iPlayer,pCaster);
  SetColorDw(iSave,pCaster);
  iSave = GetEnergy2(pCaster);
  SetCrewStatus(iPlayer,1,pCaster);
  DoEnergy(iSave-GetEnergy2(pCaster),pCaster,1);
  SetCrewEnabled(1,pCaster);

  // Clonks passend anwählen (Geht bei der Datenübertragung etwas verloren)
  // War nur ein Clonk angewählt?
  if(i == 2) {
    pClonk = GetCursor(iPlayer);
    // Den Zauberer zum Cursor machen
    SetCursor(iPlayer,pCaster,1,1);
    // Den derzeitig angewählten Clonk für die Steuerung deaktivieren
    SelectCrew(iPlayer,pClonk,0);
  }
  // Bei mehreren nur den Zauberer zum Cursor machen
  else if(i == 1) SetCursor(iPlayer,pCaster,1,1);

  // Die Meditation wird abgebochen
  if(GetAction(pCaster) == EffectVar(2,pTarget,iEffectNumber))
    ObjectSetAction(pCaster,"Walk");
  if(GetEffect(0,pCaster,EffectVar(7,pTarget,iEffectNumber),1))
    RemoveEffect(0,pCaster,EffectVar(7,pTarget,iEffectNumber));

  // Kopie noch da?
  if(pDouble) {
    // Ist das Ziel weg?
    if(!pTarget) {
      // Abbruch
      RemoveObject(pDouble,1);
    }
    // Ansonsten noch Sicht gewähren
    else {
      // Inhalt rauswerfen
      i = ContentsCount(0,pDouble);
      while(i--) Exit(Contents(i-1,pDouble));
      // Layer ändern
      SetObjectLayer(pDouble,pDouble);
      // In das Tier hineinversetzen
      Schedule(Format("Enter(Object(%d))",ObjectNumber(pTarget)),1,0,pDouble);
      // Sicht einstellen
      SetPlrViewRange(GetObjPlrViewRange(pDouble),pDouble,1);
      // Noch bis zum völligem Erlöschen der Sicht warten, dann entfernen
      Schedule("RemoveObject(0,1)",EffectVar(8,pTarget,iEffectNumber)/10,,pDouble);
    }
  }
  if(EffectVar(5,pTarget,iEffectNumber)) RemoveObject(EffectVar(5,pTarget,iEffectNumber),1);

  // Effekte
  DoParticleEffect(GetX(pCaster),GetY(pCaster));
  DoParticleEffect(GetX(pTarget),GetY(pTarget));
  DrawParticleLine("PxSpark",GetX(pCaster),GetY(pCaster),GetX(pTarget),GetY(pTarget),15,150,RGBa(255,50,50,50),RGBa(50,255,50,100));
  Sound("DeMagic",false,pTarget);

  // Fertig
  return(1);
}

protected func DoParticleEffect(int iX, int iY, int iColor1, int iColor2) 
{
  if(!iColor1) iColor1 = RGB(196, 196, 196);
  if(!iColor2) iColor2 = RGB(255, 255, 255);
  CastParticles("PxSpark",5,35,AbsX(iX),AbsY(iY),5*8,5*50,iColor1,iColor2);
  return(1);
}

// Um bei Schaden den Effekt abzubrechen
protected func FxDmgCheckStart(object pTarget, int iEffectNumber, int iTemp, object pShiftTo)
{
  // Keine temporären Aufrufe
  if(iTemp) return(0);

  EffectVar(0,pTarget,iEffectNumber) = pShiftTo;
}
    
protected func FxDmgCheckDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  // Nicht bei Heilung abbrechen
  if(iDmgEngy >= 0) return(iDmgEngy);
  // War der erlittene Schaden zu hoch?
  if(!(iDmgEngy/10000))
    // Hat der Clonk nicht mehr genug Lebensenergie?
    if(GetEnergy(pTarget) > 10) return(iDmgEngy);
  // Eins von beiden tretet ein -> Zauberabbruch
  RemoveEffect("PossessionSpell",EffectVar(0,pTarget,iEffectNumber));
  RemoveEffect(0,pTarget,iEffectNumber);
  return(iDmgEngy);
}

protected func FxDmgCheckStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) { return(1); }

protected func FxIntResetPhaseStart(pTarget, iEffectNumber, iTemp, iPhase)
{
  if(iTemp) return(0);
  EffectVar(0,pTarget,iEffectNumber) = iPhase;
  EffectVar(1,pTarget,iEffectNumber) = GetAction(pTarget);
  SetPhase(EffectVar(0,pTarget,iEffectNumber), pTarget);
  return(1);
}

protected func FxIntResetPhaseTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(GetAction(pTarget) != EffectVar(1,pTarget,iEffectNumber)) return(-1);
  SetPhase(EffectVar(0,pTarget,iEffectNumber), pTarget);
  return(1);
}

// Für die Kopie
func FxBanDoubleStart(pTarget, iEffectNumber, iTemp) { return(1); }
func FxBanDoubleEffect(iEffect, pTarget, iEffectNumber) { return(-1); }
func FxBanDoubleStop(pTarget, iEffectNumber, iReason, iTemp)
{
  if(iReason == 3) return(0);
  return(-1);
}
// Für die Namensanzeige-Kopie
func FxBanReplicationStart(pTarget, iEffectNumber, iTemp) { return(1); }
func FxBanReplicationEffect(iEffect, pTarget, iEffectNumber) { return(-1); }
func FxBanReplicationDamage(pTarget, iEffectNumber, iDmgEngy, iCause) { return(0); }
func FxBanReplicationStop(pTarget, iEffectNumber, iReason, iTemp)
{
  if(iReason == 3) return(0);
  return(-1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("555"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)