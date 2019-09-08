/*-- Urwaldclonk --*/

#strict

#include CLNK

local idWeapon, fSplittingPack, iAimPhase, iScaleDelay, HandcraftItem;

/* Initialisierung */

protected func Initialize()
{
  inherited();
  UpdatePhysical("Magic");
}

public func IsJungleClonk() { return(1); }

/* Alte urwaldclonks hatten noch keine Magieenergie */

private func UpdatePhysical(string phys) 
{
  if(GetPhysical(phys,1) < GetPhysical(phys,0,0,GetID()))
    SetPhysical(phys,GetPhysical(phys,0,0,GetID()),1);
}

/* Steuerung */

public func ControlThrow() 
{
  // Lianenklettern
  if (ControlVine("ControlThrow")) return(1); 
  
  // Reiten und Werfen
  if(IsRiding())
    if(Contents(0))
      return(SetAction("RideThrow"));

  // Anfassen: keine überladene Steuerung
  if(GetAction()S="Push")
    return(0);

  // spezielle Bedienung von Waffen in der Hand: Feuern
  if(Ctrl2ID("Fire")) return(1);

  // spezielle Bedienung von Waffen: Waffen einsetzen
  if(!GetPlrDownDouble(GetOwner()))
    if(IsWeaponReady())
      if(Ctrl2Obj("ControlThrow"))
          return(1);
  
  // Keine besondere Wurfaktion
  return(_inherited(...));

}

/* Bedienung an Waffen weitergeben, die gerade benutzt werden */

public func ControlLeft()
{
  if (ControlVine("ControlLeft")) return(1);
  if(Ctrl2ID("ControlLeft"))
    return(1);
  return(_inherited(...));
}
public func ControlRight()
{
  if (ControlVine("ControlRight")) return(1);
  if(Ctrl2ID("ControlRight"))
    return(1);
  return(_inherited(...));
}
public func ControlUp()
{
  if (ControlVine("ControlUp")) return(1);
  if(Ctrl2ID("ControlUp"))
    return(1);
  return(_inherited(...));
}
public func ControlUpdate(object pByObject, int comdir, bool dig, bool throw)
{
  if(ControlVine("ControlUpdate", comdir, dig, throw)) return(1);
  if(Ctrl2ID("ControlUpdate", comdir, dig, throw))
    return(1);
  return(_inherited(pByObject, comdir, dig, throw, ...));
}

public func ControlDown()  
{ 
  // Lianenkontrolle
  if (ControlVine("ControlDown")) return(1); 
  // Baumzerkleinern abbrechen
  if (GetAction() S= "Chop2") return(StopAction());
  // Weitergeleitete Objektkontrolle
  if(Ctrl2ID("ControlDown"))
    return(1);
  return(_inherited(...));
}

public func ControlDigDouble()          // Der Urwaldclonk kann Bäume instant-zerlegen
{
  // Bedienung im Gebäude
  if(Contained()) if(Contained()->~ContainedDigDouble(this())) return(1);
  // Lianenklettern
  if (ControlVine("ControlDigDouble")) return(1); 
  // anfassbare Objekte aktivieren
  if(GetAction()eq"Push") if(GetActionTarget()->~ControlDigDouble(this())) return(1);
  // folgende Aktionen nur im Stehen oder im Schwimmen
  if(GetAction()ne"Walk" && GetAction()ne"Swim") return(1);
  // aktivierbare Objekte aktivieren
  if(Contents()) if(Contents()->~Activate(this())) return(1);
  // wenn schon gefällt wird nicht neu fällen / nicht im Schwimmen fällen
  if(GetAction()eq"Chop2" || GetAction()eq"Swim") return(1);
  var pTree;
  // gefällte bäume zerlegen
  while( pTree = FindObject(0,0,1,0,0,OCF_Grab(),0,0,NoContainer(),pTree) )
    if(pTree->~IsTree())
      return(1,SetAction("Chop2",pTree));
  // nicht gefällte bäume zerlegen
  while( pTree = FindObject(0,0,1,0,0,OCF_Chop(),0,0,NoContainer(),pTree) )
    if(pTree->~IsTree())
      return(1,SetAction("Chop2",pTree));
  return(1);
}

// Abbruch von bestimmten Aktionen

