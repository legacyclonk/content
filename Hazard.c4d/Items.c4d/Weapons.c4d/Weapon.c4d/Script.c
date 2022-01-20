/*-- Waffe --*/

#strict 

/* Konstanten */
//Feuermodi-Konstanten
static const FM_Name      = 1;  // Name des Feuermodus'

static const FM_AmmoID    = 2;  // ID der benutzten Munition
static const FM_AmmoLoad  = 3;  // Menge der eingeladenen Munition
static const FM_AmmoUsage = 4;  // Munitionsverbrauch pro AmmoRate Schüsse
static const FM_AmmoRate  = 5;  // s.o.

static const FM_Reload    = 6;  // Nachladezeit der Waffe in Frames.
static const FM_Recharge  = 7;  // Zeitabstand zwischen zwei Schüssen in Frames.

static const FM_Auto      = 8;  // Automatische Waffe.
static const FM_Aim       = 9;  // Waffe zielt
static const FM_Condition =10;  // Vorraussetzung für den Schussmodus

static const FM_Icon      =11;  // Icon das im Schussmodimenü angezeigt wird

static const FM_Damage    =12;  // Schaden, den der Schussmodus anrichtet

static const FM_Error     =-1;  // Für auftretende Fehler

// Bot-Konstanten für Feuermodi
static const BOT_Range    = 100;  // Reichweite des Modus
static const BOT_DmgType  = 101;  // Schadenstyp, den der Modus hauptsächlich anrichtet

static const BOT_Ballistic= 102;  // Der Modus schießt ballistische Geschosse

static const BOT_Power    = 103;  // Stärke der Waffe
  static const BOT_Power_1 =  1;  // geringe Stärke (Pistole, Pistolen-Granatwerfer, Pistole-Laser, Pistole-Schleim, EMP)
  static const BOT_Power_2 =  2;  // mittlere Stärke (Pumpgun, Energiegewehr, Flammenwerfer, Napalmgranate, Lasergranate)
  static const BOT_Power_3 =  3;  // hohe Stärke (Granatwerfer: Normal, Cluster & Schleim, Motorsäge, Energiegewehr mit Laser, Biowerfer, Clusterpumpgun, Feuerball)
  static const BOT_Power_LongLoad = 4;
                                  // Waffen, mit einer langen Nachladezeit (Partikelk., Bazooka, Minigun)
                                  // Diese Waffen werden nur leer geschossen und dann erstmal nicht benutzt
static const BOT_EMP      = 104;  // EMP-Waffe. Wird nur gegen Maschinen eingesetzt

public func Default(int data)    // Standardeinstellungen
{
  if(data == FM_Name)      return("Standard");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoUsage) return(1);
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Auto)      return(false);
  if(data == FM_Aim)       return(0);
  if(data == FM_Condition) return(true);
  if(data == FM_AmmoLoad)  return(5);
  if(data == FM_Reload)    return(100);
  if(data == FM_Recharge)  return(10);
  if(data == FM_Icon)      return(GetID());
  if(data == FM_Damage)    return(0);
  if(data == BOT_Range)    return(100);
  if(data == BOT_DmgType)  return(DMG_Projectile);
  if(data == BOT_Ballistic)return(0);
  if(data == BOT_Power)    return(BOT_Power_1);
  if(data == BOT_EMP)      return(false);
}

public func HandX()    { return(0); }    // X-Position in der Hand
public func HandY()    { return(0); }    // Y-Position in der Hand
public func HandSize() { return(1000); } // Größe in der Hand, Standard: 1000
public func HandBarrel(){return(0); }    // Y-Offset des Laufs

public func CanAim() { return(true); }   // mit diesem Gegenstand kann man zielen

/* Callbacks an Objekte die WEPN includieren */
// i ist dabei firemode

public func OnEmpty(int i)     { } // wird aufgerufen wenn Waffe leer (keine Munition)
public func OnSelect(int i)    { } // wird bei Anwahl im Inventar aufgerufen
public func OnDeselect(int i)  { } // wird bei Abwahl im Inventar aufgerufen

