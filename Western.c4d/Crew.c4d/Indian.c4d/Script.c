/*-- Indianerclonk --*/

#strict

#include COWB

local HandcraftItem, iLastAimAngle, iWaitTime;
local fChief;

/* Initialisierung */

protected func Initialize()
{
  // Clan-Effekt
  if(GetClan(this()) < 0 && GetOwner() != NO_OWNER)
    {
    var ef = AddEffect("Clan", 0, 1, 10, this());
    EffectVar(0,0,ef) = -GetClan();
    EffectVar(1,0,ef) = GetOwner();
    }
  return(_inherited());
}

/* Bei dem Beitritt zum Spieler Portraits prüfen */

public func Recruitment(int player)
{
  // Zur Netzwerk-Sicherheit
  var iRandomPortrait = Random(GetPortraitCount());
  // Mal schauen, was fuer ein Portrait wir haben
  var portrait = GetPortrait(this(), 0, 1);
  var portrait_id = GetPortrait(this(), 1, 1);
  // Ist es ein eigenes Portrait? Dann lassen wir es.
  if(portrait S= "custom") return(_inherited() );
  // Ist eine ID angegeben?
  if(portrait_id && portrait_id != NONE)
  {
    // Kein Indianerportrait?
    if(portrait_id != GetID() )
    {
      SetPortrait(Format("%d", 1 + iRandomPortrait), this(), GetID(), true, false);
      return(_inherited() );
    }
  }
  // Es handelt sich um ein Indianerportrait. Aber ein richtiges?
  if(!WildcardMatch(portrait, "?") )
  {
    // Nein, das war ein Häuptlingsportrait oder sowas. Wir wollen aber lieber ein normales.
    SetPortrait(Format("%d", 1 + iRandomPortrait), this(), GetID(), true, false);
  }
  return(_inherited(player) );
}

protected func UpdateTransferZone()
{
  // Beim Laden Portrait wiederherstellen
  ScheduleCall(this(), "AdjustPortrait", 1);
}

private func AdjustPortrait()
{
  // Beim Laden Portrait wiederherstellen
  if(LocalN("fChief")) return(SetPortrait("IndianChief", this(), GetID())); 
}

private func GetPortraitCount() { return(3); }

/* TimerCall */

private func Life()
{
  // Schon tot
  if (!GetAlive()) return(0);
  var pObj;
  // Das Fortpflanzungssystem der Indianer ist recht kompliziert:
  // ein Indianer gebärt Nachwuchs, wenn er voll ausgewachsen ist,
  // volle Lebensenergie hat, es ein Hauptzelt gibt, und genügend Wohnraum
  // auserdem darf er keinen Hunger/Durst haben
  // und er muss sich in einem Wohntipi befinden
  // und es muss mindestens 100 Nahrung im Lager sein
  if(GetID(Contained()) == TIPI)
    if(GetOCF() & OCF_Fullcon())
      if(!IsHungry() && !IsThirsty())
        if(GetEnergy() >= GetPhysical("Energy", 0, this())/1000)
          if(pObj = FindObjectClan(MTIP, GetClan()))
            if(pObj->~AllowBirth())
              if(iWaitTime++>40)
                Reproduction();
  if(!Contained()) iWaitTime=0;
}

// Fortpflanzen
private func Reproduction() {
  // Nachwuchs
  var pIndian=CreateObject(INDI,0,0,GetOwner());
  FindObjectClan(STIP, GetClan())->~DoFoodAmount(100);
  MakeCrewMember(pIndian,GetOwner());
  Enter(Contained(),pIndian);
  DoCon(-40,pIndian);
  iWaitTime = 0;
}

/* Steuerung */

public func ControlUp()
{
  if(GetAction() S= "Crawl") { SetXDir(); return(SetAction("FlatUp")); }
  return(_inherited());
}

public func ControlDown()
{
  // Baumzerkleinern abbrechen
  if(GetAction() S= "Chop2") return(StopAction());
  // Kriechen anfangen
  if(GetPlrDownDouble(GetOwner()) && ((GetAction() S= "Walk")||(GetAction() S= "SpearWalk")) )
    if(!GetEffect("NoCrawl",this()))
      return(SetAction("StartCrawl"));

  return(_inherited());
}

public func ControlDownSingle()
{ 
  // Nicht beim Zielen
  if(WildcardMatch(GetAction(),"*Bow*"))
    return(1);

  return(_inherited());
}

public func ControlDownDouble()
{ 
  // Nicht beim Zielen
  if(WildcardMatch(GetAction(),"*Bow*")) return(1);
  // Damit man beim Kriechen durch das Anfassen von Objekten nicht aufsteht 
  if(IsCrawling()) return(1);
  return(_inherited());
}

public func ControlThrow()
{
  // Keine Objekte?
  if(!Contents(0)) return(_inherited(...));
  // Kriechpause, wenn der Spieler nur Objekte ablegen will
  if(GetPlrDownDouble(GetOwner()))
    if(!IsCrawling())
      AddEffect("NoCrawl", this(), 99, 25, this());
    else
      return(0);
  // Aus dem Kriechen werfen
  if(IsCrawling())
    if(GetAction() eq "Crawl")
      return(SetAction("CrawlThrow"));
  // Weiter im Text
  var iReturn = _inherited(...);
  if(!iReturn)
    // Ablegen vom Bogen verhindern
    if(GetProcedure() eq "SCALE")
      if(Contents()->~IsBow())
        if(!GetPlrDownDouble(GetOwner()))
          return(1);
  return(iReturn);
}

public func ControlDig()
{
  // Weitergeleitete Objektkontrolle
  if(Ctrl2ID("ControlDig"))
    return(1);

  return(_inherited());
}

public func ControlDigSingle()
{
  // Beim Kriechen kann man nicht graben
  if(IsCrawling()) return(1);
  return(_inherited());
}

// Bäume zerlegen
public func ControlDigDouble()
{
  // nur im Stehen
  if(GetAction() ne "Walk" && GetAction() ne "SpearWalk") return(_inherited());
  // Nicht aus einem Gebäude heraus
  if(Contained()) return(_inherited());

  var pTree;
  // gefällte bäume zerlegen
  while( pTree = FindObject(0,0,1,0,0,OCF_Grab(),0,0,NoContainer(),pTree) )
    if(pTree->~IsTree())
      return(1,SetAction("Chop2",pTree));
  return(_inherited());
}