public func ControlLeftDouble()  { if (ControlVine("ControlLeftDouble")) return(1); return(StopAction()); }
public func ControlRightDouble() { if (ControlVine("ControlRightDouble")) return(1); return(StopAction()); }

public func ControlSpecial() // Inventarwechsel
  {
  [$CtrlInventoryDesc$|Image=CXIV]
  // Inventar verschieben
  ShiftContents(0,0,0,1);
  CheckArmed();
  return(1);
  }

public func ControlCommand(string szCommand,object pTarget,int iX, int iY)
{
  if (ControlVine("ControlCommand")) return(1);
  if(Ctrl2ID("ControlCommand",iX,iY)) return(1);
  return(StopAction());
}

public func ContextProduction(pCaller)
{
  [$Herstellmenu$|Image=MLLT|Condition=HasObjKnowledge]
  // Clonk anhalten
  SetComDir(COMD_Stop());
  // Herstellungsmenü
  var idKnowledge;
  CreateMenu(MLLT, pCaller, this(), 1, "$Keinplan$");
  for (var i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pCaller), 0, i, C4D_Object()); ++i)
    AddMenuItem("$Stellher$", "StartHandcraft", idKnowledge, pCaller);
  return(1);
}

/* Um evtl einen Speer anzuzeigen */

protected func Ejection() { CheckArmed(); }
protected func Departure() { CheckArmed(); }

public func MaxContentsCount() { return(4-!!idWeapon); }

public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Der Urwaldclonk kann zusätzlich noch 18 Pfeile tragen (also 3 Packs)
  if(iIndex == 0) { if(fAmount) return(18); return("IsArrow"); }
}

protected func CalcValue()
{
  // Standardwert
  var iValue = _inherited();
  // Wert der benutzten Waffe hinzurechnen
  if(idWeapon)
    iValue += GetDefValue(idWeapon);
  // Wert zurückliefern
  return(iValue);  
}

/* Aufrüfe von Aktionen */

protected func AbortAction() {          // AbortCall von SlingSpin, AimBlowtube
  if(WildcardMatch(GetAction(),"*Sling*")) return();
  if(WildcardMatch(GetAction(),"*Blowtube*")) return();
  if(!idWeapon) return();
  // Waffe wieder im Inventar erzeugen
  CreateContents(idWeapon);
  idWeapon=0;
}

protected func AimAgain() {         // nach dem Nachladen neu zielen
  SetAction("AimBlowtube");
  SetPhase(iAimPhase);
}

protected func SlingSound() {       // StartCall von SlingSpin
  // Sound
  Sound("SlingshotSpin");
}

protected func SlingSpinning() {        // EndCall von SlingSpin
  // nach einer bestimmten Zeit abbrechen
  if(GetActTime()>200)
  {
   if(GetContact(this(),-1) & CNAT_Bottom)
    SetAction("Walk");  
   else SetAction("Jump");
  }
}

protected func SlingThrowing() {        // uund wegschleudern (in der richtigen Animationsphase)
  if(GetPhase()!=3) return();
  Ctrl2ID("Throwing");
}

protected func CheckChop() {            // EndCall von Chop2 (Bäume zerlegen)
  var pA = GetActionTarget();
  // Aktionsziel auf mysteriöse Art und Weise verschwunden
  if(pA)
    if(ObjectDistance(pA) < Sqrt(GetDefWidth(GetID(pA))*GetDefHeight(GetID(pA)))/2 + 8)
    {
      DoDamage(+5,pA);
      // lange genug dran rumgeschnippelt
      if(GetDamage(pA) > (pA->TreeStrength())*Max(GetCon(),30)/125 )
      {
        // SetCon(BoundBy(GetCon(pA)-30,30,100),pA);
        Split2Components(GetActionTarget());
      }
      else { return(); }
    }
  SetAction("Walk");
  CheckArmed();
}

