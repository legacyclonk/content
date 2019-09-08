/* Cowboy */

#strict
#include CLNK

local idWeapon, fSplittingPack, iAimPhase, iRifleAmmo;

/* Dem Spieler bei Bedarf einen HUD geben */

protected func Recruitment()
{
  // Hud geben	
  if(!FindObjectOwner(AHUD,GetOwner())) CreateObject(AHUD,0,0,GetOwner());	
  // Mit voller Energier starten
  DoEnergy(100);
  // Weiter im Text
  return(_inherited(...));
}

/* Steuerung */

public func ControlThrow()
{
  // spezielle Bedienung von Waffen in der Hand: Feuern
  if(Ctrl2ID("Fire")) return(1);

  // Bei vorherigem Doppel-Stop nur Ablegen
  if(GetPlrDownDouble(GetOwner())) return(0);

  // Steuerung an Effekt weitergeben 
  if(Control2Effect("ControlThrow")) return(1);

  // Anfassen: keine überladene Steuerung
  if(GetAction()S="Push")
    return(0);

  // spezielle Bedienung von Waffen: Waffen einsetzen
  if(IsWeaponReady())
    if(Ctrl2Obj("ControlThrow"))
      return(1);

  // Reiten und Werfen
  if(IsRiding())
    if(Contents(0))
      if(IsRideThrowReady()) // Spezielle Abfrage
        return(SetAction("RideThrow"));

  // Ablegen von Waffen verhindern
  if(GetProcedure() eq "SCALE")
    if(Contents())
      if(Contents()->~IsWeapon())
        return(1);

  // Keine besondere Wurfaktion
  return(0);
}

// Bedienung an Waffen weitergeben, die gerade benutzt werden

public func ControlLeft()
{
  // Weitergeleitete Objektkontrolle
  if(Ctrl2ID("ControlLeft"))
    return(1);

  return(_inherited());
}
public func ControlRight()
{
  // Weitergeleitete Objektkontrolle
  if(Ctrl2ID("ControlRight"))
    return(1);

  return(_inherited());
}
public func ControlUp()
{
  // Weitergeleitete Objektkontrolle
  if(Ctrl2ID("ControlUp"))
    return(1);

  return(_inherited());
}

public func ControlDig()
{
  // Weitergeleitete Objektkontrolle
  if(Ctrl2ID("ControlDig"))
    return(1);

  return(_inherited());
}

public func ControlDown()
{ 
  // Weitergeleitete Objektkontrolle
  if(Ctrl2ID("ControlDown"))
    return(1);

  return(_inherited());
}

public func ControlDownSingle()
{ 
  // Nicht beim Zielen
  if(WildcardMatch(GetAction(),"*Rifle*"))
    return(1);

  return(_inherited());
}

public func ControlDownDouble()
{ 
  // Nicht beim Zielen
  if(WildcardMatch(GetAction(),"*Rifle*"))
    return(1);

  return(_inherited());
}

public func ControlUpdate(object controller, int comdir, bool dig, bool throw)
{
  // Weitergeleitete Objektkontrolle

  // Befehle nur abbrechen, wenn comdir gesetzt  
  var fNoOrderCancel = true;
  if (comdir) fNoOrderCancel = false;
  
  // Fourth parameter is fNoOrderCancel, so there is no possibilty to pass
  // throw. Someone should change this.
  if(Ctrl2ID("ControlUpdate", comdir, dig, fNoOrderCancel))
    return(1);

  return(_inherited(controller, comdir, dig, throw));
}

// Abbruch von bestimmten Aktionen

public func ControlLeftDouble()
{
  // Steuerung an Effekt weitergeben 
  if(Control2Effect("ControlLeftDouble")) return(true);

  // Aktion abbrechen
  return(StopAction());
}

public func ControlRightDouble()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlRightDouble")) return(true);

  // Aktion abbrechen
  return(StopAction());
}

public func ControlSpecial()            // Inventarwechsel
  {
  [$CtrlInventoryDesc$|Image=CXIV]
  // Inventar verschieben
  ShiftContents(0,0,0,1);
  if(Contents(0)) Contents(0)->~Selection();
  return(1);
  }

public func ControlContents()
{
  StopAction();
}

public func ControlCommand(string szCommand,object pTarget,int iX, int iY)
{
  if(Ctrl2ID("ControlCommand",iX,iY)) return(1);
  return(_inherited(szCommand,pTarget,iX,iY, ...));
}

