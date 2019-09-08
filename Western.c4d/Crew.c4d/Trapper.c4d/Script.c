/*-- Trapper --*/

#strict

#include COWB

local HandcraftItem;
local fGrabbedInfo;
local iScaleDelay;

public func IsTrapper() { return(1); }

// Der Trapper benutzt immer die InfoSektion eines Cowboys
protected func Recruitment()
{
  if(!fGrabbedInfo) // Ein Hack für Peter
    {
    // Cowboy erstellen
    var cb = CreateObject(COWB, 0, 10, GetOwner());
    MakeCrewMember(cb, GetOwner());
    // InfoSektion klauen
    fGrabbedInfo = true;
    GrabObjectInfo(cb);
    // Den anderen Clonk wieder löschen
    RemoveObject(cb);
    // Volles Leben
    DoEnergy(100);
    // Neues Portrait
    AdjustPortrait();
    // Workaround für HEAD-Bug
    SetPlrViewRange(500);
    }
  // Weiter im Text
  return(_inherited());
}

private func GetPortraitCount() { return(1); }

// Der Trapper kann 4 Sachen tragen
public func MaxContentsCount() { return(4); }

// Bauen
// der Einfachheit halber wir kein Hammer mehr benötigt
/*
private func Building() {
  if(GetActionTarget()->~IsTrapperHandcraft()) if(!FindContents(MLLT)) return (NeedTool(MLLT));
  if(!Random(2)) Sound("Build1"); 
}*/

/* Bäume zerlegen */
public func ControlThrow() { if (ControlVine("ControlThrow")) return(1); return(_inherited());	}
public func ControlLeft()  { if (ControlVine("ControlLeft")) return(1); return(_inherited());	}
public func ControlRight() { if (ControlVine("ControlRight")) return(1); return(_inherited());	}
public func ControlUp()    { if (ControlVine("ControlUp")) return(1); return(_inherited());	}
public func ControlUpdate(object pByObject, int comdir, bool dig, bool throw) { if(ControlVine("ControlUpdate", comdir, dig, throw)) return(1); return(_inherited(pByObject, comdir, dig, throw)); }
public func ControlDown() { if (ControlVine("ControlDown")) return(1); return(_inherited()); }
public func ControlLeftDouble()  { if (ControlVine("ControlLeftDouble")) return(1); return(_inherited()); }
public func ControlRightDouble() { if (ControlVine("ControlRightDouble")) return(1); return(_inherited()); }
public func ControlDigDouble()
{
  // Lianenklettern
  if (ControlVine("ControlDigDouble")) return(1); 
  // nur im Stehen
  if(GetAction() ne "Walk") return(_inherited());
  // Nicht aus einem Gebäude heraus
  if(Contained()) return(_inherited());

  var pTree;
  // gefällte bäume zerlegen
  if(FindObject(CNSW)) // Nur mit Regel
  while( pTree = FindObject(0,0,1,0,0,OCF_Grab(),0,0,NoContainer(),pTree) )
    if(pTree->~IsTree())
      return(1,SetAction("Chop2",pTree));
  return(_inherited());
}

public func ControlDown()
{
  // Baumzerkleinern abbrechen
  if (GetAction() S= "Chop2") return(StopAction());

  return(_inherited());
}

protected func CheckTree()  // AbortCall von Chop
{
  if(!FindObject(CNSW)) return();
  // Baum gefällt?
  if(!(GetOCF(GetActionTarget()) & OCF_Grab)) return();
  // Beim Kommando nicht weitermachen (zB. Autoproduktion im Sägewerk)
//  if(GetCommand()) return();
  // Nur im Stehen
  if(!(GetAction() eq "Walk")) return();
  // Nur, wenn ein Baum gefällt wurde (nicht bei Telegrafenmasten)
  if(!(GetActionTarget()->~IsTree())) return();
  // Zerlegen
  SetAction("Chop2", GetActionTarget());
}