private func Throwing(pObj) {           // EndCall von ThrowSpear (Speerwurf) und Throw
  // Ggf. zu werfendes Objekt suchen
  if(!pObj) { pObj = Contents(0); if(!Contents()) return(); }
  // Wurfparameter berechnen
  var iX, iY, iR, iXDir, iYDir, iRDir, iDir;
  iDir = GetDir()*2-1;

  if(Ctrl2Obj("Throwing")) { return(1); }
  else
  {
    // Standardwurf  
    iX = 0*iDir;
    iY = -10;
    iR = Random(360);
    iXDir = GetPhysical("Throw") / 25000 * iDir;
    iYDir = -GetPhysical("Throw") / 25000;
    iRDir = Random(40) - 20;
  }
  // Reitet? Eigengeschwindigkeit addieren
  if (GetActionTarget()) if (IsRiding())
  {
    iXDir += GetXDir(GetActionTarget()) / 10;
    iYDir += GetYDir(GetActionTarget()) / 10;
  }
  // Werfen!
  Exit(pObj, iX, iY, iR, iXDir, iYDir, iRDir);
  CheckArmed();
  return(1);  
}

protected func Meditating()         // StartCall von Meditate: einige Effekte
{
  if(!Random(4))
    CreateParticle("NoGravSpark",RandomX(-6,+6),RandomX(6,10),0,-3,RandomX(20,40),RGB(64,64,255));
}

/* Prozeduren */

protected func StopAction()         // Abbruch von bestimmten Aktionen
{
  if(GetAction() eq "Chop2" || GetAction() eq "SlingSpin" || GetAction() eq "AimBlowtube" || GetAction() eq "Meditate")
   { SetComDir(COMD_Stop()); SetAction("Walk"); CheckArmed(); return(1); }
  if(GetAction() eq "JumpSlingSpin")
   { SetComDir(COMD_Stop()); SetAction("Jump"); CheckArmed(); return(1); }
}

/* Funktionen */

private func IsWeaponReady()            // ist bereit für das Benutzen von Waffen
{
  return((GetAction() eq "Walk"
      || GetAction() eq "SpearWalk"
      || GetAction() eq "Jump"
      || GetAction() eq "SpearJump")
      && !Contained()
  );
}

private func CheckArmed()           // Bewaffnung (mit Speer) prüfen
{
  if (GetAction()S="Walk") if(IsArmed()) return(SetAction("SpearWalk"));
  if (GetAction()S="Jump") if(IsArmed()) return(SetAction("SpearJump"));
  if (GetAction()S="SpearWalk") if(!IsArmed()) return(SetAction("Walk"));
  if (GetAction()S="SpearJump") if(!IsArmed()) return(SetAction("Jump"));
}

private func StartJump() // StartCall von Jump, SpearJump, Dive
{
  // Bewaffnung prüfen
  CheckArmed();
  // Spezielle Funktionen während des Sprungs
  AddEffect("Jumping", this(), 1, 1, this());
}

protected func FxJumpingTimer()
{
  // Noch im Sprung
  if (IsJumping())
  {
    // Nach einer Liane Ausschau halten
    var pVine; 
    if ((pVine = FindVine()) && (pVine != LastVine))
      GrabVine(pVine);
    // Frei und unterhalb von letzter Liane
    if (LastVine && !pVine && (GetY() > GetY(LastVine)))
      LastVine = 0;
  }
  // Sprung beendet
  else 
  {
    return (-1);
  } 
}
  
// Ist mit Speer bewaffnet

public func IsArmed() { return(DefinitionCall(GetID(Contents()),"IsSpear")); }

// Springt gerade

public func IsJumping() { return((GetAction()S="Jump") || (GetAction()S="SpearJump") || (GetAction()S="Dive")); }

// Steuerung an Objekt/Definition weitergeben

private func Ctrl2ID(string szCtrl) {
  if(idWeapon)
    if(DefinitionCall(idWeapon,szCtrl,this(),Par(1),Par(2),Par(3),Par(4),Par(5)))
      return(1);
}

private func Ctrl2Obj(string szCtrl) { if(Contents()) if(ObjectCall(Contents(),szCtrl,this())) return(1); }

/* Funktionen zur Behandlung von Pfeilen */
/* ****** aus Knight.c4d kopiert! ****** */

/* Fasst iCount Objekte vom Typ idUnpacked zu einem idPacked zusammen */
private func PackContents(iCount, idUnpacked, idPacked)
  {
  // Komponenten entfernen
  var i=iCount, pContents;
  while(i--) if (pContents=FindContents(idUnpacked)) RemoveObject(pContents); else Log("error finding it! (%d)", i);
  // Objekt erzeugen und ggf. weiter packen
  Collection(CreateContents(idPacked));
  return(1);
  }