protected func HasContents ()
{
  return(Contents());
}

// Maximale Inventaranzahl
public func MaxContentsCount() { return(3-!!idWeapon); }

public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Der Cowboy kann zusätzlich noch 40 Kugeln tragen
  if(iIndex == 0) { if(fAmount) return(40); return("IsBullet"); }
}

protected func GetObject2Drop(pCollectObj) 
  { 
  var iAmmoPriority; 
  // Wenn ein Munitonspack aufgenommen werden soll... 
  if (pCollectObj) if (ObjectCall(pCollectObj, "IsCartridgePack")) 
    // Munition eine niedrigere Priorität geben 
    iAmmoPriority=-500;
  // Das schlechteste Objekt wegwerfen 
  var pBestDropObj, iBestDropPrio=1000; 
  var pObj, iPrio; 
  var idCollectObj = GetID(pCollectObj); 
  for(var i = 0; pObj = Contents(i); i++) 
    if ((iPrio = GetContentsPriority(pObj, iAmmoPriority, idCollectObj)) < iBestDropPrio) 
      { 
      iBestDropPrio = iPrio; 
      pBestDropObj = pObj; 
      } 
  // Dieses Objekt zurückgeben 
  return(pBestDropObj); 
  } 
 
private func GetContentsPriority(pContents, iAmmoPriority, idCollectObj) 
  { 
  // Munition? Normalerweise hohe Priorität, wenn nicht gerade ein anderes Munitionspaket eingesammelt werden soll 
  if (pContents->~IsCartridgePack()) return(302+iAmmoPriority);
  var idCnt = GetID(pContents);
  // Winchester?
  if (idCnt==WINC)
    {
    // Noch eine Winchester vorhanden? Muss nicht sein
    if (idCollectObj==WINC || ContentsCount(WINC) > 1) return(1);
    // Ansonsten wichtig
    return(216);
    }
  // Whisky? Nur wenn voll!
  if (idCnt==WHKY) if(GetAction(pContents)S="Full") return(212); else return(0);
  // Der Rest kann kaum gebraucht werden; ablegen nach Wert
  return(GetValue(pContents));
  } 

protected func CalcValue()
{
  // Standardwert
  var iValue = _inherited(...);
  // Wert der benutzten Waffe hinzurechnen
  if(idWeapon)
    iValue += GetDefValue(idWeapon);
  // Wert zurückliefern
  return(iValue);  
}

// Der Cowboy braucht einen Sattel
public func NeedSaddle()  
{
  return(1);  
}

/* Aufrufe von Aktionen */

protected func AbortAction() {          // AbortCall von AimRifle
  if(WildcardMatch(GetAction(),"*Rifle*")) return();
  if(!idWeapon) return();
  // Waffe wieder im Inventar erzeugen
  var pWeap = CreateContents(idWeapon);
  idWeapon=0;
  // evtl. Ammo in Waffe laden
  while(iRifleAmmo)
    {
//    CreateContents(CART, Contents());
    var pCross = WINC->GetCrosshair(this());
    while(Contents(0,pCross)) Enter(pWeap, Contents(0,pCross));
    iRifleAmmo--;
    }
}

protected func AimAgain() {         // nach dem Nachladen neu zielen
  if(idWeapon == WINC)
    {
    var obj;
    while(iRifleAmmo < 6 && GetCartridgeCount())
      {
      obj = GetCartridge();
      Enter(WINC->GetCrosshair(this()), obj);
      iRifleAmmo++;
      Sound("RifleLoad2");
      }
    SetAction("AimRifle");
    }

  SetPhase(iAimPhase);
  Sound("RifleLoad2");
}

protected func AimAgainRide() {         // nach dem Nachladen neu zielen (auf Pferd)
  if(idWeapon == WINC)
    {
    var obj;
    while(iRifleAmmo < 6 && GetCartridgeCount())
      {
      obj = GetCartridge();
      Enter(WINC->GetCrosshair(this()), obj);
      iRifleAmmo++;
      Sound("RifleLoad2");
      }
    SetAction("RideAimRifle");
    }

  SetPhase(iAimPhase);
  Sound("RifleLoad2");
}

private func Throwing(pObj) {           // EndCall von Throw
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
  return(1);  
}

/* Prozeduren */