public func OnReload(int i)    { } // wird bei Start des Nachladens aufgerufen
public func OnReloaded(int i)  { } // wird nach erfolgreichem Nachladen aufgerufen

public func OnAutoStart(int i) { } // wird beim Starten einer autom. Waffe aufgerufen (erster Schuss)
public func OnAutoStop(int i)  { } // wird beim Stoppen einer atuom. Waffe aufgerufen

public func OnAimStart(int i)  { } // Anfangen zu zielen
public func OnAimStop(int i)   { } // Stoppen zu zielen

public func IsUpgradeable(id i){ } // Upgradebar?
public func OnUpgrade(id i)    { } // Beim Upgraden
public func OnDowngrade(id i)  { } // Beim Downgraden

/* Lokale Variablen */

local firemode;             // aktueller Feuermodus

local ratecount, stopauto, shooting;  // intern...

local controller;           // the object which controls the weapon (the hazard clonk)

local upgrades;							// Welche Upgrades wir haben

protected func Initialize() {
  firemode = 1;
  ratecount = GetFMData(FM_AmmoRate);
  stopauto = false;
  upgrades = CreateArray();
  SetUser();
}

/* Feuern */

public func SetUser(object pObj)
{
	if(pObj != controller)
	{
		controller = pObj;
		if(controller) UpdateHUD();
	}
}
public func GetUser() { return(controller); }

public func ControlUpdate(object caller, int comdir, bool dig, bool throw)
{
  SetUser(caller);

  // autom. Schuss beenden, wenn Werfen losgelassen (JumpAndRun)
  if(IsRecharging() && !throw)
    StopAutoFire();
  // Nicht nachladen, wenn die Munition ausging und der Controller rechtzeitig
  // werfen wieder loslässt.  
  else if(!throw)
    ClearScheduleCall(this(), "Reload");
}