/* Aufnahme */
protected func Collection(pObj)
  {
  CheckArmed();
  // Nicht mehr enthalten (oder gelöscht): Abbrechen
  if (Contained(pObj) != this()) return(1);
  // Durch Check deaktiviert?
  if (fSplittingPack) return(1);
  // Einzelne Objekte zu Paketen zusammenfassen
  // Ist das aufgenommene Objekt überhaupt packbar?
  var idPackTo = pObj->~PackTo();
  if (!idPackTo) return(1);
  // Sind es genug Objekte?
  var iPackCount = idPackTo->PackCount();
  if (ContentsCount(GetID(pObj)) >= iPackCount)
    // Warum nur nicht ComposeContents? - Könnte auch gehen, ausprobieren, testen.
    PackContents(iPackCount, GetID(pObj), idPackTo);
  return(1);
  }

/* Überprüft ob Par(0) ein Pfeil-ID ist  */
private func CheckArrow(idDef) { return(idDef->~IsArrow()); }

/* Überprüft ob Par(0) ein Pfeilpaket-ID ist */
private func CheckArrowPack(idDef) { return(idDef->~IsArrowPack()); }

/* Liefert die Gesamtzahl eines Objekt(paket)typs */
private func GetObjectCount(idObj)
  {
  var idUnpackedObj;
  if (idUnpackedObj = idObj->~UnpackTo())
    // Auch verschachtelte Pakete mitzählen
    return(GetObjectCount(idUnpackedObj) * idObj->PackCount());
  // Ansonsten ist es nur ein Objekt
  return(1);
  }

// Pfeilpaket aufteilen
private func SplitPack2Components(pPack)
  {
  // Zusammenfassen der Pfeile abschalten
  fSplittingPack=1;
  // Aufteilen
  if(!(pPack->~Unpack(this()))) Split2Components(pPack);
  // Zuzsammenfassen der Pfeile wieder aktivieren
  fSplittingPack=0;
  // Fertig, Erfolg
  return(1);
  }

/* Pfeil aus dem Inventar nehmen */
public func GetArrow()
  {
  // Einzelne Pfeile suchen
  var pObj, pArrow;
  for(var i = 0; pObj = Contents(i); i++)
    if(pObj->~IsArrow())
      return(pObj);
  // Bei Bedarf Pakete aufteilen
  for(var i = 0; pObj = Contents(i); i++)
    if(pObj->~IsArrowPack())
    {
      // Pfeil aus Paket verwenden
      if(pArrow = pObj->~GetItem()) return(pArrow);
      // oder bei alten Pfeilen Paket aufteilen
      if (SplitPack2Components(pObj))
        return(FindSingleArrow());
    }
  // Keine Pfeile gefunden
  return(0);
  }

public func FindSingleArrow()
  {
  // Einzelne Pfeile suchen
  var pObj;
  for(var i = 0; pObj = Contents(i); i++)
    if(pObj->~IsArrow())
      return(pObj);
  // Keiner gefunden
  return(0);
  }

/* Pfeile im Inventar zählen */
private func GetArrowCount()
  {
   return(GetSpecialCount("IsArrow"));
  }

/* Produktion (fast identisch zum INUK) */

protected func StartHandcraft(id def) {
  // Arbeit beginnen
  AddCommand (this (), "Call", this (), def,0,0,0, "DoHandcraft", 0, 1);
  // Aber nur im Freien (vorerst jedenfalls)
  AddCommand (this (), "MoveTo", 0, GetX (), GetY ());
  // Material zusammensuchen
  var i, comp, num;
  while (comp = GetComponent(0, i++, 0, def))
    if ((num = GetComponent(comp, 0, 0, def) - ContentsCount (comp)) > 0)
      while (num--) AddCommand (this (), "Acquire", 0,0,0,0,0, comp);
  // Hammer wird benötigt
  if (!FindContents(MLLT) && def != MLLT) NeedMallet ();
}

func DoHandcraft (self, def) {
  // Nicht im Wasser etc.
  if(GetAction() ne "Walk") return();
  // Zielobjekt erzeugen, ggf. abbrechen
  if (!(HandcraftItem=ComposeContents(def))) {
    return(1);
  }
  // Aktion starten
  SetAction("Handcraft",HandcraftItem);
  return(1);
}