protected func StopAction()         // Abbruch von bestimmten Aktionen
{
  if(WildcardMatch(GetAction(),"Ride*Rifle"))
   { SetComDir(COMD_Stop); SetAction("Ride"); return(1); }
  if(WildcardMatch(GetAction(),    "*Rifle"))
   { SetComDir(COMD_Stop); SetAction("Walk"); return(1); }
}

/* Funktionen */

private func IsWeaponReady()            // ist bereit für das Benutzen von Waffen
{
  return((GetAction() eq "Walk"
      || GetAction() eq "Jump"
      || GetAction() eq "Ride"
      || GetAction() eq "RideStill")
      && !Contained()
  );
}

private func IsRideThrowReady()
{
  return((GetAction() eq "Ride"
      || GetAction() eq "RideStill")
  );
}

private func Riding()
{
  // Richtung an die des Pferdes anpassen
  SetDir(GetDir(GetActionTarget()));
  // Pferd steht still: Clonk soll auch still sitzen
  if (GetActionTarget()->~IsStill() && GetAction() eq "Ride")
      SetAction("RideStill");
  // Pferd steht nicht still: Clonk soll auch nicht still sitzen
  if (!(GetActionTarget()->~IsStill()) && GetAction() eq "RideStill")
      SetAction("Ride");
  return(1);
}

// Springt gerade

public func IsJumping() { return((GetAction()S="Jump") || (GetAction()S="Dive")); }

// Steuerung an Objekt/Definition weitergeben

private func Ctrl2ID(string szCtrl, par1, par2, fNoOrderCancel) {
  // Befehl abbrechen
  if(!fNoOrderCancel) if(GetEffect("Order*", this())) RemoveEffect("Order*", this());
  // An Waffe weiterleiten	
  if(idWeapon)
    if(DefinitionCall(idWeapon,szCtrl,this(),par1,par2))
      return(1);
}

private func Ctrl2Obj(string szCtrl) { if(Contents()) if(ObjectCall(Contents(),szCtrl,this())) return(1); }
  
/*  Munition   */

// Paket aufteilen

/* Patrone aus dem Inventar nehmen */

public func GetCartridge()
  {
  var pObj;	  
  // Patronen in einem Paket suchen
  for(var i = 0; pObj = Contents(i); i++)
    if(pObj->~IsCartridgePack())
      return(pObj->~GetItem());
  // Keine Patronen gefunden
  return(0);
  }

/* Patronen im Inventar zählen */
private func GetCartridgeCount()
  {
    return(GetSpecialCount("IsBullet"));
  }

/* Schusswaffen */

public func FireGun ()
{
  // Waffentyp überprüfen
  if (!Contents()->~IsGun()) return(0);
  // Gehen
  if (GetAction() eq "Walk")
    if (SetAction("DrawGun"))
      return(1);
  // Springen
  if (GetAction() eq "Jump")
    if (SetAction("JumpDrawGun"))
      return(1);
  // Reiten
  if (Or(SEqual(GetAction(),"Ride"),SEqual(GetAction(),"RideStill")))
    // Reitschuss
    if (SetAction("RideDrawGun"))
      return(1);
  // Feuern nicht möglich, aber der Revolver soll nicht weggeworfen werden
  return(1);
}

public func FireRifle ()
{
  // Waffentyp überprüfen
  if (!Contents()->~IsRifle()) return(0);
  // Gehen
  if (GetAction() eq "Walk")
    if (SetAction("AimRifle"))
      return(1, SetPhase(6));
  // Reiten
  if (Or(SEqual(GetAction(),"Ride"),SEqual(GetAction(),"RideStill")))
    {
    // Reitzielen
    if (SetAction("RideAimRifle"))
      {
      if(!GetEffect("RideAimDir", this())) AddEffect("RideAimDir", this(), 99, 2);
      return(1);
      }
    }
  // Feuern nicht möglich
  return(0);
}

private func DrawingGun ()
{
  Sound("RevolverDraw");
  SetPhase(5);
}

private func FiringGun ()
{
  var fire = Contents()->~Fire();
  if (!fire)
    if (GetAction() eq "FireGun")
      return(SetAction("ReplaceGun"));
}

// Sprungschießen
protected func AbortJumpDrawGun(int iPhase)
{
  if(GetAction() eq "Walk") {
    SetAction("DrawGun");
    SetPhase(iPhase);
  }
  return(1);
}