protected func CheckChop() {            // EndCall von Chop2 (Bäume zerlegen)
  var pA = GetActionTarget();
  // Aktionsziel auf mysteriöse Art und Weise verschwunden
  if(pA)
    if(ObjectDistance(pA) < Sqrt(GetDefWidth(GetID(pA))*GetDefHeight(GetID(pA)))/2 + 8)
    {
      DoDamage(+5,pA);
      // lange genug dran rumgeschnippelt
      if(GetDamage(pA) > ((pA->TreeStrength())*Max(GetCon(),30)/125)*2 )
      {
        // SetCon(BoundBy(GetCon(pA)-30,30,100),pA);
        Split2Components(GetActionTarget());
      }
      else { return(); }
    }
  SetAction("Walk");
}

/* Fallen entschärfen */
public func ContextTrapDeactivate()
{
  [$CtxDeTrap$|Image=MCMS]
  // Clonk anhalten
  SetComDir(COMD_Stop());
  // Fallen suchen und in ein Menü stopfen
  CreateMenu(MCMS,0,0,0,"$CtxDeTrap$",0,1);
  var trap, range = TrapSeeRange();
  while(trap = FindObject(0, -range/2, -range/2, range, range, OCF_Collectible, 0,0, NoContainer(), trap))
    if(trap->~IsTrap())
      if(trap->~IsArmed())
        AddMenuItem("%s $TxtDeTrap$", Format("DeactivateTrap(Object(%d))", ObjectNumber(trap)), GetID(trap));
}

private func DeactivateTrap(object pTrap)
{
  pTrap->~DeArm(this());
  Sound("ControlRate");
}

/* Fallen aufstellen */

public func ContextTrap()
{
  [$TxtTrap$|Image=CATA]
  // Clonk anhalten
  SetComDir(COMD_Stop());
  var idKnowledge;
  CreateMenu(CATA,this(),this(),1,"$TxtNotrapplans$");
  // Gegenstände herstellen
  for(var i=0;idKnowledge=GetPlrKnowledge(GetOwner(),0,i,C4D_Object());++i)
   if(DefinitionCall(idKnowledge, "IsTrap"))
    AddMenuItem( "$TxtTraps$", "StartTrapping", idKnowledge,this());
  return(1);
}

protected func StartTrapping(id def)
{
  // Falle aufstellen
  AddCommand (this (), "Call", this (), def,0,0,0, "CreateTrap", 0, 1);
  // Vorher Material holen
  AcquireMaterialFor(def);
}

protected func CreateTrap(object self, id def)
{
  // Nur im Stehen
  if(GetAction() ne "Walk") return();
  // Falle erzeugen, ggf. abbrechen
  var Trap;
  if (!(Trap=ComposeContents(def))) {
    return(1);
  }
  else {
    Exit (Trap, GetDir () * (20 + GetDefCoreVal("Width","DefCore",def)/2) - 10, 10 * GetCon() / 100);
  }
  // obligatorische Aktion starten
  SetAction("Trapping");
  return(1);
}

public func Trapping()
{ Sound("Build1"); }

/* Fallen entdecken */
protected func SeekTraps()
{
  if(!GetAlive()) return(0);
  var iTraps = FindObjects(Find_Func("IsTrap"),Find_Func("IsArmed"),Find_Distance(TrapSeeRange()/2),Find_NoContainer());
  for(var iObj in iTraps) UncoverTrap(iObj);
}

public func UncoverTrap(object pTrap)
{
  if(!(pTrap->~MakeVisibleTo(GetOwner(), this()))) return(0);
  // Hack
  var iTim,i,iR;
  iTim = CreateObject(TIM1,AbsX(GetX(pTrap)),AbsY(GetY(pTrap)),GetOwner());
  SetVisibility(VIS_Owner,iTim);
  // Wenn es vor einem Gebäude aufgebaut ist
  SetCategory(GetCategory(iTim)|8388608,iTim);
  Schedule("RemoveObject()",15,0,iTim);
  for(i = 0; i < 15; i++) {
    iR = Random(360);
    CreateParticle("NoGravSpark",AbsX(GetX(pTrap) + Cos(iR, 15)),AbsY(GetY(pTrap) + Sin(iR, 15)),0,0,30,RGBa(250,150,150,100),iTim);
  }
  return(1);
}

public func TrapSeeRange() { return(50); }

/* Herstellen */

public func ContextConstruction(pCaller)
{
  [$CtxConstructionDesc$|Image=CXCN|Condition=HasKnowledge]
  SetCommand(this(),"Construct");
  // Einen Frame später die Fahrzeuge zu dem Menü dazufügen
  ScheduleCall(this(), "MenuVehicles", 1);
  return(1);
}