func DoHandcraftFailed (self, id def) {
  // ComposeContents wird nur aufgerufen, wenn nicht alle Komponenten da sind,
  // weil es nur fuer die Fehlermeldung gebraucht wird.
  var doit = false, com, i;
  while (com = GetComponent (0,i++,0,def))
    if (ContentsCount(com) < GetComponent (com,0,0,def)) doit = true;
  if (doit) ComposeContents(def);
//  AddMessage ("$Handwerkdisaster$", this (), GetName (0, def));
  // keine Objektaufruf-Nicht-Moeglich-Meldung
  return(1);
}

private func Handcraft() {
  // Kein Zielobjekt
  if (!GetActionTarget()) return(SetAction("Walk"));
  // Zielobjekt rauslegen
  if (Contained(GetActionTarget())==this())
    Exit(GetActionTarget(),-10+GetDir()*20,+10);
  // Zielobjekt in eigenen Behälter befördern
  if (Contained () && !Contained (GetActionTarget ())) Enter (Contained (), GetActionTarget ());
  // Zielobjekt wurde von jemand anders eingesammelt
  if (Contained(GetActionTarget())) return(SetAction("Walk"));
  // Zielobjekt ist zu weit weg
  if ( !Inside(GetX()-GetX(GetActionTarget()),-20,+20) || !Inside(GetY()-GetY(GetActionTarget()),-20,+20))
    return(SetAction("Walk"));
  // Geräusch
  Sound("Build1");
  // Weitermachen
  if (GetActTime()<50) return(0); 
  // Fertig
  HandcraftItem = 0;
  SetAction("Walk");
  return(1);
}
func AbortHandcraft () {
  if (HandcraftItem) Split2Components(HandcraftItem);
}

private func Building() {
  if(!FindContents(MLLT)) return (NeedMallet ());
  if(!Random(2)) Sound("Build1"); 
}

func ControlCommandConstruction (target, x, y, target2, data) {
  if (!FindContents(MLLT)) return (1, NeedMallet ());
  if (!Inside(GetX () - x, -5, 5) || !Inside(GetY () - y, -20, 20)) {
    AddCommand(this (), "MoveTo", 0, x, y, 0, 50);
    // override internal, don't finish
    return (1);
  }
  if (Construct (data))
  // override internal, finish successful
    return (2);
  // finish unsuccessful
  return (3);
}

protected func Construct(idConstruct) {
  var pSite;
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction()ne"Walk") return(0);
  if (Contained()) return(0);
  // "1-con-baustellen verhindern"
  var doit = false, def, i;
  while (def = GetComponent (0,i++,0,idConstruct)) if (FindContents(def)) doit = true;
  if (!doit && ObjectCount(CNMT)) {
    AddMessage("$Bauenmitnix$", this());
    return();
  }
  // Baustelle erschaffen
  if (!(pSite=CreateConstruction(idConstruct,0,10,GetOwner(),1,1,1) )) return(0);
  // Meldung
  Message("$TxtConstructions$",this(),GetName(pSite));
  AddCommand (this (), "Build", pSite, 0, 0, 0, 0, 0, 0, 1);
  return(1);
}

public func GetObject2Drop (object forobj) {/*
  // Versucht der CLonk Baumaterial zu beschaffen?
  var i;
  while (GetCommand (0, 0, i) ne "Call") ++i;
  if (GetCommand (0, 5, 1) eq "DoHandcraft")*/
  // Oder leg einfach das letzte ab, wenn es kein Hammer ist.
  var i = ContentsCount() - 1;
  if (GetID (Contents(i)) == MLLT) --i;
  if (Contents (i)) return (Contents (i));
  return (Contents());
}

/* Material beschaffen */

private func NeedMallet() { 
  SetAction ("Walk");
  AddCommand (this (), "Acquire", 0,0,0,0,0, MLLT);
}

func ControlCommandFinished (string CommandName, object Target, int Tx, int Ty, object Target2, Data) {
  if (CommandName eq "Acquire") if (Data == MLLT) if (!FindContents (MLLT))
    AddMessage("$Hammernoetig$",this());
  return (_inherited (CommandName, Target, Tx, Ty, Target2, Data));
}