// Damit man das Zerkleinern bzw. Berauschen abbrechen kann
public func ControlLeftDouble(object pByObj)
{
  var iEffect;
  if(iEffect = GetEffect("IntDecompose",this())) RemoveEffect(0,this(),iEffect);
  if(iEffect = GetEffect("IntSmoking",this()))   RemoveEffect(0,this(),iEffect);
  return(_inherited(pByObj));
}

public func ControlRightDouble(object pByObj)
{
  var iEffect;
  if(iEffect = GetEffect("IntDecompose",this())) RemoveEffect(0,this(),iEffect);
  if(iEffect = GetEffect("IntSmoking",this()))   RemoveEffect(0,this(),iEffect);
  return(_inherited(pByObj));
}

public func ControlSpecial()
{
  var ret = inherited(...);
  CheckArmed();
  return ret;
}

public func ControlContents()
{
  var ret = inherited(...);
  ScheduleCall(this, "CheckArmed", 1, 1);
  return ret;
}

// Bogensteuerung
public func BowControlUp(pBow, &rVal)
  {
  ChangeBowAimAngle(-1);
  // Funktion überladen
  return(1);
  }

public func BowControlDown(pBow, &rVal)
  {
  ChangeBowAimAngle(+1);
  // Funktion überladen
  return(1);
  }

/* Kontext */

public func ContextArm() 
{  
  [$CtxArmDesc$|Image=ARPI|Condition=IsNotFullyArmed]  
  // Alten Befehl löschen
  if(GetEffect("Order*", this())) RemoveEffect("Order*", this());
  // Waffen suchen und anlegen
  AddEffect("OrderArm", this(), 1, 30, this());
  return(1); 
} 

//! Der Clonk gilt als bewaffnet, wenn er mindestens eine Feuerwaffe und Munition trägt.

private func IsFullyArmed() 
{ 
  return( (FindContents(BOW2)&&(GetArrowCount()||HasQuiverArrows())) || FindContents(TOMA) || FindContents(SPRI) ); 
}

public func FxOrderArmTimer(object pTarget, int iEffectNumber) 
{
  var pWeapon, idWp1, idWp2;
  // Wenn's geht eine Tomahawk oder einen Speer suchen
  if(!FindContents(SPRI) && !FindContents(TOMA)) {
    if(Random(2)) { idWp1 = SPRI; idWp2 = TOMA; }
    else { idWp2 = SPRI; idWp1 = TOMA; }
    pWeapon = FindAvailableWeapon(idWp1);
    if(!pWeapon) pWeapon = FindAvailableWeapon(idWp2);
    if(pWeapon) return(SetCommand(this(),"Get",pWeapon));
  }
  // Bogen suchen
  if(!FindContents(BOW2)) {
    pWeapon = FindAvailableWeapon(BOW2);
    if(pWeapon) return(SetCommand(this(),"Get",pWeapon));
  }
  else
    // Hat einen Bogen: noch Pfeile dazu suchen
    if(GetArrowCount() <= 20 && HasQuiverArrows() <= 20) {
      // Köcher suchen
      if(pWeapon = FindAvailableWeapon(QUIV))
        return(SetCommand(this(),"Get",pWeapon));
      // Pfeile suchen
      if(pWeapon = FindAvailableWeapon(ARPI))
        return(SetCommand(this(),"Get",pWeapon));
      else if(pWeapon = FindAvailableWeapon(PARP))
        return(SetCommand(this(),"Get",pWeapon)); 
    }
  // Fehlgeschlagen
  if(!IsFullyArmed()) return(OrderFailure("$MsgNoWeapons$")); // "Keine Waffen."
  // Fertig
  return(OrderSuccess("$MsgArmed$")); // "Bewaffnet."
}

public func ContextDefend() 
{ 
  [$CtxDefendDesc$|Image=BOW2] 
  // Alten Befehl löschen
  if(GetEffect("Order*", this())) RemoveEffect("Order*", this());
  // Diese Position verteidigen 
  ordrData1 = GetX(); ordrData2 = GetY();
  AddEffect("OrderDefend", this(), 1, 30, this());
  return(1); 
}

func FxOrderDefendTimer(pThis, iNumber)
{
  // Distanz von der Verteidigungsposition ermitteln 
  var iDx=Abs(GetX()-ordrData1), iDy=Abs(GetY()-ordrData2); 
  // Kein Ziel: zurück zur Verteidigungsposition 
  if (!pOrdrTarget) 
    if (iDx>20 || iDy>50) 
      return(SetCommand(this(),"MoveTo",0,ordrData1,ordrData2)); 
  // Zu weit weg von Verteidigungsposition 
  if (iDx>150 || iDy>150) 
    return(1, pOrdrTarget=0); 
  // Keine feindliche Einheit in Reichweite 
  pOrdrTarget=FindEnemyUnit(); 
  if (!pOrdrTarget) return(ExecuteWatch()); 
  if (ObjectDistance(pOrdrTarget) > 350) return(ExecuteWatch()); 
  // Bogen einsetzen (wenn möglich) 
  if (IsBowEquipped()) 
    if (ExecuteAttackBow(pOrdrTarget)) 
      return(1);
  // Speer einsetzen (wenn möglich) 
  if (FindSpearContents()) 
    if (ExecuteAttackSpear(pOrdrTarget)) 
      return(1);
  // Winchester einsetzen (wenn möglich) 
  if (IsRifleEquipped()) 
    if (ExecuteAttackRifle(pOrdrTarget)) 
      return(1);
  // Direkter Angriff (mit kurzfristiger Laufzeit) 
  StopAction(); 
  AddCommand(this(),"Attack",pOrdrTarget,0,0,0,2); 
  // Weiter 
  return(1);
}

//! Trägt der Clonk einen Bogen und hat Pfeile dabei? 
private func IsBowEquipped()
{ 
  // Bogen dabei? 
  if (!FindContents(BOW2) && idWeapon != BOW2) return(0); 
  // Pfeile dabei? 
  if (!GetArrowCount() && !HasQuiverArrows()) return(0); 
  // Bogen und Pfeile dabei 
  return(1); 
}