public func ControlThrow(caller)
{
  SetUser(caller);
  
  // autom. Schuss beenden, wenn erneut Werfen gedrückt (klassisch)
  if(IsRecharging())
  {
    if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
      StopAutoFire();

    return(1);
  }

  // Unterstützt der Schussmodus Zielen, aber wir tuns nicht?
  if(GetFMData(FM_Aim)>0 && !(GetUser()->~IsAiming()) && !(GetUser()->~AimOverride()))
  {
    // Können wir?
    if(GetUser()->~ReadyToAim())
      // Auf gehts:
      GetUser()->StartAiming();
  
    // Nachladen?
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    // weil keine Muni mehr da war? -> nachladen wenn geht
    if(!CheckAmmo(ammoid,ammousage,this()))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      // nicht genügend Munition
      else {
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
    // Sonst nix.
    return(1);
  }

  // Feuern! Fehlgeschlagen?
  if(!Fire()) {

    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    // weil keine Muni mehr da war? -> nachladen wenn geht
    if(!CheckAmmo(ammoid,ammousage,this()))
      if(CheckAmmo(ammoid,ammousage,GetUser())) {
        Reload();
      }
      // nicht genügend Munition
      else {
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
  }

  return(1);
}

public func Fire()
{

  // bereit zum Schießen
  if(!(GetUser()->~ReadyToFire())) return(false); 

  // lädt grad nach... bitte nicht stören
  if(IsReloading()) return(false);

  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);
  // leer?
  if(!CheckAmmo(ammoid,ammousage,this())) {
    OnEmpty(firemode);
    return(false);
  }

  // Callback
  if(GetFMData(FM_Auto))
    OnAutoStart(firemode);

  // FEUAAA!!!1 OMGOMG
  Shoot();

  return(true);
}

/* Waffenmenü */

public func ControlDigDouble(caller)
{
  SetUser(caller);

  var name;
  // Das Menü für die Schussmodiauswahl
  CreateMenu(GetID(), GetUser(), 0, 0, "$FireMode$", C4MN_Extra_Info, 0, true);
  // Solang suchen, bis keine Schussmodi mehr da sind
  for(var i = 1; name=GetFMData(FM_Name, i); i++ )
  {
    // Vorraussetzung muss erfüllt sein
    if(GetFMData(FM_Condition, i))
    {
      var id = GetFMData(FM_AmmoID, i);
      var iconid = GetFMData(FM_Icon, i);
      var obj = CreateObject(iconid);
      // Wir haben ein Objekt fürs Menü und legen die Ammografik drauf
      SetGraphics(0, obj, id, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(650,0,5000,0,650,5000, obj, 1);
      
      // Eintragstext
      var entryname=Format("%s (%s)", name, GetName(0, id));
      
      // Genug Ammo für zumindest einen Schuss vorhanden sein
      if(!CheckAmmo(id,GetFMData(FM_AmmoUsage,i),GetUser(),this()))
      {
        // Name des Eintrags wird grau
        entryname=Format("<c %x>%s</c>", RGB(100,100,100), entryname);
        // Eintrag grau
        obj->SetClrModulation(RGB(100,100,100));
      }
      // Eintrag hinzufügen.
      AddMenuItem(entryname, "ChangeFireMode", GetID(this()), GetUser(), 0, i, Format("$DescFireMode$", name), 4, obj); 
      // Objekte natürlich wieder löschen
      RemoveObject(obj);
    }
  }
  return(true);
}

/* Nachladen/Feuermodus ändern */

private func Failure(dummy, i)
{
  // Geht nicht: Bescheidsagen
  var id2=GetFMData(FM_AmmoID, i);
  PlayerMessage(GetOwner(GetUser()), "$NotEnoughAmmo$", GetUser(),id2);
  OnEmpty(i);
}

public func GetFireMode() { return(firemode); }

private func ChangeFireMode(dummy, i)
{
  // Immer noch genügend Munition da?
  if(!CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoUsage,i),GetUser(),this()))
  {
    // Nein. Mecker.
    Failure(dummy, i);
    // Menü will geupdatet werden, wahrscheinlich
    var item=GetMenuSelection(GetUser());
    CloseMenu(GetUser());
    ControlDigDouble(GetUser());
    SelectMenuItem (item, GetUser());
    // Fertig.
    return(); 
  }
  CloseMenu(GetUser());
  var old = GetFireMode();
  // Schussmodus umstellen
  if(SetFireMode(i))
  {
    OnAutoStop(old);
    // Danach neue Munition einladen.
    Reload();
  }
}

private func SetFireMode(int i) {
  if(i == 0) return();

  // Gleicher Modus: Nur nachladen wenn nicht mehr voll und lädt nicht nach
  if(i == firemode) {
    if(CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoLoad),this())) return(); 
    if(IsReloading()) return();
  }
  // Alte Munition ausladen
  Empty();
  // Schussmodus umstellen
  firemode=i;
  stopauto=false;
  ratecount = GetFMData(FM_AmmoRate, i);
  
  // Helpmessage
  if(GetUser())
    HelpMessage(GetUser()->GetOwner(),"$FireModeChanged$",GetUser(),GetFMData(FM_Name),GetFMData(FM_AmmoID));
  
  return(1);
}

public func Reload()    // Waffe nachladen
{
  ResumeReload();
  if(IsReloading()) return(false);

  // Verzögerung? Abbrechen
  if(IsRecharging()) RemoveEffect("Recharge", this());

  // nicht genug Ammo um nachzuladen
  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);
  if(!CheckAmmo(ammoid,ammousage,GetUser()))
    return(false);

  // entleeren falls noch voll
  if(CheckAmmo(ammoid,ammousage,this()))
    Empty();

  if(GetFMData(FM_Auto))
    OnAutoStop(firemode);
  OnReload(firemode);
  var reloadtime = GetFMData(FM_Reload)*MaxReloadAmount(GetUser())/GetFMData(FM_AmmoLoad);

  // Mit dem Nachladen beginnen
  AddEffect("Reload", this(), 1, 1, this(), 0, Max(1, reloadtime));
  return(true);
}