func GetProducerOf (def) {
  // Der Inuk kann alleine automatisch Sachen herstellen
  if (def == WOOD) return (this ());
  if (GetCategory (0, def) & C4D_Object ()) if (GetPlrKnowledge (GetOwner (), def)) return (this ());
  return (inherited (def));
}

func HowToProduce (clonk, def) {
  if (def == WOOD) {
    // Baum suchen
    var pTree;
    if (!(pTree = FindWorkTree(clonk)))
      // Kein geeigneter Baum (fertig)
      { AddMessage("$TxtNomoresuitabletrees$",clonk); FinishCommand (clonk, 0, 1); return(0); /* fail command - if only the Call-Command would check the return value... */ }
    // Baum zerlegen...
    AddCommand(clonk,"Wait",0,0,0,0,0,5*(pTree->TreeStrength())*Max(GetCon(),30)/100);
    AddCommand(clonk,"Call",clonk,0,0,0,0,"ControlDigDouble");
    AddCommand(clonk,"Grab",pTree,0,0,0, 500);
    // ...aber erst fällen
    if (!(GetOCF(pTree) & OCF_Grab()))
      AddCommand(clonk,"Chop",pTree);
  }
  else StartHandcraft (def);
}

public func AutoProductionFailed() 
{ 
  // _Nicht_ Kaufen gehen
  return (1);
}

/* Baumsuchen (Sägewerkscript) */

private func FindWorkTree(object clonk)
{
  // Arbeiter fällt bereits einen Baum
  if (GetAction(clonk) eq "Chop") 
    return(GetActionTarget(0, clonk));
  // Arbeiter hat bereits einen Baum angefaßt
  if (GetAction(clonk) eq "Push") 
    if (GetActionTarget(0,clonk)->~IsTree())
      return(GetActionTarget(0,clonk));
  // Baum zum Anfassen suchen
  var obj;
  if (obj = FindTreeToGrab()) return(obj);
  // Baum zum Fällen suchen
  if (obj = FindTreeToChop()) return(obj);
  // Kein Baum gefunden
  return(0);
}

private func FindTreeToChop()
{
  var obj;
  while (obj = FindObject(0,0,0,-1,-1,OCF_Chop(),0,0,0,obj))
    // Ausgewachsen
    if (GetOCF(obj) & OCF_Fullcon())
    // Im freien Bereich
    if (GetOCF(obj) & OCF_InFree())
    // Nicht zu weit weg
    if (ObjectDistance(obj) < 500)
    // Brennt nicht
    if (!OnFire(obj))
    // Baum wird nicht bereits von einem Clonk gefällt
    if (!FindObject(0,0,0,0,0,OCF_CrewMember(),"Chop",obj))
      // Passenden Baum gefunden
      return(obj);
  // Keinen passenden Baum gefunden
  return(0);
}
  