private func ExecuteAttackBow(pTarget) 
  { 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  // Ziel in Reichweite? 
  if (Abs(iTx-GetX())>300 || Abs(iTy-GetY())>300) 
    // Nicht? Mögliche Abschußposition einnehmen 
    return( SetCommand(this(),"MoveTo",0, BoundBy(GetX(),iTx-290,iTx+290),GetY()) ); 
  // Bogen auswählen 
  ShiftContents(0, 0, BOW2); 
  // Läd noch nach
  if(IsLoading()) return(1);
  // Kann leider von hier aus nicht zielen 
  if (!IsAiming()) 
  {
    if (!ReadyToFire()) 
      return(0); 
    else LoadBow(FindContents(BOW2));
  }
  // Ziel nicht im Freien 
  if (Contained(pTarget)) 
    return(0); 
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX(),GetY(), iTx, iTy )) 
    if (!PathFree( GetX(),GetY()-20, iTy-20 )) 
      return(0); 
  // Schießen 
  FireBowAt(iTx,iTy, false); 
  // Weitermachen   
  return(1); 
  }

/* Speer im Inventar finden */
private func FindSpearContents() 
{ 
  var pObj,i; 
  // SPRI suchen 
  if(pObj = FindContents(SPRI)) return(pObj); 
  // Objekte mit IsSpear() suchen 
  while(pObj = Contents(i++)) if(pObj->~IsSpear()) return(pObj); 
  // Nichts gefunden 
  return(); 
} 

private func ExecuteAttackSpear(pTarget) 
  { 
  // Speer noch vorhanden? 
  var pSpear = FindSpearContents(); if (!pSpear) return(); 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  var iDx = iTx-GetX(); 
  // Schon zu nah 
  if (Abs(iDx)>50) return(0); 
  // Ziel in Reichweite? 
  if (Abs(iDx)>120 || Abs(iTy-GetY())>40) 
      // Mögliche Abschußposition einnehmen 
      return(SetCommand( this(),"MoveTo",0, BoundBy(GetX(),iTx-100,iTx+100),GetY() )); 
  // Speer auswählen 
  ShiftContents(0,0,GetID(pSpear)); 
  // Kann leider von hier aus nicht werfen 
  if (!ReadyToFire()) return(0); 
  // Ziel nicht im Freien 
  if (Contained(pTarget)) return(0); 
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX(),GetY(), iTx, iTy )) 
    if (!PathFree( GetX(),GetY()-10, iTx,iTy-10 )) 
      if (!PathFree( GetX(),GetY()-20, iTx,iTy-20 )) 
        return(0); 
  // Richtung  
  if (iDx>0) SetDir(DIR_Right()); else SetDir(DIR_Left()); 
  // Speer werfen 
  if (IsRiding())
    SetAction("RideThrowSpear"); 
  else
    SetAction("ThrowSpear"); 
  // Weitermachen   
  return(1); 
  }

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
    if(idKnowledge->~IsIndianHandcraft())
      if(GetDefCoreVal("Construction", "DefCore", idKnowledge))  
        AddMenuItem("$TxtConstructions$","Construct",idKnowledge,this());
}

public func ContextProduction(pCaller)
{
  [$MnuCrafts$|Image=MLLT|Condition=HasObjKnowledge]
  // Clonk anhalten
  SetComDir(COMD_Stop());
  // Herstellungsmenü
  CreateMenu(MLLT, pCaller, this(), 1, "$TxtNoconstructionplans$");
  for(var idKnowledge, i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pCaller), 0, i, C4D_Object()); i++)
    // Nur Handarbeitsprodukte    
    if(idKnowledge->~IsIndianHandcraft())
      AddMenuItem("$TxtCrafts$", "StartHandcraft", idKnowledge, pCaller);
  return(1);
}

//public func HasKnowledge() { return(GetPlrKnowledge(GetOwner(),0,0,C4D_Structure())); }
//public func HasObjKnowledge() { return(GetPlrKnowledge(GetOwner(),0,0,C4D_Object())); }
public func HasKnowledge() {
  if(GetPlrKnowledge(GetOwner(),0,0,C4D_Structure())) return(true);
  for(var i, iID; iID = GetPlrKnowledge(GetOwner(),0,i,C4D_Vehicle()); i++)
    if(iID->~IsIndianHandcraft())
      if(GetDefCoreVal("Construction","DefCore",iID))
        return(true);
  return(false);
}
public func HasObjKnowledge() {
  for(var i, iID; iID = GetPlrKnowledge(GetOwner(),0,i,C4D_Object()); i++)
    if(iID->~IsIndianHandcraft()) return(true);
  return(false);
}

public func ContextHome(pCaller)
{
  [$CtxHomeDesc$|Image=CXHM|Condition=HasMainTent]
  SetCommand(this(),"Enter", FindMainTent(GetClan()));
  return(1);
}

public func HasMainTent() { return(FindMainTent(GetClan()) && (GetID(Contained()) != MTIP)); }

/* Aktionen */

// Bauen
// der Einfachheit halber wir kein Hammer mehr benötigt
/*
private func Building() {
  if(GetActionTarget()->~IsTent()) if(!FindContents(MLLT)) return (NeedTool(MLLT));
  if(!Random(2)) Sound("Build1"); 
}*/