public func CancelReload()  // Nachladen abbrechen
{
  if(!IsReloading()) return(false);
  RemoveEffect("Reload", this(),0,true);
  return(true);
}

public func PauseReload() // Nachladen pausieren
{
  if(!IsReloading()) return(false);
  if(EffectVar(1,this(),GetEffect("Reload",this())) == 0) return(false);
  EffectVar(1,this(),GetEffect("Reload",this())) = 0;
  return(true);
}

public func ResumeReload()  // Nachladen wiederaufnehmen (wenn pausiert)
{
  if(!IsReloading()) return(false);
  if(EffectVar(1,this(),GetEffect("Reload",this())) == 1) return(false);
  EffectVar(1,this(),GetEffect("Reload",this())) = 1;
  return(true);
}

private func Recharge()    // Wartezeit zwischen zwei SchÃ¼ssen
{
  var rechargetime = GetFMData(FM_Recharge);
  AddEffect("Recharge", this(), 1, 1, this(),0,1+Max(1, rechargetime));
}

public func Empty()    // Waffe ausleeren
{
  // Laden wir nach? Abbrechen.
  if(IsReloading()) RemoveEffect("Reload", this());
  if(IsRecharging()) RemoveEffect("Recharge", this());

  // Munitionsart wo raus muss
  var ammoid = GetFMData(FM_AmmoID);
  // Zu schiebende Munitionsmenge
  var ammoamount = GetAmmo(ammoid, this());
  // Clonk wiedergeben 
  DoAmmo(ammoid, ammoamount, GetUser());
  // Hier entfernen
  DoAmmo(ammoid, -ammoamount, this());
}

private func Reloaded(caller)   // Waffe nachgeladen
{

  // Munitionsart wo rein muss
  var ammoid = GetFMData(FM_AmmoID);
  var ammoamount = MaxReloadAmount(caller);

  // zwischendurch Ammo zuviel ammo verbraucht? Fehlschlag.
  if(!CheckAmmo(ammoid,ammoamount,caller))
    return(false);

  // Hier einfügen
  DoAmmo(ammoid, ammoamount, this());
  // Dem Clonk abziehen
  DoAmmo(ammoid, -ammoamount, caller);

  HelpMessage(caller->GetOwner(),"$Reloaded$",caller,ammoamount,ammoid);


  // Callback
  OnReloaded(firemode);
}

/* Upgradezeug */

// ‬Waffe mit uid Upgraden
public func Upgrade(id uid)
{
  // ist die Waffe damit überhaupt upgradebar?
  if(!IsUpgradeable(uid))
    // nichts tun
    return(false);
  
  upgrades[GetLength(upgrades)] = uid;
  OnUpgrade(uid);
  return(true);
}

public func SetUpgradeGraphic(int bitsWithGraphic)
{
	var myBits = GetUpgradeGraphicBitmask();
	if(bitsWithGraphic) myBits &= bitsWithGraphic;
	
	var graphicsString = 0;
	if(myBits) graphicsString = Format("Upgrade%d",myBits);
	
	SetGraphics(graphicsString);
}

public func GetUpgradeGraphicBitmask()
{
	var bitmask = 0;
	for(var upgradeId in upgrades)
	{
		bitmask += upgradeId->~GetUpgradeGraphicsIDBit();
	}	
	return bitmask;
}

// Upgrade uid entfernen
public func Downgrade(id uid)
{
  // haben wir das Upgrade überhaupt?
  if(!GetUpgrade(uid))
    return(false);
  
  for(var i = 0; i < GetLength(upgrades); i++)
    if(upgrades[i] == uid)
    {
      upgrades[i] = 0;
      for(var j = i; j < GetLength(upgrades)-1; j++)
        upgrades[j] = upgrades[j+1];
      
      OnDowngrade(uid);
      if(Contained())
        CreateContents(uid,Contained());
      else
        CreateObject(uid);
      return(true);
    }
  
  return(false);
}

