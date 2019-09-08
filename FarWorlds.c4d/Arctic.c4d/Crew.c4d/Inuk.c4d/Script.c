/*-- Inuk INUK --*/

#strict
#include CLNK

/* Locals */

local bWearAnorak,iWarmth,iHunger,iWarmTime,iMagicWarmthCnt,iMagicSatiationCnt, HandcraftItem;

private func MaximumWarmth() { return(1800); }
private func MaximumHunger() { return(1000); }
private func ReproductionTime() { return(200); }

/* Initialiserung */

protected func Construction() {
  //hat einen Anorak an
  bWearAnorak=1;
  GraphicDressed();
  // Standardaktion
  SetAction("Walk");
  SetDir(Random(2));
  // Wäremewert
  iWarmth=MaximumWarmth();
  // Hunger 
  iHunger=0;
}

public func Birth() {
  //ein Neugeborenes hat keinen Anorak an
  bWearAnorak=0;
  GraphicUndressed();
  //haben Hunger (wegen "Dorf der Verdammten")
  iHunger=680;
  Sound("InukBirth");
}

/* TimerCall */

private func Life() {
  
  // Schon tot
  if (!GetAlive()) return(0);
  var iWarmthChange,iBuildingWarmth;

  // Wärmeverlust/Aufwärmung je nach Temperatur
  iWarmthChange=BoundBy(GetTemperature()-10, -100, +20);
  // Wärmeschutz durch Kleidung
  if (bWearAnorak) iWarmthChange/=3;
  // Extremer Wärmeverlust im Wasser
  if (!Contained())
    if (InLiquid()) 
      iWarmthChange*=8;
  // Aufwärmung in Gebäuden
  if (Contained())
    if (iBuildingWarmth=(Contained()->~GetWarmth()))
      iWarmthChange=iBuildingWarmth;
  // Aufwärmung durch Lagerfeuer
  if(!Contained())
    if(FindObject(CPFR,-30,-20,60,30,0,"Burn"))
      iWarmthChange+=60;
  // Wärmewert verändern
  iWarmth=BoundBy( iWarmth+iWarmthChange, 0, MaximumWarmth() );

  // Wärmewert 0: Energieverlust
  if (!iWarmth) DoEnergy(-1);
  // Aufwärmung durch Gebäude: Zauber-Aufwärmungen zurücksetzen
  if (iBuildingWarmth) iMagicWarmthCnt=0;
  
  // Hunger steigt
  iHunger=BoundBy(iHunger+1, 0, MaximumHunger());

  CheckWarnings(iWarmthChange);

  // Sehr starker Hunger: essen gehen
  if (iHunger>MaximumHunger()-MaximumHunger()/5)
    // Nur wenn nichts anderes zu tun
    if (!GetCommand())
      GoEat();
  // Extremer Hunger: Energieverlust
  if (iHunger==MaximumHunger())
    DoEnergy(-5);
      
  // Zeit in Gebäude und warm
  if (Contained() && iWarmthChange>0) iWarmTime++;
  else iWarmTime=0;
  // Zur Zeit kein Platz im Iglu: Aufenthaltwert beschränken
  if (Contained())
    if (ObjectCall(Contained(),"IsOccupied"))
      iWarmTime=Min(iWarmTime,ReproductionTime()/2);  

  // Das Fortpflanzungssystem der Inuit ist recht kompliziert:
  // ein Inuk gebiert Nachwuchs, wenn er voll ausgewachsen ist, 
  // lange Zeit in einem warmen Iglu verbracht hat, weder friert
  // noch Hunger hat und im Iglu seit längerer Zeit noch Platz ist.
  if (Contained())
    if (GetOCF()&OCF_Fullcon())
      if (iWarmTime>ReproductionTime())
        if (!IsHungry())
          if (!IsCold())
            if (!ObjectCall(Contained(),"IsOccupied"))
              Reproduction();
      
  // Zur Kontrolle
  //Message("Temperatur: %d|Wärmeverlust: %d|Wärme: %d|Hunger: %d|",this(),GetTemperature(),iWarmthChange,iWarmth,iHunger);      
}