// Tiere zerlegen
private func Emboweling() {
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
    Exit(GetActionTarget(),-10+GetDir()*20,+10);
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
  GetActionTarget()->Embowel(this);  
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

protected func StartHandcraft(id idDef, int iParameter, int iSpecial) {
  // Hier wird getestet, welche Lederart am besten produziert werden soll (z.B. Wipfleder oder Schlangenleder)	
  if(idDef->~GetBestAlternativeID(this())) idDef = idDef->~GetBestAlternativeID(this());
  // Arbeit beginnen
  AddCommand(this(),"Call",this(),idDef,0,0,0,"DoHandcraft",0,3);
  // Vorher Material holen
  AcquireMaterialFor(idDef);
  // Hammer wird benötigt
  if(idDef != MLLT) if(!FindContents(MLLT)) NeedTool(MLLT);
  // Gegebenfalls mehrere Objekte dieser Art erstellen
  if(iSpecial)
    AppendCommand(this(),"Call",this(),idDef,iSpecial,0,0,"HandcraftComplete", 0, 1);
}

public func HandcraftComplete(object iSelf, id idDef, int iSpecial) {
  //Dauerproduktion nicht an: beenden.
  if(!iSpecial) return(0);
  //Vor der Fortsetzung noch etwa 50 Frames warten (So lange dauert die Herstellung)
  AppendCommand(this(),"Wait",0,0,0,0,75);
  AppendCommand(this(),"Call",this(),idDef,iSpecial,0,0,"StartHandcraft2");
  return(1);
}

protected func StartHandcraft2(object iSelf, id idDef, int iSpecial) { return(StartHandcraft(idDef,0,iSpecial)); } // weiterbasteln

private func AcquireMaterialFor(id idDef)
{
  // Zurueckkehren
  AddCommand(this(),"MoveTo",0,GetX(),GetY());
  // Und nix anfassen
  AddCommand(this(),"UnGrab");
  // Material zusammensuchen
  var i, idComp, iCount;
  while(idComp = GetComponent(0,i++,0,idDef))
    if((iCount = GetComponent(idComp,0,0,idDef) - ContentsCount(idComp)) > 0)
      while(iCount--) AddCommand(this(),"Acquire",0,0,0,0,0,idComp);
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

// StartCall von Meditate: einige Effekte
protected func Meditating()
{
  if(!Random(4))
    CreateParticle("NoGravSpark",RandomX(-6,+6),RandomX(6,10),0,-3,RandomX(20,40),RGB(64,64,255));
}

// Kriechen
protected func StartCrawl()
{
  // Wenn nötig Effekt erzeugen 
  if(!GetEffect("Crawl", this())) AddEffect("Crawl", this(), 1, 0, this());
}

protected func AbortCrawl() 
{
  // Bei manchen Aktionen nicht abbrechen   
  if(GetAction()S="Scale")  return(SetAction("Crawl")); // Anstoßen an der Wand
  if(GetAction()S="Hangle") return(SetAction("Crawl")); // Anstoßen an der Decke
  if(GetAction()S="Tumble") return(SetAction("Crawl")); // Bei Objekttreffern liegen bleiben
  if(GetAction()S="Walk")   return(SetAction("Crawl")); // Mysteriöse Walk-Aktion
  if(IsCrawling())  return(0);  // Immer noch am Kriechen
  // Shape und Vertices zurücksetzen
  RemoveEffect("Crawl", this());
  SetAction("FlatUp"); 
}

// Rauchen
protected func Smoking()
{
  if(!Inside(GetPhase(),2,4)) return(0);
  Smoke(-6+12*GetDir(),-2,8);
  CreateParticle("MaterialSpark", -6+12*GetDir(), -2, 0, 0, 20, RGB(100,50,0));
  DoSmoke(this(),100);
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

// Im Kriechen werfen
private func CrawlThrowing()
{
  // Erstes Inhaltsobjekt werfen
  var pObj = Contents(0);
  // Wurfparameter berechnen
  var iX, iY, iR, iXDir, iYDir, iRDir;
  var iDir = GetDir()*2-1;
  iX = 7; if (!GetDir()) iX = -iX;
  iY = 7;
  iR = Random(360);
  iXDir = GetPhysical("Throw") / 25000; if(!GetDir()) iXDir = -iXDir;
  iYDir = -GetPhysical("Throw") / 25000;
  iRDir = Random(40) - 20;
  if(pObj->~IsSpear())
  {
    // Speerwurf
    iX = 10*iDir;
    iY = 2;
    iR = 90+60*iDir;
    iXDir = 5*iDir;
    iYDir = -2;
    iRDir = 4*iDir;
  }
  // Werfen!
  Exit(pObj, iX, iY, iR, iXDir, iYDir, iRDir);  
  // Fertig
  return(1);  
}

/* Kommandos */

// Gebäude errichten
protected func ControlCommandConstruction(object pTarget, int iX, int iY, object pTarget2, id idData) {
  // Keine Konstruktion erlaubt?
  if(idData->~RejectConstruction(AbsX(iX),AbsY(iY),this()))
    // Construct-Kommando beenden
    return(FinishCommand(this(),false,0));
  // Indianergebäude benötigen einen Hammer
  if(idData->~IsIndianHandcraft()) { if(!FindContents(MLLT)) return(1,NeedTool(MLLT)); }
  else return(0);
  if(!Inside(AbsX(iX),-2,2) || !Inside(AbsY(iY),-20,20)) {
    AddCommand(this(),"MoveTo",0,iX,iY,0,50);
    return(1);
  }
  if(Construct(idData,iX,iY)) return(2);
  else return(3);
}

protected func Construct(id idConstruct, int iComX, int iComY) {
  var i, idDef, pSite;
  var iX = 0, iY = -GetObjectVal("Offset",0,this(),1);
  // Nur wenn der Clonk steht und sich im Freien befindet
  if(Contained()) return(0);
  if(!WildcardMatch(GetAction(),"*Walk")) return(0);
  // Muss der Clonk das Gebäude an einem bestimmten Punkt bauen?
  if(iComX || iComY) { iX = AbsX(iComX); iY = AbsY(iComY); }
  // Kann das Gebäude hier überhaupt gebaut werden?
  if(idConstruct->~RejectConstruction(iX,iY,this())) return(0);
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
  if(!(pSite = CreateConstruction(idConstruct,iX,iY,GetOwner(),1,1,1))) return(0);
  // Wenn kein Baumaterial benötigt wird Komponenten sofort erzeugen (Für Fahrzeuge)
  if(!ObjectCount(CNMT))
    for(i = 0; idDef = GetComponent(0,i,0,idConstruct); i++)
      SetComponent(idDef,GetComponent(idDef,0,0,idConstruct),pSite);
  // Wurde dies nicht durch ein Kommando befohlen?
  if(!iComX && !iComY)
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
  if (GetCategory (0, def) & C4D_Object ())  if(def->~IsIndianHandcraft())  if (GetPlrKnowledge (GetOwner (), def)) return (this ());
  return (inherited (def));
}

func HowToProduce (clonk, def) {
  if (def == WOOD) {
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

func ControlCommandFinished (string CommandName, object Target, any Tx, int Ty, object Target2, any Data) {
  if (CommandName eq "Acquire") if (Data == MLLT) if (!FindContents (MLLT))
    AddMessage("$TxtMalletrequired$",this());
  return (_inherited (CommandName, Target, Tx, Ty, Target2, Data));
}

/* Aufrufe */

// Um evtl. einen Speer anzuzeigen
protected func Ejection() { CheckArmed(); }
protected func Departure() { CheckArmed(); }

// Aufnahme
protected func Collection2(pObj)
{
  CheckArmed();
  // Eventuell neue Pfeile in Köcher stecken
  if(FindContents(QUIV)) FindContents(QUIV)->Entrance(this());
  return(_inherited(pObj));
}

// Kampf ablehnen
protected func RejectFight(object pEnemy) 
{ 
  // Ist ein letzter Gegner gespeichert? 
  if(GetEffect("IntINDILastEnemy", this(), 0, 1) )
    // Ist der letzte Gegner der, mit dem wir hier kaempfen wuerden?
    if(pEnemy == EffectVar(0, this(), GetEffect("IntINDILastEnemy", this()) ) )
      // Ja, also abbrechen
      return(1);
  // Kampf erlauben
  return(0);
}

// Der Indianer braucht keinen Sattel
public func NeedSaddle() {  return(0);  }

// AbortCall AimRifle, AimBow
protected func AbortAction() {
  if(WildcardMatch(GetAction(),"*Bow*")) return();
  return(_inherited());
}

// Zielen beginnen (EndCall von LoadBow)
protected func BeginAiming() { return(SetPhase(iLastAimAngle)); } 

// Pfeil aus dem Inventar nehmen
public func GetArrow(bool NoRemove)
{
  // Einzelne Pfeile suchen
  var pObj;
  for(var i = 0; pObj = Contents(i); i++)
    if(pObj->~IsArrow())
      return(pObj);
  // Bei Bedarf Pakete aufteilen
  for(var i = 0; pObj = Contents(i); i++)
    if(pObj->~IsArrowPack())
      // Pfeil aus Paket verwenden
      return(pObj->~GetItem());
  // Im Köcher nach Pfeilen suchen
  if(HasQuiverArrows()) return(FetchArrow());
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

// Pfeile im Inventar zählen
private func GetArrowCount()
{
  return(GetSpecialCount("IsArrow"));
}

/* Köcher */

// Schauen ob ein Pfeil in den Köcher passt
private func CheckQuiverArrow(pObj)
{
  if(FindContents(QUIV))
    return(FindContents(QUIV)->~AddArrow(pObj));        
}

// Schauen ob ein Pfeilpaket in den Köcher passt
private func CheckQuiverArrowPack(pObj)
{
  if(FindContents(QUIV))
    return(FindContents(QUIV)->~AddPack(pObj));     
}

// Sind Pfeile im Köcher?
public func HasQuiverArrows()
{
  if(FindContents(QUIV))
    return(FindContents(QUIV)->~GetArrowCount());   
}

// Pfeil aus dem Köcher nehmen
public func FetchArrow()
{
  var obj;  
  if(FindContents(QUIV))
    if(obj = FindContents(QUIV)->~FetchArrow()) return(obj);      
}

/* Bogenschießen */

// Beginnt den Zielvorgang
public func AimBow()
{
  // Gibt den Rückgabewert von SetAction zurück
  if (IsRiding())
    {
    if(!GetEffect("RideAimDir",this())) AddEffect("RideAimDir", this(), 99, 2);
    return(SetAction("RideAimBow"));
    }
  else
    return(SetAction("AimBow"));
}

// Beginnt den Ladevorgang
public func LoadBow(pBow, fFirstTime)
{
  // Nur mit Pfeil!
  if (!GetArrowCount() && !HasQuiverArrows())
    {
    // Kein Pfeil fürs erste Zielen: Meldung ausgeben
    if (fFirstTime)
      {
      Sound("CommandFailure1");
      PlayerMessage(GetController(), "$MsgNoArrow$", this()); // Kein Pfeil
      }
    return(1);
    }
  // Bogen?
  if(pBow && !idWeapon)
  {
    idWeapon = GetID(pBow);
    RemoveObject(pBow);
  }
  if(!idWeapon) return();
  // Schon beim Nachladen?
  if (IsLoading()) return(1);
  // War gerade beim Zielen: letzten Winkel sichern
  if (IsAiming())
    iLastAimAngle = GetPhase();
  else
    iLastAimAngle = 4;
  // Muss den Rückgabewert von SetAction zurückgeben, da das Setzen durch Überladungen fehlgeschlagen sein kann
  if (IsRiding())
    {
    if(!GetEffect("RideAimDir", this())) AddEffect("RideAimDir", this(), 99, 2);
    // Wenn Pferd, dann reiten
    return(SetAction("RideLoadBow"));
    }
  else
    return(SetAction("LoadBow"));
}

// Ändert den Zielwinkel
private func ChangeBowAimAngle(iChange)
{
  var iOldAngle;
  // Derzeitigen Winkel ermitteln
  if (IsAiming()) iOldAngle = GetPhase();
  if (IsLoading()) iOldAngle = iLastAimAngle;
  // Am Anschlag?
  if (!Inside(iOldAngle + iChange, 0, 7)) return();
  // Neuen Winkel setzen
  if (IsAiming()) SetPhase(iOldAngle + iChange);
  if (IsLoading()) iLastAimAngle += iChange;
}

private func FireBowAt(int iX, int iY, bool fForce)
{
  var v = 10, // 10 px / tick
      f = 200, // Fixpunktfaktor
      g = -GetGravity() * f * f / 1000 / v, // 0,2 px / tick²
      x = (iX - GetX()) * f / v,
      y = -(iY - GetY()) * f / v /* Korrektur: */ - Abs(x) * GetGravity() / 2000,
      d = y * y + x * x,
      k = y + f * f * f / 2 / g,
      w = k * k - d;
  // Außer Reichweite?
  if(w < 0 && !fForce) return();
  // Flugzeiten und Winkel berechnen
  var t1 = Sqrt( (k + Sqrt(w)) * (f * f * f / g) ),
      t2 = Sqrt( (k - Sqrt(w)) * (f * f * f / g) ),
      phi1 = ArcCos(x, t1),
      phi2 = ArcCos(x, t2);
  if(y < g * t1 / f * t1 / f / f) phi1 = -phi1;
  if(y < g * t2 / f * t2 / f / f) phi2 = -phi2;
    // Winkel umrechnen
    phi1 = (270 - phi1) % 360 - 180; phi2 = 90 - phi2;
  // Erste Lösung außerhalb der Grenzen?
    if(!Inside(phi1, -120, 120) && !fForce) return();
    phi1=BoundBy(phi1, -120, 120);
    // Zielwinkel probieren
  var iAngle;
  if(!SimBow(phi1, iX))
    iAngle = phi1;
  // Zweite Schusslösung?
  else if(t2 < t1 * 3 && !SimBow(phi2, iX))
    iAngle = phi2;
  else if(fForce)
    iAngle = phi1;
    else
    return();
  // Zielt gar nicht: erst mal laden
  if (!IsAiming())
    {
    LoadBow(FindContents(BOW1), 1);
    return();
    }
  // Zielen
  AimBowToAngle(iAngle);
  // Feuern
  return(Ctrl2ID("FireAngle", iAngle, 0, 1));
}

/* Kampf */

// Prüft, ob der Clonk gerade kaempft. 
private func IsFighting() 
{ 
  if (WildcardMatch(GetAction(),"*Fight") ) return(true); 
  if (WildcardMatch(GetAction(),"*Strike") ) return(true); 
  if (WildcardMatch(GetAction(),"*Punch*") ) return(true); 
  return(false); 
}

// Erst 25 Frames nach dem ControlDown-Kommando
private func StopFighting()
{
  // ComDir auf NONE setzen, damit der Clonk nach dem Kampfabbruch nicht weiterlaeuft.
  SetComDir(COMD_None() );
  // StopFighting-Effekt anfuegen, der den Kampf nach 25 Frames abbricht, wenn nicht schon vorhanden.
  if(!GetEffect("IntINDIStopFighting",this(),0,1) ) AddEffect("IntINDIStopFighting", this(), 1, 25, this() );
}

// Während des Kämpfens
private func Fighting(pEnemy) 
{   
  // Kampf mit Handwaffe
  var pWeapon;
  if (pWeapon = GetFightWeapon())
  {
    // Erstmal Kampfaktion setzen
    if (GetAction() ne pWeapon->~GetFightAction())
      return (SetAction(pWeapon->~GetFightAction(), pEnemy));
    // Je nach Waffengeschwindigkeit Schlag durchführen
    if (!Random(pWeapon->~GetStrikeTime()))
      return (SetAction(pWeapon->~GetStrikeAction()));
    // Weiterkämpfen
    return(1);
  }
  // Keine Waffe: Gegner schlagen
  return(_inherited());
}

// Schlag mit Handwaffe
private func Striking()
{
  // Waffe bestimmen
  var pWeapon;
  if (!(pWeapon = GetFightWeapon())) return(0);
  // Sound
  pWeapon->~StrikeSound();
  // Kein Schaden
  if (!Random(2)) return(1);
  // Schaden ausrechnen
  var pEnemy = GetActionTarget();
  var iDamage = pWeapon->~GetStrikeDamage();
  iDamage = iDamage * (100 - pEnemy->~IsShielded()) / 100;
  // Schlagen!
  Punch(pEnemy, iDamage);
  return(1);
}

// Liefert die erste gefundene Handwaffe, die der Clonk verwenden kann.
public func GetFightWeapon(pClonk)
{
  // Anderer Clonk
  if (pClonk)
    return (pClonk->~GetFightWeapon());
  // Dieser Clonk
  var pObj;
  for (var i = 0; pObj = Contents(i); i++)
    if (pObj->~IsFightWeapon() && CanUse(GetID(pObj)))
      return(pObj);
  // Keine Waffe
  return(0);
}

/* Hilfen */

// Bäume zerlegen
protected func CheckTree()  // AbortCall von Chop
{
  CheckArmed();
  // Baum gefällt?
  if(!(GetOCF(GetActionTarget()) & OCF_Grab)) return();
  // Beim Kommando nicht weitermachen (zB. Autoproduktion im Sägewerk) - sowas kann der Indianer halt nicht
//  if(GetCommand()) return();
  // Nur im Stehen
  if(GetAction() ne "Walk" && GetAction() ne "SpearWalk") return();
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
        // Wart-Kommandos hier abbrechen
        if(GetCommand() eq "Wait") FinishCommand(this(), 1);
      }
      else { return(); }
    }
  SetAction("Walk");
  CheckArmed();
}

// Bewaffnung (mit Speer) prüfen
private func CheckArmed()
{
  if (GetAction()S="Walk") if(IsArmed()) return(SetActionKeepPhase("SpearWalk"));
  if (GetAction()S="Jump") if(IsArmed()) return(SetActionKeepPhase("SpearJump"));
  if (GetAction()S="SpearWalk") if(!IsArmed()) return(SetActionKeepPhase("Walk"));
  if (GetAction()S="SpearJump") if(!IsArmed()) return(SetActionKeepPhase("Jump"));
}

// Nach dem Laden wieder passendes Shape  
protected func UpdateTransferZone()
{
  if(GetAction()S="Crawl") 
  { 
    RemoveEffect("Crawl", this());
    AddEffect("Crawl", this(), 1, 0, this());
  }
  return(_inherited());
}

private func MaxContentsCount() { return(4-!!idWeapon); }

public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Mit Köcher kann der Indianer 30 Pfeile tragen
  if(FindContents(QUIV)) if(iIndex == 0) { if(fAmount) return(30); return("IsArrow"); }
}

// Waffe benutzbar
public func CanUse(idObj)
{
  if(idObj == BOW2) return(1);  
  if(idObj == TOMA) return(1);
}

// Abbruch von bestimmten Aktionen
protected func StopAction()
{
  if(GetAction() eq "Chop2" || GetAction() eq "Meditate")
   { SetComDir(COMD_Stop()); SetAction("Walk"); CheckArmed(); return(1); }
  if(GetAction() eq "AimRifle" )
   { SetComDir(COMD_Stop()); SetAction("Walk"); return(1); }
  if(GetAction() eq "RideAimRifle" )
   { SetComDir(COMD_Stop()); SetAction("Ride", GetActionTarget()); return(1); }
  StopAiming();
}

// Zielen beenden
public func StopAiming()
{
  // Zielt gar nicht: nix machen
  if (!IsAiming()) return(0);
  // Stop
  SetComDir(COMD_Stop());
  // Basisaktion setzen
  if (IsRiding())
    SetAction("Ride");
  else
    SetAction("Walk");
  // Sound
  Sound("Grab");
  // Bewaffnung prüfen
  CheckArmed();
  return(1);
}

// Winkel, in dem momentan gezielt wird
public func GetBowAimAngle()
{
  // Richtung ermitteln
  var iOutDir; if (!GetDir()) iOutDir=-1; else iOutDir=1;
  // Winkel (von oben) ermitteln
  return(iOutDir*GetPhase()*115/7);
}

// Zielabweichung des Bogens
public func GetAimingDeviation(pBow)
{
  return(5);
}

// Ausgangsgeschwindigkeit und -position, wenn mit dem Bogen in angegebene Richtung geschoßen wird
public func GetBowOut(pBow, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir)
{
  // Zufallsabweichung
  if(fDeviate)
    {
    var iDeviation = GetAimingDeviation(pBow);
    iAngle += Random(iDeviation * 2 + 1) - iDeviation;
    }
  // Austrittsgeschwindigkeit ermitteln
  iOutXDir = Sin(iAngle, 70);
  iOutYDir = -Cos(iAngle, 70);
  // Austrittsposition...
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100);
  // Pfeildrehung
  iOutR = iAngle;
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig
  return(1);
}

// Simuliert den Bogenschuss
private func SimBow(int iAngle, int iTargetX)
{
  // Startwerte
  var iX, iY, iR, iXDir, iYDir, iRDir;
  GetBowOut(0, iAngle, false, iX, iY, iR, iXDir, iYDir, iRDir);
  // Koordinaten umrechnen
  iX += GetX(); iY += GetY();
  // Zeit, die das Geschoss fliegen muss
  var iTime = (iTargetX - iX) * 10 / iXDir;
  // Simulieren
  return(SimFlight(iX, iY, iXDir, iYDir, 1, 100, iTime));
}

// Zielt in eine bestimmte Richtung (Winkel)
private func AimBowToAngle(iAngle)
{
  // Winkel anpassen
  while (iAngle > 180) iAngle -= 360;
  // Richtung
  if(!IsRiding())
    { if (iAngle > 0 ) SetDir(DIR_Right()); else SetDir(DIR_Left()); }
  else
    { if (iAngle > 0 ) { SetDir(DIR_Right()); GetActionTarget()->~TurnRight(); } else { SetDir(DIR_Left()); GetActionTarget()->~TurnLeft(); } }
  // Zielrichtung
  SetPhase(BoundBy( (Abs(iAngle)+8)/16, 0,7));
  // Fertig
  return(1);
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

/* Häuptling und dessen Zauberfunktionen */
public func MakeChief()
{
  if(fChief) return();
  fChief = 1;
  // Grafik ändern
  SetGraphics("Chief");
  SetPortrait("IndianChief", this(), GetID());
  // Mehr Zauberkraft
  SetPhysical("Magic", Max(GetPhysical("Magic", 0)+25000, 75000), 2);
  return(1);
}

public func GetSpell(iIndex)
{
  if(!iIndex--) return(BORW);// Borgen 
  if(!iIndex--) return(GZ9Z);// Schutzzaps
  if(!iIndex--) return(MLGT);// Magischer Blitz
  if(!iIndex--) return(MWWV);// Wasserwelle
  if(!iIndex--) return(MGPL);// Säuche
}

public func FindDecomposeItem()
{
  var i, pObj;
  // Nur Häuptlinge können zerkleinern
  if(!IsChief()) return(0);
  // Bei vorhandenem Amulett nicht doppelt anzeigen
  if(pObj = FindObject(0,0,0,0,0,0,"WearAmulet",this()))
    // Kann man mit dem Amulett überhaupt zaubern?
    if(pObj->~AmuletCanMagic())
      return(0);
  // Der Clonk muss draußen sein
  if(Contained()) return(0);
  // Nicht bei voller Zauberenergie 
  if(GetMagicEnergy() >= (GetPhysical("Magic")/1000)) return(0);
  // Wenn verfügbar, dann wird ein schon zerkleinerter Knochen genommen
  if(pObj = FindObject2(Sort_Func("GetCon"),Find_Func("IsManaContainer"),Find_Container(this()),Find_Not(Find_OCF(OCF_Fullcon))))
    return(pObj);
  // Ansonsten das nächste zerkleinerbare Inventarsobjekt
  for(i = 0; pObj = Contents(i); i++)
    if(pObj->~IsManaContainer())
      return(pObj);
}

public func ContextDecompose(id idType, object pObject)
{
  [$MnuDecompose$|Image=SKUL|Condition=FindDecomposeItem]
  // Nur im Stehen
  if(!WildcardMatch(GetAction(),"*Walk*")) return(0);
  // Zerlegen
  AddEffect("IntDecompose",this(),1,1,0,AMBR,FindDecomposeItem(),this());
}

protected func CanDoMagic()
{
  var pObj;
  // Nur Häuptlinge können zaubern
  if(!IsChief()) return(0);
  // Bei vorhandenem Amulett nicht doppelt anzeigen
  if(pObj = FindObject(0,0,0,0,0,0,"WearAmulet",this()))
    // Kann man mit dem Amulett überhaupt zaubern?
    if(pObj->~AmuletCanMagic())
      return(0);
  return(1);
}

protected func ContextUse(object pClonk, object pObj, int iSelection)
{
  [$TxtMagic$|Image=MCMS|Condition=CanDoMagic]
  Magic(pClonk);
}

private func Magic()
{
  // Das Menu ist jederzeit aufmachbar
  //if(!IsWeaponReady()) return();
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),this());
  // Menü öffnen
  var iExtra, idMagic, i;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=C4MN_Extra_Components; else iExtra=C4MN_Extra_ComponentsLiveMagic; }
  else                  { if(ObjectCount(NMGE)) iExtra=C4MN_Extra_None;       else iExtra=C4MN_Extra_LiveMagicValue;      }
  CreateMenu(MCMS,this(),this(),iExtra,"$MnuNoSpells$",ObjectNumber(this()));
  for(i = 0; idMagic = GetSpell(i); i++)
    // Check, ob der Spruch geladen ist
    if(FindDefinition(idMagic))
      AddMenuItem("$TxtMagic$: %s","DoMagic",idMagic,this());
  // Letzten Spruch auswählen
  SelectMenuItem(2,this());
}