// Ist waffe mit uid upgegradet?
public func GetUpgrade(id uid)
{
  for(var upg in upgrades)
    if(upg == uid)
      return(true);
      
  return(false);
} 

/* Feuern */

private func Shoot(object caller) {          // Feuern mit Feuermodus
  //User da? :S
  if(!GetUser())
    return(stopauto=true);

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim)>0 && !(GetUser()->~AimOverride()))
    return(stopauto=true); //abbrechen
  // user hat sich irgendwo reinbewegt 
  if(GetUser()->Contained() && GetUser() == Contained())
    return(stopauto=true);
  // Feuern...
  if(Call(Format("Fire%d",firemode)) == FM_Error)
  	return(stopauto=true);
  	
  if(GetFMData(FM_Auto))
    shooting = true;

  if(!GetUser()) return();
	
  // Munition abziehen
  if(ratecount == 1) {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-muni);
    ratecount = GetFMData(FM_AmmoRate);
  }
  else { --ratecount; }

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else {
    shooting = false;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
    	if(!IsReloading())
      	ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  UpdateHUD();
}

public func IsShooting() { return(shooting); }

public func GetFMData(int data, int i)
{
  // Vom Feuermodus i Information holen
  // i nicht angegeben? Muss nicht, ist aktueller dann
  if(!i) i=firemode;
  var value = ObjectCall(this(), Format("FMData%d",i), data);
  // Modifikationen
  var effect,user = GetUser(),j;
  while(effect = GetEffect("*Bonus*",this(),j) || j == 0) {
    j++;
    if(!GetEffect("*Bonus*",this(),effect,1))
      continue;
    var tval = EffectCall(this(),effect,"FMData",data,value);
    if(tval)
      value = tval;
  }
  if(user)
  {
    for(var i; i <= GetEffectCount("*Bonus*",user); i++) {
      var tval = EffectCall(user,GetEffect("*Bonus*",user,i-1),"FMData",data,value,i);
      if(tval)
        value = tval;
    }
    /*
    while(effect = GetEffect("*Bonus*", user, j) || j == 0)
    {
      j++;
      if(!GetEffect("*Bonus*",user,effect,1))
       continue;
      Log("%s",GetEffect("*Bonus*",user,effect,1));
      var tval = EffectCall(user,effect,"FMData",data,value);
      if(tval)
        value = tval;
    }*/
  }
  return(value);
}

// Informationen für Bots (je nach Feuermodus)
public func GetBotData(int data, int i)
{
  // Vom Feuermodus i Information holen
  if(!i) i=firemode;
  return(ObjectCall(this(), Format("BotData%d",i), data));
}

protected func Deselection(object pContainer)
{
  // Callback
  OnDeselect(firemode);
  // Laden stoppen
  PauseReload();
  //Automatischen Schuss stoppen
  if(GetFMData(FM_Auto))
	{
		if(IsRecharging()) stopauto=true; 
		OnAutoStop();
	}
}

protected func Selection(object pContainer)
{
  // Callback
  OnSelect(firemode);
  // Laden wiederaufnehmen
  ResumeReload();
}

public func StopAutoFire() {
  stopauto = true;
}

public func AimStart() { OnAimStart(firemode); }

public func AimStop() { 
  // automatisch feuern aus
  if(IsRecharging())
    StopAutoFire();
  OnAimStop(firemode);
}

protected func Departure()  // Waffe weggeworfen: Nachladen abbrechen. :(
{
  if(GetFMData(FM_Auto))
    if(!stopauto)
      OnAutoStop(firemode);
  if(IsReloading())
    RemoveEffect("Reload", this(), 0, true);

  SetUser();
}

protected func Entrance(object pContainer)
{
  SetUser(pContainer);
}