/* Status */

private func IsHungry() {
  if (iHunger>MaximumHunger()-MaximumHunger()/3) return(1);
}

private func IsCold() {
  if (iWarmth<MaximumWarmth()/3) return(1);
}

private func GoEat() {
  var pFood;
  // Keine verfügbare Nahrung in Reichweite
  if (!(pFood=FindFood())) return(0);
  // Nahrung eingesammelt: essen
  if (Contained(pFood)==this())
    return(pFood->Eat(this()));
  // Nahrung einsammeln
  SetCommand (this (), "Call", this (), 0, 0, 0, "GoEat");
  AddCommand (this(),"Get",pFood);
}
// Keine Meldung ausgeben
func GoEatFailed () { return (1); }

private func Reproduction() {
  // Nachwuchs
  var pInuk=CreateObject(INUK,0,0,GetOwner());
  MakeCrewMember(pInuk,GetOwner());
  Enter(Contained(),pInuk);
  DoCon(-40,pInuk);
  pInuk->INUK::Birth();
  // Wartezeit zurücksetzen
  iWarmTime=0;
}

/* Inventarsteuerung */

protected func ControlSpecial() {
  [$TxtSelectinventory$|Image=CXIV]
  // Gegebenfalls Fischen beenden
  StopFishing();
  // Inventory verschieben
  if (!ShiftContents()) return(0);
  // Neues Objekt aktiviert
  if (!(Contents()->~Selection())) // Objekteigener Aktivierungssound
    Sound("Grab"); // Genereller Aktivierungssound
  // Bewaffnung prüfen
  CheckArmed();
  return(1);
}

/* Itemlimit */
public func MaxContentsCount() { return(4); }

/* Steuerung */

protected func ControlLeftDouble() {
  // Gegebenfalls Fischen beenden
  if (StopFishing()) return(1);
  // Sonst keine Spezialsteuerung
}
  
protected func ControlRightDouble() {
  // Gegebenfalls Fischen beenden
  if (StopFishing()) return(1);
  // Sonst keine Spezialsteuerung
}

protected func ControlCommand() {
  // Gegebenfalls Fischen beenden
  if (StopFishing()) return(1);
  // Sonst keine Spezialsteuerung
}

protected func ControlThrow() {
  // Reiten und Werfen
  if (IsRiding())
    if (Contents(0))
    {
      SetAction("RideThrow");
      return(1);
    }
  // Anfassen: keine überladene Steuerung
  if(GetAction()S="Push")
    return(0);
  // Harpune werfen
  if(!GetPlrDownDouble(GetOwner()))
    if(DefinitionCall(GetID(Contents()),"IsHarpoon"))
      if(IsWeaponReady())
        return(SetAction("ThrowHarpoon"));
  // Keine besondere Wurfaktion
  return(0);
}

private func IsWeaponReady() { return(GetAction() eq "Walk" || GetAction() eq "HarpoonWalk" || GetAction() eq "Jump" || GetAction() eq "HarpoonJump"); }

/* Aktionen */

private func StartFishing() {  Sound("FishingPole"); }

protected func Collection() { CheckArmed(); }

protected func Entrance()  { CheckWarnings(); }
protected func Departure() { CheckWarnings(); }

protected func Death() { 
  Sound("Die"); 
  DeathAnnounce(); 
  // Letztes Mannschaftsmitglied tot: Script benachrichtigen
  if (!GetCrew(GetOwner()))
    GameCall("RelaunchPlayer",GetOwner(),GetKiller());
  // Anorak zurücklassen
  AnorakRemoved();
  RemoveWarnings();
  return(1);
}

protected func Destruction() { RemoveWarnings(); }