protected func DoMagic(id idSpell)
{
  var iAction = GetAction();
  // Nicht bereit
  if(iAction ne "Walk" && iAction ne "SpearWalk" &&
     iAction ne "Jump" && iAction ne "SpearJump" &&
     iAction ne "Ride" && iAction ne "RideStill" &&
     iAction ne "Swim" && iAction ne "Dive" &&
     iAction ne "KneelDown" && iAction ne "KneelUp") {
    Sound("Error",0,this(),100,GetController()+1);
    PlayerMessage(GetOwner(),"$MsgNotReadyToMagic$",this(),GetName(this()));
    return();
  }

  // Nicht in Tieren zaubern (Borgen)
  if(Contained())
    if(GetCategory(Contained()) & C4D_Living) {
      Sound("Error",0,this(),100,GetController()+1);
      PlayerMessage(GetOwner(),"$MsgNotReadyToMagic$",this(),GetName(this()));
      return();
    }

  // Nicht genug Zauberenergie
  if(!CheckMana(idSpell,this())) {
    Sound("Error",0,this(),100,GetController()+1);
    PlayerMessage(GetOwner(),"$MsgNotEnoughEnergy$",this());
    return();
  }

  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(idSpell,this())) {
    Sound("Error",0,this(),100,GetController()+1);
    PlayerMessage(GetOwner(),"$MsgNotEnoughIngredients$",this(),GetNeededAlcStr(idSpell,this()));
    return();
  }

  // Zauberobjekt erschaffen und aktivieren
  if(!CreateObject(idSpell,0,0,GetOwner())->~Activate(this(),this()))
    // Zauber fehlgeschlagen
    return(0);

  // Zauberaktion durchführen
  if(IsRiding()) SetAction("RideMagic");
  else if(iAction eq "Walk" || iAction eq "SpearWalk" || iAction eq "KneelDown" || iAction eq "KneelUp") SetAction("Magic");

  // Zauberenergie abziehen
  DoMagicEnergy(-GetValue(0,idSpell),this());

  // alchemische Zutaten abziehen
  if(ObjectCount(ALCO)) ReduceAlchem(idSpell,this());

  // Magie kann man hören