protected func AbortJumpReplaceGun(int iPhase)
{
  if(GetAction() eq "Walk") {
    SetAction("ReplaceGun");
    SetPhase(iPhase);
  }
  return(1);
}

// Nachladen
public func LoadGun()
{
  // Ladeaktion
  if(GetAction() eq "Walk") return(SetAction("LoadGun"));
  if(GetAction() eq "Ride" || GetAction() eq "RideStill") return(SetAction("RideLoadGun"));
}

public func LoadRifle()
{
  // Ladeaktion
  if(GetAction() eq "AimRifle") { Sound("RifleLoad"); return(SetAction("LoadRifle")); }
  if(GetAction() eq "RideAimRifle") { Sound("RifleLoad"); return(SetAction("RideLoadRifle")); }
}

protected func LoadGunEnd()
{
  // Gar kein Revolver mehr da?
  if(!FindContents(REVR)) return();
  // Laden
  FindContents(REVR)->~LoadAmmo();
}

/* Automatische Produktion */

// Der Drugstore soll bei der Objektsuche mit einbezogen werden

public func ControlCommandAcquire(target, x, y, target2, def)
{
  //Falls das Teil rumliegt nur aufsammeln
  var obj = GetAvailableObject (def, target2);
  if (obj) return ();
  //Gebäude suchen worin man's herstellen kann  
  if (obj = GetProducerOf (def)) {
    AddCommand (this (), "Call", this (), 0, 0, 0, 0, "AutoProduction", 0, 1);
    obj -> HowToProduce (this(), def);
    return (1);
  }
  // nächsten Drugstore suchen
  if(!(this()->~IsBandit())) // Nur wenn kein Bandit 
   if(GetBank(0, 1))	  // Nur mit Bank möglich
    if(obj = GetNextDrugstore())
    // Objekt ist im Drugstore verfügbar
      if(obj->~ObjectAvailable (def, GetOwner())) {
      obj -> AutoBuy (this(), def);
      return(1);
    }
}

public func GetNextDrugstore()
{
  // Nur Objekte mit typischen OCFs befragen damit's schneller geht.
  var obj, prod;
  while (obj = FindObject (DRST, -500, -250, 1000, 500, OCF_Fullcon,0,0,0, obj))
    if (!prod || ObjectDistance (prod) > ObjectDistance (obj))
      prod = obj;
  return (prod);
}

/* Portraits */

private func AdjustPortrait(id def)
{
  // Bin ich kein Cowboy?
  if(GetID() != COWB)
    // Mein Portrait stammt auch nicht von mir?
    if(GetPortrait(this(), true) != GetID())
      // Neues Portrait setzen
      {
      if(LocalN("fBoss")) return(SetPortrait("BanditBoss", this(), GetID())); 
      if(LocalN("fCaptain")) return(SetPortrait("Captain1", this(), GetID()));
      var iRandoms = Random(GetPortraitCount()) + 1;
      SetPortrait(Format("%d", iRandoms), this(), GetID());
      }
}

protected func UpdateTransferZone()
{
  // Beim Laden Portrait wiederherstellen
  ScheduleCall(this(), "AdjustPortrait", 1);
}

private func GetPortraitCount() { return(3); }

/* Effekte */

global func FxRideAimDirTimer(target)
{
  // Ziel zielt nicht mehr?
  if(!WildcardMatch(GetAction(target), "*Rifle*") && !WildcardMatch(GetAction(target), "*Bow*"))
    return(-1);
  // Reitet nicht mehr?
  if(!target->~IsRiding()) return(-1);
  // Richtung anpassen
  SetDir(GetDir(GetActionTarget(0,target)),target);
}

/* Befehle und deren Hilfsfunktionen */

local iOrder, pOrdrTarget;
local ordrData1, ordrData2;

public func ContextDefend() 
{ 
  [$CtxDefendDesc$|Image=WINC] 
  // Alten Befehl löschen
  if(GetEffect("Order*", this())) RemoveEffect("Order*", this());
  // Diese Position verteidigen 
  ordrData1 = GetX(); ordrData2 = GetY();
  AddEffect("OrderDefend", this(), 1, 30, this());
  return(1); 
} 

public func ContextArm() 
{  
  [$CtxArmDesc$|Image=AMBO|Condition=IsNotFullyArmed]  
  // Alten Befehl löschen
  if(GetEffect("Order*", this())) RemoveEffect("Order*", this());
  // Waffen suchen und anlegen
  AddEffect("OrderArm", this(), 1, 30, this());
  return(1); 
} 