private func Fishing() {
  var pFish;
  // Ein Fisch an der Angel?
  if (pFish=FindObject(FISH,-20+40*GetDir()-10,8,20,20, OCF_Living(), "Swim"))
    return(SetAction("PickupFish",pFish));    
  if (pFish=FindObject(QFSH,-20+40*GetDir()-10,8,20,20, OCF_Living(), "Swim"))
    return(SetAction("PickupFish",pFish));    
}
  
private func PickupFish() {
  var pBait;
  // Geräusch
  Sound("FishingPole");
  // Köder verbrauchen (wäre korrekter, wenn der Fisch den Köder essen würde)
  if (pBait=FindObject(0, -50,-50,100,100, 0, "Bait",this()))  
    RemoveObject(pBait);
  //zu Angel zurückschalten
  ShiftContents(0,0,FIPL);
  // Fisch töten
  var pFish = GetActionTarget();
  if (pFish)
  {
    Kill(pFish);
    // Toten Fisch einsammeln
    Enter(this(),pFish);
  }
}

private func ConjureUp() {
  CastParticles("PxSpark",2,25,0,0,50,100,RGB(0,128,0),RGB(64,255,64));
  // Weitermachen
  if (GetActTime()<120) return(0);
  // Aufhören
  SetAction("Conjuration3");
  return(1);
}

private func Magic() {
  CreateParticle("MSpark",0,-10,Random(40)-20,Random(10)-50,RandomX(50,100),RGB(Random(64),RandomX(128,255),Random(64)));
}

/* Essen, Einreiben und Magische Beeinflussung */  
  
public func Feed(iValue) {
  // Energie auffüllen
  DoEnergy(iValue);
  // Hunger stillen
  iHunger=BoundBy(iHunger-iValue*3-500+5*GetCon(),0,MaximumHunger());
  // Geräusch
  Sound("ClonkMunch");
  // Zauber-Sättigungen zurücksetzen
  iMagicSatiationCnt=0;
} 

public func Rub(iValue) {
  // Wärmewert verändern
  iWarmth=BoundBy(iWarmth+iValue,0,MaximumWarmth());
  // Geräusch
  Sound("InukRub");
  // Effekt Aktion
  SetAction("Rub");
}

public func MagicHunger(iValue) {
  // Zu oft durch Zauber gesättigt
  if(iMagicSatiationCnt>1) 
    return(Message("$TxtSpellshowsnoeffectRe$",this()));
  // Hunger stillen
  iHunger=BoundBy(iHunger-iValue*3,0,MaximumHunger());
  // Zauber-Sättigungen zählen
  iMagicSatiationCnt++;
  return(1);
}

public func MagicWarm(iChange) {
  // Zu oft durch Zauber aufgewärmt
  if (iChange>0)
    if (iMagicWarmthCnt>1) 
      return(Message("$TxtSpellshowsnoeffectWa$",this()));
  // Wärmewert verändern
  iWarmth=BoundBy(iWarmth+iChange, 0, MaximumWarmth());
  // Zauber-Aufwärmungen zählen
  iMagicWarmthCnt++;
  return(1);
}

/* Kann der Clonk aus der aktuellen Aktion angeln? */