//  Sound("Magic1");

  // Fertig
  return(1);
}

/* Status */

// Ist ein Indianer
public func IsIndian() { return(1); }
// Häuptling?
public func IsChief() { return(fChief); }
// Kriecht gerade
public func IsCrawling() { return(WildcardMatch(GetAction(), "*Crawl*")); }
// Ist mit Speer bewaffnet
public func IsArmed() { return(DefinitionCall(GetID(Contents(, , true)),"IsSpear")); }
// Springt gerade
public func IsJumping() { return((GetAction()S="Jump") || (GetAction()S="SpearJump") || (GetAction()S="Dive")); }
// Bereit eine Waffe einzusetzen
private func IsWeaponReady()
{
  return((GetAction() eq "Walk"
      || GetAction() eq "SpearWalk"
      || GetAction() eq "Jump"
      || GetAction() eq "SpearJump"
      || GetAction() eq "Ride"
      || GetAction() eq "RideStill")
      && !Contained()
  );
}
// Bereit eine Waffe abzufeuern
public func ReadyToFire()
{
  return((GetAction() eq "Walk"
      || GetAction() eq "SpearWalk"
      || GetAction() eq "Ride"
      || GetAction() eq "RideStill")
      && !Contained()
  );
}
// Lädt der Clonk gerade den Bogen nach?
private func IsLoading()
{
  return (GetAction() eq "LoadBow" || GetAction() eq "RideLoadBow");
}
// Zielt der Clonk gerade mit dem Bogen?
private func IsAiming()
{
  return (GetAction() eq "AimBow" || GetAction() eq "RideAimBow");
}