public func MenuVehicles()
{
  // Fahrzeuge herstellen (aber nur solche, die sich als Baustelle bauen lassen)
  var idKnowledge;
  for(var i=0;idKnowledge=GetPlrKnowledge(GetOwner(),0,i,C4D_Vehicle());++i)
    if(GetDefCoreVal("Construction", "DefCore", idKnowledge))  
      AddMenuItem("$TxtConstructions$","Construct",idKnowledge,this());
}

public func HasObjKnowledge() { return(GetPlrKnowledge(GetOwner(),0,0,C4D_Object())); }

public func ContextProduction(object clonk)
{
  [$TxtCraft$|Image=MLLT|Condition=HasObjKnowledge]
  // Clonk anhalten
  SetComDir(COMD_Stop());
  var idKnowledge;
  CreateMenu(MLLT,this(),this(),1,"$TxtNoconstructionplans$");
  // Gegenstände herstellen
  for(var i=0;idKnowledge=GetPlrKnowledge(GetOwner(),0,i,C4D_Object());++i)
  // Nur Handarbeitsprodukte    
    if(idKnowledge->~IsTrapperHandcraft())
      AddMenuItem( "$TxtCrafts$", "StartHandcraft", idKnowledge,this());
  return(1);
}

/* Basteln starten */
protected func StartHandcraft(id def, int iParameter, int iSpecial) {
  // Hier wird getestet, welche Lederart am besten proudziert werden soll (z.B. Wipfleder oder Schlangenleder)	
  if(def->~GetBestAlternativeID(this())) def = def->~GetBestAlternativeID(this());	
  // Arbeit beginnen
  AddCommand (this (), "Call", this (), def,0,0,0, "DoHandcraft", 0, 3);
  // Vorher Material holen
  AcquireMaterialFor(def);
  // Hammer wird benötigt
  if(def!=MLLT) if (!FindContents(MLLT)) NeedTool(MLLT);
  // Jetzt kann man den Clonks beim Handwerk mit Spezial 2 befehligen, mehrere Objekte dieser Art zu erschaffen.
  if(iSpecial)
    AppendCommand(this(),"Call",this(),def,iSpecial,0,0,"HandcraftComplete", 0, 1);
}

public func HandcraftComplete(object iSelf, id idDef, int iSpecial) {
  //Dauerproduktion nicht an: beenden.
  if(!iSpecial) return(0);
  //Vor der Fortsetzung noch etwa 50 Frames warten (So lange dauert die Herstellung)
  AppendCommand(this(),"Wait",0,0,0,0,75);
  AppendCommand(this(),"Call",this(),idDef,iSpecial,0,0,"StartHandcraft2");
  return(1);
}

protected func StartHandcraft2(object iSelf, id idDef, int iSpecial) { return(StartHandcraft(idDef,0,iSpecial)); }

private func AcquireMaterialFor(id def)
{
  // Zurueckkehren
  AddCommand (this (), "MoveTo", 0, GetX (), GetY ());
  // Und nix anfassen
  AddCommand (this (), "UnGrab");
  // Material zusammensuchen
  var i, comp, num;
  while (comp = GetComponent(0, i++, 0, def))
    if ((num = GetComponent(comp, 0, 0, def) - ContentsCount (comp)) > 0)
      while (num--) AddCommand (this (), "Acquire", 0,0,0,0,0, comp);
}

func DoHandcraft(object pSelf, id idDef) {
  // Nicht im Wasser etc.
  if(GetAction() ne "Walk") return();
  // Grafikstring einlesen (für die verschiedenen Pferdelederfarben)
  var szGraphics = idDef->~GetGraphicString(this());
  // Zielobjekt erzeugen, ggf. abbrechen
  if(!(HandcraftItem = ComposeContents(idDef))) return(1);
  // Grafik setzen
  if(szGraphics) HandcraftItem->~AdaptGraphics(szGraphics);
  // Layer bestimmen
  SetObjectLayer(HandcraftItem,HandcraftItem);
  // Aktion starten
  SetAction("Handcraft", HandcraftItem);
  return(1);
}