public func ReadyToFish() {
   // Nur beim Laufen
   if (GetAction()ne"Walk") return(0);
   // Nur im Freien
   if (Contained()) return(0);
   // Okay
   return(1);
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

/* Kontextmenü */

protected func ContextMagicState(pCaller) {
  [$TxtMysticstate$|Image=GSPK]
  // Clonk anhalten
  SetComDir(COMD_Stop(),pCaller);
  // Status Abfrage
  MagicState();
  return(1);
}

protected func ContextGoEat(pCaller) {
  [$TxtEat$|Image=INET]
  // Commands löschen, auch wenn's vielleicht nicht nötig ist
  SetCommand (this (), "None");
  // Essen gehen
  GoEat();
  return(1);
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
    AddMenuItem( "$TxtCrafts$", "StartHandcraft", idKnowledge,this());
  return(1);
}

public func ContextConstruction(pCaller)
{
  [$CtxConstructionDesc$|Image=IGLO]
  SetCommand(this(),"Construct");
  return(1);
}

public func ContextAnorakOff(pCaller)
{
  [$TxtTakeoffanorak$|Image=AOUT|Condition=IsDressed]
  // Clonk anhalten
  if(GetAction()eq"Walk") AnorakOff();
  return(1);
}

/* Basteln starten */

protected func StartHandcraft(id def) {
  // Arbeit beginnen
  AddCommand (this (), "Call", this (), def,0,0,0, "DoHandcraft", 0, 3);
  // Aber nur im Freien (vorerst jedenfalls)
  AddCommand (this (), "MoveTo", 0, GetX (), GetY ());
  // Und nix anfassen
  AddCommand (this (), "UnGrab");
  // Material zusammensuchen
  var i, comp, num;
  while (comp = GetComponent(0, i++, 0, def))
    if ((num = GetComponent(comp, 0, 0, def) - ContentsCount (comp)) > 0)
      while (num--) AddCommand (this (), "Acquire", 0,0,0,0,0, comp);
  // Hammer wird benötigt
  if (!FindContents(MLLT) && def != MLLT) NeedMallet ();
}

func DoHandcraft (object self, id def) {
  // Nicht im Wasser etc.
  if(GetAction() ne "Walk") return();
  // Zielobjekt erzeugen, ggf. abbrechen
  if (!(HandcraftItem=ComposeContents(def))) {
    return(1);
  }
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

/* Bei der Arbeit */

private func Handcraft() {
  // Kein Zielobjekt
  if (!GetActionTarget()) return(SetAction("Walk"));
  // Zielobjekt rauslegen
  if (Contained(GetActionTarget())==this())
    Exit (GetActionTarget (), GetDir () * 20 - 10, 10 * GetCon() / 100);
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
  
private func Emboweling() {
  // Kein Messer
  if (!FindContents(KNFE)) {
    AddCommand (this (), "Call", this (), GetActionTarget (), 0,0,0, "DoEmbowel", 0, 1);
    // Wieder ins Freie gehen, falls was aus einem Iglu geholt wurde
    if (!Contained ()) AddCommand (this (), "MoveTo", 0, GetX (), GetY ());
    return(NeedKnife());
  }
  // Kein Zielobjekt
  if (!GetActionTarget()) return(SetAction("Walk"));
  // Zielobjekt rauslegen
  if (Contained(GetActionTarget())==this())
    Exit(GetActionTarget(),-10+GetDir()*20,+10);
  // Zielobjekt wurde von jemand anders eingesammelt
  if (Contained(GetActionTarget())) return(SetAction("Walk"));
  // Zielobjekt ist zu weit weg
  if ( !Inside(GetX()-GetX(GetActionTarget()),-20,+20) || !Inside(GetY()-GetY(GetActionTarget()),-20,+20))
    return(SetAction("Walk"));
  // Geräusch
  Sound("InukEmbowel");
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
func DoEmbowelFailed () {
  // Psst!
  return (1);
}

/* Werkzeug besorgen */

private func NeedKnife() { 
  SetAction ("Walk");
  AddCommand (this (), "Call", this (), 0,0,0,0, "AcquireKnife", 0, 3);
  AddCommand (this (), "Acquire", 0,0,0,0,0, KNFE);
  return (1);
}

private func NeedMallet() { 
  SetAction ("Walk");
  AddCommand (this (), "Acquire", 0,0,0,0,0, MLLT);
}

func AcquireKnife () { return (1); }
func AcquireKnifeFailed () {
  AddMessage("$TxtKniferequired$",this());
  return (1);
}

/* Gebäude errichten */  

func ControlCommandConstruction (target, x, y, target2, data) {
  if (!FindContents(MLLT)) return (1, NeedMallet ());
  if (!Inside(GetX () - x, -5, 5) || !Inside(GetY () - y, -20, 20)) {
    AddCommand(this (), "MoveTo", 0, x, y, 0, 50);
    return (1);
  }
  if (Construct (data)) return (2);
  else return (3);
}

protected func Construct(idConstruct) {
  var pSite;
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction()ne"Walk") return(0);
  if (Contained()) return(0);
  // Baustelle erschaffen
  if (!(pSite=CreateConstruction(idConstruct,0,10,GetOwner(),1,1,1) )) return(0);
  // Meldung
  Message("$TxtConstructions$",this(),GetName(pSite));
  AddCommand (this (), "Build", pSite, 0, 0, 0, 0, 0, 0, 1);
  return(1);
}

/* Material beschaffen */

func GetProducerOf (def) {
  // Der Inuk kann alleine automatisch Sachen herstellen
  if (GetCategory (0, def) & C4D_Object ()) if (GetPlrKnowledge (GetOwner (), def)) return (this ());
  return (inherited (def));
}

func HowToProduce (self, def) {
  StartHandcraft (def);
}

func ControlCommandFinished (string CommandName, object Target, int Tx, int Ty, object Target2, Data) {
  if (CommandName eq "Acquire") if (Data == MLLT) if (!FindContents (MLLT))
    AddMessage("$TxtMalletrequired$",this());
  return (_inherited (CommandName, Target, Tx, Ty, Target2, Data));
}

public func AutoProductionFailed() 
{ 
  // _Nicht_ Kaufen gehen
  // Naechstes Command vom Fehlschlag benachrichtigen
  // Normalerweise macht Buy das
  FinishCommand(0, 0, 1);
  return (1);
}

/* Anorak ausziehen  */

public func AnorakOn() {
  if(bWearAnorak) return();
  SetComDir(COMD_Stop());
  SetXDir();
  SetAction("PutOnAnorak");
}

public func AnorakOff() {
  if(!bWearAnorak) return();
  SetComDir(COMD_Stop());
  SetXDir();
  SetAction("PutOffAnorak");
}

public func AnorakRemoved() {
  if(!bWearAnorak) return();
  CreateObject(ANRK,0,8,GetOwner());
  GraphicUndressed();
  bWearAnorak=0;
  return(1);
}

public func AnorakPutOn() {
  var pAnorak;
  if(!(pAnorak=FindContents(ANRK))) return();
  RemoveObject(pAnorak);
  GraphicDressed();
  bWearAnorak=1;
  return(1);
}

private func GraphicDressed() {
  SetGraphics();
  SetPortrait(GetPortrait(0,0,1),this(),GetPortrait(0,1,1));
}

private func GraphicUndressed() {
  SetGraphics("Undressed");
  SetPortrait("random",this(),CLNK);
}

/* Magiestatus */

func MagicState() {
  if (GetPhysical("Magic",2)<100000) 
    return(Message("$TxtsisnoAngakok$",this(),GetName()));
  Message("$TxtsisanAngakokmysticle$",this(),GetName(),Global());
  return(1);
}

/* Harpune werfen */

private func Throwing(pObj) {
  // Ggf. zu werfendes Objekt suchen
  if(!pObj) { pObj = Contents(0); if (!Contents()) return(); }
  // Wurfparameter berechnen
  var iX, iY, iR, iXDir, iYDir, iRDir;
  if(!DefinitionCall(GetID(pObj),"IsHarpoon"))
  {
    // Standardwurf  
    iX = 0; if (!GetDir()) iX = -iX;
    iY = -10;
    iR = Random(360);
    iXDir = GetPhysical("Throw") / 25000; if(!GetDir()) iXDir = -iXDir;
    iYDir = -GetPhysical("Throw") / 25000;
    iRDir = Random(40) - 20;
  }
  else
  {
    // Speerwurf
    iX = 10; if (!GetDir()) iX = -iX;
    iY = -6;
    iR = 150; if(!GetDir()) iR = 180 - iR;
    iXDir = 5; if(!GetDir()) iXDir = -iXDir;
    iYDir = -2;
    iRDir = 4; if(!GetDir()) iRDir = -iRDir;
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

/* Bewaffnung prüfen */
private func CheckArmed() {
  if (GetAction()S="Walk") if(IsArmed()) return(SetAction("HarpoonWalk"));
  if (GetAction()S="Jump") if(IsArmed()) return(SetAction("HarpoonJump"));
  if (GetAction()S="HarpoonWalk") if(!IsArmed()) return(SetAction("Walk"));
  if (GetAction()S="HarpoonJump") if(!IsArmed()) return(SetAction("Jump"));
}

/* Warnungen */

private func CheckWarnings(iWarmthChange) {
  var pActT;

  if(!GetAlive()) return();

  pActT=this();
  if(Contained()) pActT=Contained();

  // Aufwärmung
  if(iWarmth<9*MaximumWarmth()/10 && iWarmthChange>0)
            SetWarning(WNWM,pActT);
  else          RemWarning(WNWM);

  // Wärmewert: Warnung
  if(IsCold() && iWarmthChange<0)
            SetWarning(WNCL,pActT);
  else          RemWarning(WNCL);

  // Hunger: Warnung
  if(IsHungry())    SetWarning(WNHN,pActT);
  else          RemWarning(WNHN);
}

private func GetWarning(idType) {
  var pWarn, szWarnAct=DefinitionCall(idType,"WarnAction");
  while(pWarn=FindObject(idType,0,0,0,0,0,szWarnAct,0,0,pWarn))
    if(GetActionTarget(1,pWarn)==this())
      return(pWarn);
}

private func RemWarning(idType) {
  var pWarn=GetWarning(idType);
  if(!pWarn) return(0);
  RemoveObject(pWarn);
}

private func SetWarning(idType,pAct0) {
  var pWarn=GetWarning(idType), szWarnAct=DefinitionCall(idType,"WarnAction");
  if(!pWarn) return(ObjectSetAction(CreateObject(idType),szWarnAct,pAct0,this()));
  if(GetActionTarget(0,pWarn)!=pAct0)
    SetActionTargets(pAct0,this(),pWarn);
}

private func RemoveWarnings() {
  return( RemWarning(WNHN) || RemWarning(WNCL) || RemWarning(WNWM));
}

/* Trägt der Clonk eine Harpune? */
public func IsArmed() { return(ObjectCall(Contents(),"IsHarpoon")); }

/* Suchfunktionen */

private func FindFood() {
  var pFoooood;
  // Sucht nach Fleisch oder Fett im Inventar oder in der Nähe, 
  // läßt von herumliegenden Objekten aber immer mindestens eins übrig als Köder.
  if (pFoooood=FindContents(MEAT))
    return(pFoooood);
  if (pFoooood=FindContents(FAT1))
    return(pFoooood);
  if (pFoooood=FindContents(MEAT,Contained()))
    return(pFoooood);
  if (pFoooood=FindContents(FAT1,Contained()))
    return(pFoooood);
  if (pFoooood=FindObject(MEAT,-150,-50,300,100,OCF_Available()))
    if (ObjectCount(MEAT,-150,-50,300,100,OCF_Available())>1)
      return(pFoooood);
  if (pFoooood=FindObject(FAT1,-150,-50,300,100,OCF_Available()))
    if (ObjectCount(FAT1,-150,-50,300,100,OCF_Available())>1)
      return(pFoooood);
  // Keine verfügbare Nahrung gefunden
}

public func IsUndressed() {                 return(!bWearAnorak);}
public func IsDressed()   {if(!Contained()) return(bWearAnorak);}


public func IsInuk() { return true; }