public func IsWeapon() { return(true); }
public func NoWeaponChoice() { return(GetID() == WEPN); }

public func GetCharge() {   // wie voll ist die Waffe
  var charge;
  var ammoid = GetFMData(FM_AmmoID);
  // lädt nach: Nachladestatus anzeigen
  if(IsReloading()) {
    // Nachladestatus in %
    charge = 1000*(EffectVar(0, this(), GetEffect("Reload", this())))/GetFMData(FM_Reload);
  }
  // ansonsten: Ladestand anzeigen
  else {
    charge = 1000*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad);
  }

  return(charge);
}

private func MaxReloadAmount(object pAmmostore) {
  var ammoid = GetFMData(FM_AmmoID);
  var ammoload = GetFMData(FM_AmmoLoad);
  var ammousage = GetFMData(FM_AmmoUsage);
  // Zu erwartende Munitionsmenge errechnen...
  if(!CheckAmmo(ammoid,ammoload,pAmmostore))
    ammoload = GetAmmo(ammoid,pAmmostore);
  ammoload /= ammousage;
  ammoload *= ammousage;

  return(ammoload);
}

public func IsReloading() { return(GetEffect("Reload",this())); }
public func IsRecharging() { return(GetEffect("Recharge", this())); }

public func GetRecharge() {
  if(!IsRecharging())
    return(100);
  var time = GetEffect("Recharge",this(),0,6);
  return((100*time)/GetFMData(FM_Recharge));
}

/* Effekte */

public func OnAmmoChange()
{
	UpdateHUD();
}

public func FxReloadStart(object pTarget, int iNumber, int iTemp, int iTime) {
  if(iTemp) return;

  UpdateHUD();
	
  // Nachladen aktiv:
  EffectVar(1,pTarget,iNumber) = 1;

  // Zeit speichern
  EffectVar(2,pTarget,iNumber) = iTime;
}

public func FxReloadTimer(object pTarget, int iNumber, int iTime) {

  if(EffectVar(1,pTarget,iNumber))
  {
		UpdateHUD();
    if(EffectVar(0,pTarget,iNumber)++ >= EffectVar(2,pTarget,iNumber)) return(-1);
  }
}

public func FxReloadStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  // nicht nachladen
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return(0);
  // temporäre Aufrufe ignorieren
  if(fTemp) return();
  Message(" ",pTarget);
  // nur erfolgreich nachgeladen wenn Timer abgelaufen
  if(iReason == 0)
    pTarget->Reloaded(GetUser());
  
  UpdateHUD();
}

public func FxRechargeStart(object pTarget, int iNumber, int iTemp, int iTime) {
  if(iTemp) return;

	UpdateHUD();
	
  // Rechargetime speichern
  EffectVar(0,pTarget,iNumber) = iTime;
}

public func FxRechargeTimer(object pTarget, int iNumber, int iTime) {
  UpdateHUD();
  if(iTime >= EffectVar(0,pTarget,iNumber)) return -1;
}

public func FxRechargeStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  // Waffenträger weg?
  if(!GetUser()) return(0);
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return(0);
  // automatisch weiterschießen, mit JnR-Control auch bei normalen Waffen
  if(GetFMData(FM_Auto) || GetPlrCoreJumpAndRunControl(pTarget->GetController())) {
    // ... außer wenn abgebrochen oder keine Munition mehr (!)
    if(stopauto || (GetUser()->Contents() != this() && GetUser() == Contained(this())) || !(GetUser()->ReadyToFire()) || !CheckAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoUsage))) {
      // Callback bei AutoFire
      if(GetFMData(FM_Auto))
        OnAutoStop(firemode);

      stopauto = false;
      shooting = false;
    }
    else {
      Shoot(pTarget);
    }
  }
  UpdateHUD();
}

private func UpdateHUD()
{
	if(GetUser())	GetUser()->~UpdateCharge();
}

/* globale Funktionen */