private func FindTreeToGrab()
{
  var obj;
  while (obj = FindObject(0,0,0,-1,-1,OCF_Grab(),0,0,0,obj))
    // Ist ein Baum
    if (obj->~IsTree())
    // Nicht in einem Behälter
    if (!Contained(obj))
    // Nicht zu weit weg
    if (ObjectDistance(obj) < 500)
    // Brennt nicht
    if (!OnFire(obj))
    // Steckt nicht fest
    if (!Stuck(obj))
    // Baum wird nicht bereits von einem Clonk angefaßt
    if (!FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",obj))
      // Passenden Baum gefunden
      return(obj);
  // Keinen passenden Baum gefunden
  return(0);
}

/* Lianenklettern */

local LastVine;

public func IsScalingVine() { return(GetAction() S= "ScaleVine"); }

private func GrabVine(object pVine)
{
  // Kletteraktion initiieren
  SetAction("ScaleVine", pVine);
  SetComDir(COMD_Stop());
  // Richtung anpassen
  if (GetX() < GetX(pVine)) SetDir(DIR_Right()); else SetDir(DIR_Left());
  AdjustVineOffset(pVine);
  // Klettereffekt
  AddEffect("ScalingVine", this(), 1, 1, this());
}

private func ReleaseVine(int iXDir)
{
  // Letzte Liane speichern
  LastVine = GetActionTarget();
  // Klettereffekt beenden
  RemoveEffect("ScalingVine", this());
  // Fallen
  SetAction("Jump");
  SetXDir(iXDir);
}

protected func FxScalingVineTimer()
{
  var pVine;
  // Clonk hat aus irgendeinem Grund seine Kletteraktion verloren
  if (!(GetAction() S= "ScaleVine"))
    // Klettern beenden
    return(ReleaseVine(0));
  // Keine Liane mehr: klettern beenden
  if (!(pVine = FindVine()))
  {
    // Oberes Ende: versuchen, die Wand zu erreichen
    if (GetComDir() == COMD_Up())
      return(ReleaseVine(-20 + GetDir() * 40));
    // Unteres Ende: einfach fallen lassen
    else
      return(ReleaseVine(0));
  }
  // Ggf. neue Liane speichern
  SetActionTargets(pVine);
  // Abstand zur Liane
  AdjustVineOffset(pVine);
  // Klettergeschwindigkeit 
  var iSpeed = BoundBy(GetPhysical("Scale") / 10000, 0, 3), iStep = 0;
  // Reduzierte Bewegung
  iScaleDelay += iSpeed;
  if (iScaleDelay >= 3) { iScaleDelay -= 3; iStep = 1; }
  // Kletterbewegung
  var iPosY = GetY(), iLastY = GetY();
  var iPhase = GetPhase();
  if (GetComDir() == COMD_Up()) { iPosY -= iStep; iPhase += iStep; }
  if (GetComDir() == COMD_Down()) { iPosY += iStep; iPhase -= iStep; }
  if (iPhase < 0) iPhase = 15; if (iPhase > 15) iPhase = 0;
  SetPhase(iPhase);
  SetPosition(GetX(), iPosY);
  SetXDir(0); SetYDir(0);
  // Nicht in festes Material klettern
  if (Stuck()) { SetPosition(GetX(), iLastY); SetComDir(COMD_Stop()); }
}

private func AdjustVineOffset(object pVine)
{
  // Abstand nach Kletterrichtung
  var iLastX = GetX();
  if (GetDir() == DIR_Left())
    SetPosition(GetX(pVine) + 8, GetY());
  else
    SetPosition(GetX(pVine) - 8, GetY());  
  // Nicht in festes Material klettern
  if (Stuck()) 
  {
    SetPosition(iLastX, GetY());
    return(0);
  }
  // Ausrichtung erfolgreich
  return(1);
}

private func FindVine()
{
  var iOffset = 6; if (GetDir() == DIR_Left()) iOffset = -6;
  return (FindObject(VINE, iOffset, 0));
}  

private func ControlVine(string strControl, int comdir)
{
  // Nicht an einer Liane
  if (!IsScalingVine()) return(0); 

  if(strControl S= "ControlUpdate")
  {
    if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);
    if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
    SetComDir(comdir);
  }
  // Rauf
  if (strControl S= "ControlUp" && !GetPlrJumpAndRunControl(GetController()))
    SetComDir(COMD_Up());
  // Runter
  if (strControl S= "ControlDown" && !GetPlrJumpAndRunControl(GetController()))
    SetComDir(COMD_Down());
  // Rechts
  if (strControl S= "ControlRight")
  {
    // Anhalten
    if ((GetComDir() == COMD_Up()) || (GetComDir() == COMD_Down()))
      SetComDir(COMD_Right());
    // Richtung wechseln
    if (GetDir() == DIR_Right())
      { SetDir(DIR_Left()); if (!AdjustVineOffset(GetActionTarget())) SetDir(DIR_Right()); }
    // Loslassen
    else
      ReleaseVine(+10);
  }
  // Links
  if (strControl S= "ControlLeft")
  {
    // Anhalten
    if ((GetComDir() == COMD_Up()) || (GetComDir() == COMD_Down()))
      SetComDir(COMD_Left());
    // Richtung wechseln
    if (GetDir() == DIR_Left())
      { SetDir(DIR_Right()); if (!AdjustVineOffset(GetActionTarget())) SetDir(DIR_Left()); }
    // Loslassen
    else
      ReleaseVine(-10);
  }
  
  // Werfen
  if (strControl S= "ControlThrow")
  {
    // Ablegen
    SetComDir(COMD_Stop());
    SetCommand(0, "Drop");
  }
  
  // Steuerung überladen
  return(1); 
}  

protected func Chopping()
{
  if(GetAction()ne"Chop2") return(SetAction("Chop2"));
  return(inherited());
}