func DoHandcraftFailed (self, id def) {
  // ComposeContents wird nur aufgerufen, wenn nicht alle Komponenten da sind,
  // weil es nur fuer die Fehlermeldung gebraucht wird.
  var doit = false, com, i;
  while (com = GetComponent (0,i++,0,def))
    if (ContentsCount(com) < GetComponent (com,0,0,def)) doit = true;
  if (doit) ComposeContents(def);
  // keine Objektaufruf-Nicht-Moeglich-Meldung
  return(1);
}

// Bei der Arbeit
private func Handcraft() {
  // Kein Zielobjekt
  if(!GetActionTarget()) return(SetAction("Walk"));
  // Zielobjekt rauslegen
  if(Contained(GetActionTarget()) == this())
    Exit(GetActionTarget(),GetDir()*20-10,10*GetCon()/100);
  // Zielobjekt in eigenen Behälter befördern
  if(Contained() && !Contained(GetActionTarget())) Enter(Contained(),GetActionTarget());
  // Zielobjekt wurde von jemand anders eingesammelt
  if(Contained(GetActionTarget())) return(SetAction("Walk"));
  // Zielobjekt ist zu weit weg
  if(!Inside(GetX()-GetX(GetActionTarget()),-20,+20) || !Inside(GetY()-GetY(GetActionTarget()),-20,+20))
    return(SetAction("Walk"));
  // Geräusch
  Sound("Build1");
  // Weitermachen
  if(GetActTime()<50) return(0); 
  // Layer bestimmen
  SetObjectLayer(0,HandcraftItem);
  // Fertig
  HandcraftItem = 0;
  SetAction("Walk");
  return(1);
}

func AbortHandcraft () {
  if (HandcraftItem) Split2Components(HandcraftItem);
}

/* Zerlegen */

private func Emboweling() {
  // Der Trapper braucht auch ein Messer
  // Kein Messer
  if (!FindContents(KNFE)) {
    AddCommand (this (), "Call", this (), GetActionTarget (), 0,0,0, "DoEmbowel", 0, 1);
    // Zurueckkehren
    AddCommand (this (), "MoveTo", 0, GetX (), GetY ());
    AddCommand (this (), "UnGrab");
    return(NeedTool(KNFE));
  }
  // Kein Zielobjekt
  if (!GetActionTarget()) return(SetAction("Walk"));
  // Zielobjekt rauslegen
  if (Contained(GetActionTarget())==this())
  {
    Exit(GetActionTarget(),-10+GetDir()*20,+10);
    Kill(GetActionTarget());
  }
  // Zielobjekt wurde von jemand anders eingesammelt
  if (Contained(GetActionTarget())) return(SetAction("Walk"));
  // Zielobjekt ist zu weit weg
  if ( !Inside(GetX()-GetX(GetActionTarget()),-20,+20) || !Inside(GetY()-GetY(GetActionTarget()),-20,+20))
    return(SetAction("Walk"));
  // Geräusch
  Sound("Embowel");
  // Weitermachen
  if (GetActTime()<80) return(0);
  // Zielobjekt zerlegen
  GetActionTarget()->Embowel(this());  
  // Fertig
  SetAction("Walk");
}

func DoEmbowel (self, obj) {
  SetAction ("Embowel", obj);
}

/* Werkzeug beschaffen */
private func NeedTool(id Tool) {
  SetAction ("Walk");
  AddCommand (this (), "Call", this (), Tool,0,0,0, "AcquireTool", 0, 3);
  AddCommand (this (), "Acquire", 0,0,0,0,0, Tool);
  return (1);
}

func AcquireTool () { return (1); }

func AcquireToolFailed (id Tool) {
  if(Tool == KNFE) AddMessage("$TxtKniferequired$",this());
  if(Tool == MLLT) AddMessage("$TxtMalletrequired$",this());
  return (1);
}

public func ContextBait(object clonk)
{
  [$TxtMakeBait$|Image=METB|Condition=HasMeat]
  SetAction("Embowel",HasMeat());
  return(1);
}

public func HasMeat()
{
  var obj, i;
  while(obj = Contents(i++)) if(GetID(obj)!=METB) if(obj->~IsBaitMeat()) return(obj);
  return(0);
}

/* Fischen */