global func HelpMessage(int iPlr, string szMsg, object pTarget, a,b,c,d,e,f,g)
{
  // Nur ausgeben, wenn auch erwünscht
  if(GetPlrExtraData(iPlr, "Hazard_NoHelpMsg")) return(0);
  // Hallo Neuling! Hier deine Info!
  return(PlayerMessage(iPlr, szMsg, pTarget, a,b,c,d,e,f,g));
}

/*  Waffenrotationszeugs und so :)   */

/* Von Sven kopiertes und geändertes Zeugs */
// Edit: Inzwischen selbst neu gecodet. Sven2->RemoveObject();
// mit Extravielen Kommentaren für Nachtschatten aka Clonk Nukem!!!!
//   jede Zeile nen Kommentar?? Hälst du CN für doof, boni? ^^ - newton
//     er hat gesagt er will viele Kommentare, also bekommt er sie. ;) - boni

// EffectVars:
//  0 - ID der Waffe
//  1 - Aktuelle Drehung
//  2 - X-Position des Anfangs der Waffe
//  3 - Y-Position des Anfangs der Waffe
//  4 - X-Position des Ende des Laufs
//  5 - Y-Position des Ende des Laufs
//  6 - Aktuelle Waffe

//  7 - Dir für Performance-Einsparungs-Zwecke
//  8 - Winkel für Performance-Einsparungs-Zwecke

//  9 - Skalierungsfaktor der Waffe (HandSize)
// 10 - ObjektBreite/2
// 11 - BarrelXOffset
// 12 - BarrelXOffset

global func FxShowWeaponStart(object pTarget, int iNumber, int iTemp)
{
  if (iTemp) return(FX_OK);
  // Erste Grafikaktualisierung
  FxShowWeaponTimer(pTarget, iNumber);
  return (FX_OK);
}

global func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
  // Waffe aktualisieren:
  var xoff, yoff, r;  // Offset, Winkel
  // kein Inventar oder falsche Aktion
  if(!Contents(0,pTarget)) {
		EffectCall(pTarget,iNumber,"Reset");
    return(FX_OK);
  }
  
  //Die Waffe momentan überhaupt anzeigen?
  if(!(pTarget->~WeaponAt(xoff, yoff, r)))
  {
		EffectCall(pTarget,iNumber,"Reset");
    return(FX_OK);
  }
  
  var obj = Contents(0,pTarget), id=GetID(obj);
  // Waffe nicht mehr aktuell
  if(EffectVar(0, pTarget, iNumber) != id) {
    // neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
    if(obj->~IsWeapon() || obj->~IsDrawable())
    {
      SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
      
      // neue Daten
      EffectVar(0, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      EffectVar(7, pTarget, iNumber) = 0;
      EffectVar(8, pTarget, iNumber) = 0;
      EffectVar(9, pTarget, iNumber) = id->~HandSize();
      EffectVar(10, pTarget, iNumber) = GetDefCoreVal("Width",0,id)/2;
      EffectVar(11, pTarget, iNumber) = DefinitionCall(id,"BarrelXOffset");
      EffectVar(12, pTarget, iNumber) = DefinitionCall(id,"BarrelYOffset");
      
      if(!EffectVar(9, pTarget, iNumber))
      	EffectVar(9, pTarget, iNumber) = 1000;
    }
    // neues Objekt ist keine Waffe
    else {
			EffectCall(pTarget,iNumber,"Reset");
      return(FX_OK);
    }
  }

  id = EffectVar(0, pTarget, iNumber);
  obj = EffectVar(6, pTarget, iNumber);

  // Ausrichtung nach Blickrichtung des Clonks
  // Variablen für die Transformation

  var width, height;  // Breiten- und Höhenverzerrung der Waffe
  var xskew, yskew;   // Zerrung der Waffe, wird zur Rotation gebraucht
  var size;           // Größe der Waffe in der Hand: 1000 = 100%
  // Variablen für die Position
  var xaim, yaim;     // Offset, dass sich durch zielen ergibt
  var dir;            // Richtung in die das Objekt schaut
  
  //schnell noch Rotation dazurechnen oder so!
  r += ObjectCall(obj,"HandR");
  
  // Variablen mit Werten versehen
  dir  = GetDir()*2-1;
  if(r > 180 || r < -180)
    dir *= -1;
  r *= dir;

  // Müssen wir überhaupt updaten? Wir gehen davon aus, dass sich HandX/Y nicht ändert, wenn sich die Rotation auch nicht ändert.
  if(EffectVar(8, pTarget, iNumber) == r && EffectVar(7, pTarget, iNumber) == dir)
  	return(FX_OK);
  else
  {
  	EffectVar(8, pTarget, iNumber) = r;
  	EffectVar(7, pTarget, iNumber) = dir;
  }


  width = height = xskew = yskew = 1;
  size = EffectVar(9, pTarget, iNumber);


  var xfact = size * ObjectCall(obj,"HandX");    // Attachpunkte dazurechnen
  var yfact = size * ObjectCall(obj,"HandY");

  xoff += Cos(r,xfact)/1000 + dir*Sin(r,yfact)/1000;
  yoff -= Cos(r,yfact)/1000 - dir*Sin(r,xfact)/1000;

  if(dir == 1) 
  {
    height = -1;
    xskew = -1;
    yskew = -1;
  }

  r = -90*dir-r-90;
  height *= width *= Cos(r, size);
  xskew *= Sin(r, size);
  yskew *= -xskew;
  xoff *= dir;

  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer); //position
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj); //Größe und Rotation

  // abspeichern, damit abrufbar
  r = -r-90;