public func SetOrderNone() 
  { 
  // Alten Befehl löschen
  if(GetEffect("Order*", this())) RemoveEffect("Order*", this());
  return(1); 
  } 

private func OrderFailure(szMessage) 
  { 
  Sound("KnightFailure"); 
  SetOrderNone(); 
  SetCommand(this(),"None"); 
  SetComDir(COMD_Stop()); 
  Message(szMessage,this()); 
  return(1); 
  } 
 
private func OrderSuccess(szMessage, p1, p2, p3) 
  { 
  Sound("KnightDone"); 
  SetOrderNone(); 
  SetCommand(this(),"None"); 
  SetComDir(COMD_Stop()); 
  Message(szMessage,this(), p1, p2, p3); 
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
  // Winchester einsetzen (wenn möglich) 
  if (IsRifleEquipped()) 
    if (ExecuteAttackRifle(pOrdrTarget)) 
      return(1); 
  // Revolver einsetzen (wenn möglich) 
  if (IsGunEquipped()) 
    if (ExecuteAttackGun(pOrdrTarget)) 
      return(1);
  // Direkter Angriff (mit kurzfristiger Laufzeit) 
  StopAction(); 
  AddCommand(this(),"Attack",pOrdrTarget,0,0,0,2); 
  // Weiter 
  return(1); 
}

private func FindEnemyUnit() 
{ 
  var pObj; 
  // Nächstgelegene Mannschaftsobjekte überprüfen 
  while (pObj = FindObject(0, 0,0,-1,-1, OCF_CrewMember(), 0, 0, 0, pObj)) 
    // Verfeindet? 
    if (Hostile(GetOwner(),GetOwner(pObj))) 
      // Feind gefunden 
      return(pObj); 
  // Keine feindliche Einheit gefunden 
  return(0); 
}

private func ExecuteWatch() 
{
  // Schon mal Waffen laden
  var pWeapon;
  if(GetCartridgeCount())
  {
    if(pWeapon = FindContents(WINC)) pWeapon->ControlThrow(this());
    if(idWeapon == WINC && !iRifleAmmo) return(LoadRifle());
    if(pWeapon = FindContents(REVR)) if(!FindContents(pWeapon->GetAmmoID(), pWeapon)) return(LoadGun());
  }
  if (Random(3)) return(1); 
  // Umschauen
  if (GetAction() eq "Walk" || GetAction() eq "AimRifle") 
  {
    SetDir(Random(2));
    var obj = FindObject(WCHR, 0, 0, 0, 0, 0, "Crosshair", this()); 
    if(obj) SetVertexXY(0,-Sin(Local(0,obj),40)*(GetDir()*2-1),Cos(Local(0,obj),40),obj);
  }
  return(1); 
}

//! Trägt der Clonk einen Winchester und hat Munitoion dabei? 
private func IsRifleEquipped() 
{ 
  // Winchester dabei? 
  if (!FindContents(WINC) && idWeapon != WINC) return(0); 
  // Munitoion dabei? 
  if (!GetCartridgeCount()) return(0); 
  // Winchester und Munitoion dabei 
  return(1); 
}

//! Trägt der Clonk einen Revolver und hat Munitoion dabei? 
private func IsGunEquipped() 
{ 
  // Revolver dabei? 
  if (!FindContents(REVR)) return(0); 
  // Munitoion dabei? 
  if (!GetCartridgeCount()) return(0); 
  // Revolver und Munitoion dabei 
  return(1); 
}

private func ExecuteAttackRifle(pTarget) 
  { 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  // Ziel in Reichweite? 
//  if (Abs(iTx-GetX())>300 || Abs(iTy-GetY())>300) 
    // Nicht? Mögliche Abschußposition einnehmen 
//    return( SetCommand(this(),"MoveTo",0, BoundBy(GetX(),iTx-290,iTx+290),GetY()) ); 
  // Kann leider von hier aus nicht zielen 
  if (!IsAiming()) 
    if (!IsWeaponReady()) 
      return(0);
  // Zielen beginnen 
  if(idWeapon != WINC)
  {
	  ShiftContents(0, 0, WINC);	
	  FindContents(WINC)->~ControlThrow(this()); 
  }
  // Ziel nicht im Freien 
  if (Contained(pTarget)) 
    return(0); 
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX(),GetY(), iTx, iTy )) 
    if (!PathFree( GetX()+14*BoundBy(iTx-GetX(), -1, 1),GetY(), iTx, iTy )) 
      return(0); 
  // Schießen 
  //FireBowAt(iTx,iTy, false); 
  WINC->ControlCommand(this(), iTx, iTy, GetAimDevitation());
  // Weitermachen   
  return(1); 
  } 