private func Fishing() {
  var pFish;
  // Köder aus Fleisch sind nicht recyclebar, wenn man sie in das Maul eines nahgelegenen Fisches stopft.
  if(GetActTime(this()) < 10) return();
  // Ein Fisch an der Angel?
  if (pFish=FindObject(FISH,-20+40*GetDir()-10,8,20,20, OCF_Living(), "Swim"))
    return(SetAction("PickupFish",pFish));    
  if (pFish=FindObject(QFSH,-20+40*GetDir()-10,8,20,20, OCF_Living(), "Swim"))
    return(SetAction("PickupFish",pFish));    
}
  
private func PickupFish()
{
  var pBait = FindObject2(Find_Action("Bait"),Find_ActionTarget(this()));
  // Geräusch
  Sound("FishingPole");
  // Köder verbrauchen (wäre korrekter, wenn der Fisch den Köder essen würde), nicht verbrauchen, wenn der wiederverwendbar ist
  if(pBait = FindObject2(Find_Action("Bait"),Find_ActionTarget(this()))) {
    if(!pBait->~BaitRecyclable())
      RemoveObject(pBait);
    // Wenn Köder noch da ist: Nur her damit!
    else if(!RejectCollect(0,pBait))
      Enter(this(),pBait);
    else
      SetPosition(GetX(), GetY(), pBait);
  }
  // Fisch töten
  var pFish = GetActionTarget();
  if(pFish) {
    Kill(pFish);
    // Toten Fisch einsammeln
    Enter(this(),pFish);
  }
  //Zuletzt: Zur Angel zurückschalten
  ShiftContents(0,0,FIPL);
}

public func ReadyToFish() {
   // Nur beim Laufen
   if (GetAction()ne"Walk") return(0);
   // Nur im Freien
   if (Contained()) return(0);
   // Okay
   return(1);
}

private func StartFishing() {  Sound("FishingPole"); }

private func CreateBait() {
  // Wird von der Angel selbst erledigt.
  FindContents(FIPL)->FIPL::CreateBait();
}

/* Fischen beenden */

public func StopFishing() {
  var pBait;
  // Fischt gar nicht
  if (GetAction()ne"Fish") return(0);
  // Fischen beenden
  SetComDir(COMD_Stop());
  SetAction("Walk");
  Sound("Grab");
  // Köder wieder einholen
  if (pBait=FindObject(0, -50,-50,100,100, 0, "Bait",this()))
    Enter(this(),pBait);
  return(1);
}

/* Köder auslegen */

private func CreateBait() {
  // Wird von der Angel selbst erledigt.
  FindContents(FIPL)->FIPL::CreateBait();
}

/* Kommandos */
// Gebäude errichten
func ControlCommandConstruction (target, x, y, target2, data) {
  // Keine Konstruktion erlaubt?
  if(data->~RejectConstruction(x - GetX(), y - GetY(), this()) )
    // Construct-Kommando beenden
    return(FinishCommand(this(), false, 0) );
  if(data->~IsTrapperHandcraft())
  {  if (!FindContents(MLLT)) return (1, NeedTool(MLLT)); }
  else return();
  if (!Inside(GetX () - x, -5, 5) || !Inside(GetY () - y, -20, 20)) {
    AddCommand(this (), "MoveTo", 0, x, y, 0, 50);
    return (1);
  }
  if (Construct (data)) return (2);
  else return (3);
}

protected func Construct(id idConstruct) {
  var i, idDef, pSite;
  // Nur wenn der Clonk steht und sich im Freien befindet
  if(Contained()) return(0);
  if(!WildcardMatch(GetAction(),"*Walk")) return(0);
  // Kann das Gebäude hier überhaupt gebaut werden?
  if(idConstruct->~RejectConstruction(0,10,this())) return(0);
  // Ist Baumaterial vorhanden?
  if(ObjectCount(CNMT)) {
    for(i = 0; idDef = GetComponent(0,i,0,idConstruct); i++)
      if(FindContents(idDef)) break;
    if(!idDef) {
      // Keine Baustelle ohne Material
      AddMessage("$TxtNoConMaterial$",this());
      return(0);
    }
  }
  // Baustelle erschaffen
  if(!(pSite = CreateConstruction(idConstruct,0,10,GetOwner(),1,1,1))) return(0);
  // Wenn kein Baumaterial benötigt wird Komponenten sofort erzeugen (Für Fahrzeuge)
  if(!ObjectCount(CNMT))
    for(i = 0; idDef = GetComponent(0,i,0,idConstruct); i++)
      SetComponent(idDef,GetComponent(idDef,0,0,idConstruct),pSite);
  // Meldung
  Message("$TxtConstructions$",this(),GetName(pSite));
  // Arbeit gleich anfangen
  AddCommand(this(),"Build",pSite,0,0,0,0,0,0,1);
  // Fertig
  return(1);
}