//  var w,brly,brlx,r2,dist;
  var w = EffectVar(10, pTarget, iNumber);
  var brlx = EffectVar(11, pTarget, iNumber);
  var brly = EffectVar(12, pTarget, iNumber);
  var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
  var dist = Distance(0,0,w*1000-brlx,brly);
  //Log("%d - %d - %d - %d",w,brl,r2,dist);
  
  var v2 = xoff-Sin(r,size*w);
  var v3 = yoff+Cos(r,size*w);
  var v4 = xoff+Sin(r+r2,size*(dist))/1000;
  var v5 = yoff-Cos(r+r2,size*(dist))/1000;
  if(EffectVar(1, pTarget, iNumber) != r)
	  EffectVar(1, pTarget, iNumber) = r;
  if(EffectVar(2, pTarget, iNumber) != v2)
	  EffectVar(2, pTarget, iNumber) = v2;
  if(EffectVar(3, pTarget, iNumber) != v3)
	  EffectVar(3, pTarget, iNumber) = v3;
	if(EffectVar(4, pTarget, iNumber) != v4)
	  EffectVar(4, pTarget, iNumber) = v4;
	if(EffectVar(5, pTarget, iNumber) != v5)
	  EffectVar(5, pTarget, iNumber) = v5;
  
  //EffectVar(4, pTarget, iNumber) = xoff+Sin(r,size*(w));	
  //EffectVar(5, pTarget, iNumber) = yoff-Cos(r,size*(w));
  //Log("%d / %d",EffectVar(4, pTarget, iNumber),EffectVar(5, pTarget, iNumber));
}
  
global func FxShowWeaponReset(object pTarget, int iNumber)
{
  if(EffectVar(0, pTarget, iNumber) != 0)
  {
	  EffectVar(0, pTarget, iNumber) = 0;
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
  }
  if(EffectVar(6, pTarget, iNumber))
  {
    SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
    EffectVar(6, pTarget, iNumber) = 0;
  }
}
  
global func FxShowWeaponStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  if (fTemp) return(FX_OK);
  // Grafik entfernen
  SetGraphics(0, pTarget, 0, WeaponDrawLayer);
  
  if(EffectVar(6,pTarget,iNumber))
	SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
}  