/* Effekte */

public func FxNoCrawlTimer() { return(-1); }

public func FxCrawlStart(object pTarget, int iEffectNumber)
{
  // Physical, Shape und Vertixes anpassen
  SetPhysical("Walk", GetPhysical("Walk", 0)/4, 3);
  SetShape(-8, 2, 16, 8);
  SetVertexXY(0, 0,5);
  SetVertexXY(1, 0,2);
  SetVertexXY(2, 0,9);
  SetVertexXY(3,-2,3);
  SetVertexXY(4, 2,3);
  SetVertexXY(5,-4,3);
  SetVertexXY(6, 4,3);
}

public func FxCrawlStop(object pTarget, int iEffectNumber)
{
  // Physical, Shape und Vertices zurücksetzen
  ResetPhysical(0, "Walk");
  SetShape(-8, -10, 16, 20);
  SetVertexXY(0, 0, 0);
  SetVertexXY(1, 0,-7);
  SetVertexXY(2, 0, 9);
  SetVertexXY(3,-2,-3);
  SetVertexXY(4, 2,-3);
  SetVertexXY(5,-4, 3);
  SetVertexXY(6, 4, 3);
}

protected func FxIntINDIStopFightingStop()
{
  // Kaempfen wir noch?
  if(IsFighting() )
  {
    // Letzten Gegner fuer weitere 25 Frames speichern
    AddEffect("IntINDILastEnemy", this(), 1, 25, this(), 0, GetActionTarget() );
    // Kampf abbrechen
    SetAction("Walk");
  }
}

protected func FxIntINDILastEnemyStart(pTarget, iEffect, pEnemy) 
{ 
  EffectVar(0, pTarget, iEffect) = pEnemy; 
}