public func GetObject2Drop (object forobj) {
  // Alles außer Hammer kann weg
  var i = ContentsCount() - 1;
  if (GetID (Contents(i)) == MLLT) --i;
  if (Contents (i)) return (Contents (i));
  return (Contents());
}

// Fehler beim Selberherstellen
public func AutoProductionFailed() 
{ 
  // _Nicht_ Kaufen gehen
  // Naechstes Command vom Fehlschlag benachrichtigen
  // Normalerweise macht Buy das
  FinishCommand(0, 0, 1);
  return (1);
}

/* Material beschaffen */
func GetProducerOf (def) {
  // Der Indianer kann alleine automatisch Sachen herstellen
  // Allerdings keinen Hammer, denn dazu bräuchte er einen Hammer
//  if (def == MLLT) return (inherited (def));
  if (def == WOOD) return (this ());
  if (GetCategory (0, def) & C4D_Object ()) if(def->~IsTrapperHandcraft()) if (GetPlrKnowledge (GetOwner (), def)) return (this ());
  return (inherited (def));
}

func HowToProduce (clonk, def) {
  if (def == WOOD && FindObject(CNSW)) {
    // Baum suchen
    var pTree;
    if (!(pTree = FindWorkTree(clonk)))
      // Kein geeigneter Baum (fertig)
      { AddMessage("$TxtNomoresuitabletrees$",clonk); FinishCommand (clonk, 0, 1); return(0); }
    // Baum zerlegen...
    AddCommand(clonk,"Wait",0,0,0,0,0,5*(pTree->TreeStrength())*Max(GetCon(),30)/100);
    AddCommand(clonk,"Call",clonk,0,0,0,0,"ControlDigDouble");
//    AddCommand(clonk,"Wait",0,0,0,0,0,5*(pTree->TreeStrength())*Max(GetCon(),30)/125);
//    AddCommand(clonk,"Grab",pTree,0,0,0, 500);
    // ...aber erst fällen
    if (!(GetOCF(pTree) & OCF_Grab()))
      AddCommand(clonk,"Chop",pTree);
  }
  else StartHandcraft (def);
}

func ControlCommandFinished (string CommandName, object Target, int Tx, int Ty, object Target2, Data) {
  if (CommandName eq "Acquire") if (Data == MLLT) if (!FindContents (MLLT))
    AddMessage("$TxtMalletrequired$",this());
  return (_inherited (CommandName, Target, Tx, Ty, Target2, Data));
}

/* Importiert von Sägewerk */
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
    // Wichtig: Baum ist ein Baum (und kein Telegrafenmast)
    if(obj->~IsTree())
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

private func ReleaseVine(int iXDir, int iYDir)
{
  // Letzte Liane speichern
  LastVine = GetActionTarget();
  // Klettereffekt beenden
  RemoveEffect("ScalingVine", this());
  // Fallen
  SetAction("Jump");
  SetXDir(iXDir);
  SetYDir(iYDir);
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

// Für nicht Net2
global func ComDirTransform(int comdir, int tocomdir) { return(_inherited(comdir, tocomdir)); }
global func ComDirLike(int comdir1, int comdir2) { return(_inherited(comdir1, comdir2)); }

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
      ReleaseVine(+20, -10);
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
      ReleaseVine(-20, -10);
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

private func StartJump() // StartCall von Jump, SpearJump, Dive
{
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
/* Überladungen */

protected func StopAction()         // Abbruch von bestimmten Aktionen
{
  if(GetAction() eq "AimRifle" )
   { SetComDir(COMD_Stop()); SetAction("Walk"); return(1); }
  if(GetAction() eq "RideAimRifle" )
   { SetComDir(COMD_Stop()); SetAction("Ride", GetActionTarget()); return(1); }
  if(GetAction() eq "Fish")
   { StopFishing(); return(1); }
  if(GetAction() eq "Chop2")
   { SetAction("Walk"); return(1); }
}