private func GetAimDevitation() { return(10); }

private func ExecuteAttackGun(pTarget) 
  { 
  // Revolver noch vorhanden? 
  var pGun = FindContents(REVR); if (!pGun) return(0); 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  var iDx = iTx-GetX(); 
  // Ziel in Reichweite? 
  if (Abs(iTy-GetY())>40) 
      // Mögliche Abschußposition einnehmen 
      return(SetCommand( this(),"MoveTo",0, BoundBy(GetX(),iTx-100,iTx+100),GetY() )); 
  // Revolver auswählen 
  ShiftContents(0,0,REVR); 
  // Kann leider von hier aus nicht werfen 
  if (!IsWeaponReady()) return(0); 
  // Ziel nicht im Freien 
  if (Contained(pTarget)) return(0); 
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX(),GetY(), iTx, iTy )) 
        return(0); 
  // Richtung  
  if (iDx>0) SetDir(DIR_Right()); else SetDir(DIR_Left()); 
  // Schießen 
  FindContents(REVR)->ControlThrow(this());
  // Weitermachen   
  return(1); 
  } 

//! Zielt der Clonk gerade mit dem Winchester? 
private func IsAiming() 
{ 
  return (GetAction() eq "AimRifle" || GetAction() eq "RideAimRifle"); 
}

//! Kann der Clonk aus der aktuellen Aktion den Bogen oder eine Wurfwaffe einsetzen? 
public func ReadyToFire() 
{ 
  // Nur beim Laufen - und neu auch beim Reiten... 
  if (GetAction() eq "Walk" || GetAction() eq "Ride" || GetAction() eq "RideStill") return(1); 
  return(0); 
} 

func FxOrderArmTimer(pThis, iNumber) 
  { 
  var pWeapon;	  
  // Als Kavallerist Säbel suchen
  if (!FindContents(SABR) && GetID()==CVRM) 
    { 
    pWeapon = FindAvailableWeapon(SABR); 
    if (pWeapon) return(SetCommand(this(),"Get",pWeapon)); 
    } 
  // Wenn's geht einen Winchester suchen 
  if (!FindContents(WINC) && !FindContents(REVR)) 
    { 
    var idWp1,idWp2; 
    if(Random(2)) { idWp1 = WINC; idWp2 = REVR; }
    else { idWp2 = WINC; idWp1 = REVR; }
    pWeapon = FindAvailableWeapon(idWp1); 
    if (!pWeapon) pWeapon = FindAvailableWeapon(idWp2); 
    if (pWeapon) return(SetCommand(this(),"Get",pWeapon)); 
    } 
  else 
    // Hat eine Waffe: noch Munition dazu suchen 
    if (GetCartridgeCount()<31) 
      if (pWeapon=FindAvailableWeapon(AMBO))
        return(SetCommand(this(),"Get",pWeapon));   
  // Fehlgeschlagen 
  if (!IsFullyArmed()) return(OrderFailure("$MsgNoWeapons$")); // "Keine Waffen." 
  // Fertig   
  return(OrderSuccess("$MsgArmed$")); // "Bewaffnet." 
  } 

// Verfügbare Waffe suchen 
private func FindAvailableWeapon(idOfType) 
  { 
  var pWeapon = FindObject(idOfType,0,0,-1,-1,OCF_Available()); 
  if (ObjectDistance(pWeapon) < 500) return(pWeapon); 
  return(0); 
  } 

//! Der Clonk gilt als bewaffnet, wenn er mindestens eine Feuerwaffe und Munition trägt.
private func IsFullyArmed() 
{ 
  return ( (FindContents(WINC)||FindContents(REVR)) && GetCartridgeCount()); 
} 
 
//! Nicht voll bewaffnet. 
public func IsNotFullyArmed()  
{  
  return (!IsFullyArmed());  
}

// Ist ein Cowboy
public func IsCowboy() { return(1); }
