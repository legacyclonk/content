/*---- HazardClonk ----*/

#strict
#include CLNK

#include L_DR	// can have drone (with context menu)
#include L_GE	// can use gear (with context menu)
#include L_LA	// can climb on ladders	
#include L_AG	// is agil
#include L_JP	// can use jetpack
#include L_CA	// can aim

static const WeaponDrawLayer = 1; //Layer in dem die Waffen gezeichnet werden

local ammobag;
local wpneffect;

local LastDmgType;

/* Initialisierung */

protected func Construction()
{
  aMacroCommandList = CreateArray();
  _inherited();
}

protected func Initialize()
{
  //dieser Effekt ist für die Waffen in den Händen zuständig
  wpneffect = AddEffect("ShowWeapon",this(),1,1,this(),HZCK);
  // Magieanzeige ist hier die Munitionsanzeige - nicht mehr
//  SetPhysical("Magic",100000,2);
  //Energie fixen (kA wo das gechanged wird...) UH H4X
  DoEnergy( -GetPhysical("Energy") / 1000 + 1); //Noch 1 Energie
  SetPhysical("Energy", 150000, 2); //Physical ändern
  DoEnergy( GetPhysical("Energy") / 100 - 1); // Neu dazugeben
  // Munitionsgürtel
  AmmoStoring();
	var hud = GetHUD(this());
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,0);

	// Verzögertes hud-update
	if(hud)
		ScheduleCall(this,"UpdateCharge",1);
	
  return(inherited());
}

public func IsThreat() { return(true); }

/* Rekrutierung */

protected func Recruitment()
{
  ResetPortrait(this,true);
  return(_inherited(...) );
}

// Anzahl Standardportraits
public func StdPortraitCount() { return(3); }

global func ResetPortrait(object pObj, bool fPermanent)
{
  // Zur Netzwerk-Sicherheit
  var iRandomPortrait = Random(pObj->~StdPortraitCount()) + 1;
  // Mal schauen, was fuer ein Portrait wir haben
  var portrait = GetPortrait(pObj, 0, 1);
  var portrait_id = GetPortrait(pObj, 1, 1);
  // Ist es ein eigenes Portrait? Dann lassen wir es.
  if(portrait S= "custom") return;
  // wenn kein valides normales Hazard-Portrait (nicht ArmoredX), reset
  if(!portrait_id || portrait_id == NONE || portrait_id != pObj->GetID() || !WildcardMatch(portrait, "?"))
  {
    SetPortrait(Format("%d", iRandomPortrait), pObj, pObj->GetID(), fPermanent, false);
	}
	else
	{
		SetPortrait(portrait, pObj, portrait_id, fPermanent, false);
	}
}


/* Steuerung Richtung (an Inhalt weitergeben, sonst internen Befehl ausführen) */

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  if(inherited(clonk, comdir, dig, throw)) return(1);
  if(Control2Grab("ControlUpdate", comdir, dig, throw)) return(0);
  if(Control2Contents("ControlUpdate", comdir, dig, throw)) return(1);
  if(ControlAim("ControlUpdate", comdir, dig, throw)) return(1);
  if(ControlLadder("ControlUpdate", comdir, dig, throw)) return(1);
}

protected func ControlUp() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlUp");

  if (ControlAim("ControlUp")) return(1);
  if (inherited()) return(1); // reiten und effektkontrolle
  if (Control2Grab("ControlUp")) return(0);
  if (Control2Contents("ControlUp") ) return(1);
  if (ControlLadder("ControlUp") ) return(1);
  if (ControlAgility("ControlUp") ) return(1);
}

protected func ControlUpDouble() {

  if (ControlAim("ControlUp")) return(1);
  if (_inherited()) return(1); // riding and control2effect
  if (Control2Grab("ControlUpDouble")) return(0);
  if (Control2Contents("ControlUpDouble") ) return(1);
  if (ControlJetpack("ControlUpDouble") ) return(1);
  if (ControlAgility("ControlUpDouble") ) return(1);
}

protected func ControlDown() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlDown");

  if (ControlAim("ControlDown")) return(1);
  if (inherited()) return(1); // reiten und effektkontrolle
  if (Control2Grab("ControlDown")) return(0);
  if (Control2Contents("ControlDown") ) return(1);
  if (ControlLadder("ControlDown") ) return(1);
  if(Contents(0))
    if (GetPlrDownDouble(GetOwner()))
		if(ReadyToSquatAim())
    {
         StartSquatAiming(); 
         return(1);
    }
}

private func ReadyToSquatAim() {
	return(	!GetEffect("SquatAimTimeout") 
		&&	Contents(0)->~CanAim()
		&& !IsAiming()
		&& (GetAction() eq "WalkArmed" || GetAction() eq "Walk"));
}

protected func ControlLeft() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlLeft");

  if (ControlAim("ControlLeft")) return(1);
  if (_inherited()) return(1); // riding and control2effect
  if (Control2Grab("ControlLeft")) return(0);
  if (Control2Contents("ControlLeft")) return(1);
  if (ControlLadder("ControlLeft") ) return(1);
  if (ControlJetpack("ControlLeft") ) return(1);
  if (ControlAgility("ControlLeft") ) return(1);
}

protected func ControlRight() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlRight");

  if (ControlAim("ControlRight")) return(1);
  if (_inherited()) return(1); // riding and control2effect
  if (Control2Grab("ControlRight")) return(0);
  if (Control2Contents("ControlRight")) return(1);
  if (ControlLadder("ControlRight") ) return(1);
  if (ControlJetpack("ControlRight") ) return(1);
  if (ControlAgility("ControlRight") ) return(1);
}

protected func ControlLeftDouble() {

  if(IsHealing()) {
	  StopHealing();
	  return(1);
  }
  if (ControlAim("ControlLeftDouble")) return(1);
  if (_inherited()) return(1); // riding and control2effect
  if (Control2Grab("ControlLeftDouble")) return(0);
  if (Control2Contents("ControlLeftDouble")) return(1);
  if (ControlJetpack("ControlLeftDouble") ) return(1);
  if (ControlAgility("ControlLeftDouble") ) return(1);
}

protected func ControlRightDouble() {

  if(IsHealing()) {
	  StopHealing();
	  return(1);
  }
  if (ControlAim("ControlRightDouble")) return(1);
  if (_inherited()) return(1); // riding and control2effect
  if (Control2Grab("ControlRightDouble")) return(0);
  if (Control2Contents("ControlRightDouble")) return(1);
  if (ControlJetpack("ControlRightDouble") ) return(1);
  if (ControlAgility("ControlRightDouble") ) return(1);
}

protected func ControlDigDouble() {
	ClearMacroCommands();
  
  if (_inherited()) return(1); // riding and control2effect
  if (Control2Grab("ControlDigDouble")) return(0);
  if (Control2Contents("ControlDigDouble")) return(1);
}

protected func ControlDigSingle() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlDig");
  
  if (_inherited()) return(1); // riding and control2effect
  if (Control2Grab("ControlDig")) return(0);
  if (Control2Contents("ControlDig")) return(1);
  if (ControlLadder("ControlDig") ) return(1);
}

protected func ContactBottom() {
  if (GetAction() S= "Swim")
    SetAction("Walk");
  if (GetAction() S= "SwimArmed")
    SetAction("WalkArmed");
  return(1);
}

/* Steuerung Werfen */

protected func ControlThrow() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlThrow");

  // Bei vorherigem Doppel-Stop nur Ablegen  
  if ( GetPlrDownDouble(GetOwner()) )
  {
    AddEffect("SquatAimTimeout", this(), 1, 15, this());
    return(inherited());
  }
  // Steuerung an gerittenes Objekt weitergeben (macht der CLNK nicht)
  if(IsRiding())
    if(GetActionTarget()->~ControlThrow(this()))
      return(1); 

  if (Control2Grab("ControlThrow")) return(0);
  if (Control2Contents("ControlThrow") ) return(1);
  if (ControlLadder("ControlThrow") ) return(1);

  return(inherited());
}

/* Steuerung Inhalt */

protected func ControlSpecial()
  {
  [$CtrlInventoryDesc$|Image=INVT]
  if(!Contents()) return();

  ClearMacroCommands();

  //ControlSpecial an Container weitergeben (z.B. Fahrzeuge)
  if(Contained() && Contained()->~ContainedSpecial(this))
    return true;
	
	//Manche Sachen dürfen einfach nicht
	if(Contents()->~RejectShift())
		return;
  // wenn wir zielen, wollen wir nur Waffen haben
  if(IsAiming() && Contents(0)->~CanAim())
  {
  	// nächste Waffe suchen
  	for(var i = 1; i < ContentsCount(); i++)
  		if(Contents(i)->~CanAim())
  		{
  			// zur Waffe wechseln
  			ShiftContents(0,0,Contents(i)->GetID(),true);
  			break;
  		}
  }
  else
	  // Inventory verschieben
  	ShiftContents(0,0,0,1);
		
  UpdateCharge();
	CheckArmed();
}

protected func Ejection(object pObj)      // Wegwerfen
{
  // Nach Waffe suchen
  CheckArmed();
  // Objekt benachrichtigen
  if(Contents(0)) Contents(0)->~Selection();
  // Munitionsanzeige updaten
  UpdateCharge();
	return _inherited(pObj);
}

protected func Collection2(object pObj)           // Einsammeln
{
	if(pObj->~RejectShift() && pObj != Contents(0))
	{
		ShiftContents(0,0,pObj->GetID(),true);
	}
	else
	{
		// das neue Item nach hinten verschieben (außer es ist Ammo)
		if(!(pObj->~IsAmmoPacket()) || NoAmmo())
			if (1 == ContentsCount(GetID(pObj)))
				if(!(pObj->GetOCF() & OCF_Living))
				{
					ShiftContents(0,0,0,0);
					pObj ->~ OnDeselect();
				}
	}
  UpdateCharge();
	CheckArmed();
  return _inherited(pObj);
}

public func AmmoTransfered(object pAP) {
  AddEffect("AmmoCollection",this(),100,10,this(),0,pAP->AmmoID(),pAP->AmmoCount());
}


protected func RejectCollect(id idObj, object pObj)
{
  // Für die KI
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(EffectVar(0, pObj, effect) == this())
      return(1);
  // Spawnpunkt-Hack
  if(idObj == SPNP) return();
	
  //Spielzielgegenstände immer aufsammeln
  if(pObj->~IsAlwaysCollectable())
    return false;
	
  // Munitionspaket?
  if(pObj ->~ IsAmmoPacket())
    // Davon kann man in jeden Fall _eines_ im Inventar haben
    if(!CustomContentsCount("IsAmmoPacket"))
      return(0);
  //Waffe?
  if(pObj ->~ IsWeapon()) {
    //Sonderbehandlung?
    if(!(pObj ->~ OnCollection(this()))) {
      //nein. Standardprozedur:
      //schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
      if(ContentsCount(idObj) || CustomContentsCount("IsWeapon") >= WeaponCollectionLimit())
        return(1);  //Ja, nicht aufnehmen
      else
        return(0);
    }
    else
      return(0);
  }
  // Einsammellimit für Ausrüstung
  if(pObj ->~ IsEquipment() && ContentsCount(idObj) > 0)
    return(1);
  //Wieviel haben wir denn schon im inventar?
  if(ContentsCount() - CustomContentsCount("IsWeapon") >= ObjectCollectionLimit())
    return(1);
  
  // nicht angelegte Ausrüstung nochmal aufsammeln
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return(1);
  
  // Ok
  return(0);
}

protected func Hangling() {
  PauseReloading();
  if(!GetEffect("ScaleReloading",this()))
    AddEffect("ScaleReloading",this(),1,1,this(),HZCK);
}

protected func Scaling()
{
  PauseReloading();
  if(!GetEffect("ScaleReloading",this()))
    AddEffect("ScaleReloading",this(),1,1,this(),HZCK);
  return(_inherited());
}

func FxScaleReloadingTimer()
{
  if(!Contents())
    return();
  if(!WildcardMatch(GetAction(),"*Scale*") && !WildcardMatch(GetAction(), "*Hangle*"))
  {
    ResumeReloading();
    return(-1);
  }
  if(Contents()->~IsReloading())
  	PauseReloading();
}

public func PauseReloading() {
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if(c->GetFMData(FM_Aim) > 0)
        c->PauseReload();
}

public func ResumeReloading() {
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if(c->GetFMData(FM_Aim) > 0)
        c->ResumeReload();
}

public func CustomContentsCount(string funcname)
{
  // Alle Inventarobjekte durchlaufen und Waffen zählen
  for(var i=0,j,obj ; obj = Contents(i) ; i++)
    if(ObjectCall(obj, funcname))
      j++;
  // Waffen gezählt
  return(j);
}

public func WeaponCollectionLimit() { return(3); } // max. Waffen im Inventar
public func ObjectCollectionLimit() { return(2); } // max. Objekte im Inventar

protected func Departure(pContainer)      // Gebäude verlassen
{
  // nach Waffe suchen
  CheckArmed();
  UpdateCharge();
}

protected func ContentsDestruction(object pObj)      // Wenn Inhaltsobjekte verschwinden
{
	if(pObj == Contents(0))
	{
		pObj->Exit(); // lösch dich gefälligst draußen!
		// damit können entsprechende Calls jetzt schon in diesem Objekt gemacht werden
		CheckArmed();
		if(Contents(0)) Contents(0)->~Selection(this);
		UpdateCharge();
	}
}

protected func ControlContents(idTarget)
{
  // Zielobjekt will gar nicht, dass wir was machen
  if(idTarget->~NoControlContents()) return();
  // BR-Bombe etc. darf man nicht abwählen
  if(Contents())
    if(Contents()->~RejectShift())
      return(1);

  // controlel Aim -> aufhören zu zielen außer die nächste Waffe kann es
  // Siehe ChangeWeapon
  if(ControlAim("ControlContents",idTarget)) return(1);

  // Hast du noch einen letzten Wunsch, Contents(0)??!
  if(Contents(0)) Contents(0)->~Deselection(this());
  // Rotieren
  if (!ShiftContents(0, 0, idTarget)) return(1);
  // Waffe ziehen/wegstecken
  CheckArmed();
  // Objekt benachrichtigen
  if(Contents(0)) Contents(0)->~Selection(this());
  // Munitionsanzeige updaten
  UpdateCharge();
}

private func ChangeWeapon(object pTarget) {
    // Zielaktion anpassen
    if(pTarget->~IsWeapon())  {
      var phase = GetPhase();
      if(pTarget->~GetFMData(FM_Aim) == 2 || pTarget->~GetFMData(FM_Aim) == -1)
        SetAction("AimSquatLow");
      else
        SetAction("AimSquat");
        
      SetPhase(phase);
    }
    else {
      StopAiming();
    }
}

/* Leitet die Steuerung an das erste Inhaltsobjekt weiter. Rückgabewert 1 wenn erfolgreich */

private func Control2Contents (string command)
{
  // Haben wir was angefasst?
  if(GetAction() S= "Push")
    return(0);
  // Pause Reload: nicht wieder anfangen ey!!!
  if(command S= "ControlThrow") {
	if(WildcardMatch(GetAction(),"Scale*") || GetAction() S= "Hangle")
	  return(1);
  }
	
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(ObjectCall(Contents(), command, this(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7)))
    return(1);
  return(0);
}

/* Und mit angefassten Objekten auch noch */

private func Control2Grab ()
{
  // Haben wir was angefasst?
  if(GetAction() eq "Push")
    return(1);

}

/* Spezielles Leiterverhalten */

public func ReleaseLadderUp() {
	//nach oben abspringen
	ReleaseLadder(-15*(GetDir()*2-1),-25);
}

/* Maussteuerung */

protected func ControlCommand(string szCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj) 
{
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlCommand");

  // Context -> Keine Sonderbehandlung 
  if(szCommand S= "Context") { return(0); }

  if (szCommand S= "MoveTo")
	{ 
    if (Control2Grab("ControlCommand") || IsRiding())
		{
			// Spezieller ControlCommand an das angefasst Objekt mit Info welches Objekt es aufruft (letzter Param)
			if(GetActionTarget())
			{
				if(GetActionTarget()->~ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, iData, pCmdObj, this))
					return(1);
			}
		}
		if (ControlAim("ControlCommand",Par(1),Par(2),Par(3))) return(1);
    // Klettert an einer Leiter
    if (ControlLadder("ControlCommand")) return(0);
  }
  // Sonst nicht abfangen 
  return(_inherited(szCommand, pTarget, iTx, iTy, pTarget2, iData, pCmdObj) ); 
} 

/* Kontextmenü */

protected func ContextHelpMessagesOn(object pCaller) {
  [$CtxHelpMessagesOn$|Image=CXIN|Condition=HelpMessagesOff]
  SetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg", false);
  Sound("Click", 1, 0,0, GetOwner()+1);
}

protected func ContextHelpMessagesOff(object pCaller) {
  [$CtxHelpMessagesOff$|Image=CXIN|Condition=HelpMessagesOn]
  SetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg", true);
  Sound("Click", 1, 0,0, GetOwner()+1);
}

public func HelpMessagesOn() { return(!GetPlrExtraData(GetOwner(), "Hazard_NoHelpMsg")); }
public func HelpMessagesOff() { return(!HelpMessagesOn()); }

/*
only used as aimbot; there are no scenarios where it legitimally makes sense

protected func ContextGuard(object pCaller) {
  [$CtxGuard$|Image=FLA2|Condition=Outside]
  // Bewachen
  SetAggroLevel(Aggro_Guard);
  Sound("Confirm*");
}
*/

public func Outside() { return(!Contained()); }

protected func ContextDropAmmo(object pCaller)
{
  [$AmmoDrop$|Image=STAP|Condition=AmmoBagContextCheck]

  CreateMenu(STAP,this,this,C4MN_Extra_None,"$AmmoDrop$",0,C4MN_Style_Context);

  for(var ammo in GetDroppableAmmoPacks())
  {
    var ammoPackSize = ammo->AmmoPackID()->AmmoCount();
    AddMenuItem(ammo->GetName(), "DropAmmoPack", ammo->GetID(), this, ammoPackSize, ammoPackSize);
  }
}

protected func DropAmmoPack(id ammo, int amount)
{
  if(!CheckAmmo(ammo, amount)) return false;

  DoAmmo(ammo, -amount);
  CreateObject(ammo->AmmoPackID(),0,0);
}

private func GetDroppableAmmoPacks()
{
  var result = CreateArray();
  if(!ammobag) return result;

  var ammo, i = 0, j = 0;
  while(ammo = ammobag->Contents(i++))
  {
    if(!ammo->~IsAmmo()) continue;
    var ammoPackSize = ammo->AmmoPackID()->AmmoCount();
    if(GetAmmo(ammo->GetID()) < ammoPackSize) continue;
    result[j++] = ammo;
  }
  return result;
}

private func AmmoBagContextCheck()
{
  if(FindObject(NOAM)) return false;
  return GetLength(GetDroppableAmmoPacks()) > 0;
}


/* Gestorben */

protected func Death(int iKilledBy) {
  var plr = GetOwner();

// HIER GEHÖRT KEIN INHERITED HIN
// das macht die ganze aufrufkette kaputt!
//  _inherited();

  // Info-Broadcasts für sterbende Clonks
  GameCallEx("OnClonkDeath", this(), iKilledBy);
	
  // Sound und Meldung
  Sound("Die");
  DeathAnnounce(GetOwner(),this(),GetKiller());
	NoDeathAnnounce(); // nicht nochmal aufrufen
  // Letztes Mannschaftsmitglied func tot() { neuer Einsatz
  
  if(GetPlayerType(GetOwner()) == C4PT_Script)
    GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  else
  {
    var gotcrew;
    for(var i; i < GetCrewCount(plr); i++)
      if(GetOCF(GetCrew(plr,i)) & OCF_Alive)
        gotcrew = true;
  
    if(!gotcrew)
      GameCallEx("RelaunchPlayer",GetOwner(),this(), GetKiller());
  }

  if(ammobag)
    RemoveObject(ammobag);

  if(GetEffect(0,this(),wpneffect))
    RemoveEffect(0,this(),wpneffect);

  FadeOut(this());
	
  if(HasGear()) TakeOffGear();
	
  return(1);
}

public func NoDeathAnnounce() // Hält 1 Frame lang, also Clonk schnell töten, dann kommt keine Nachricht
{
	AddEffect("NoAnnounce", this, 1, 1, this);
}

public func FxNoAnnounceStop(dummy, dummy2, int iReason)
{
	if(iReason == 4) return -1;
}

private func DeathAnnounce(int plr, object clonk, int killplr) {
  if(GetEffect("NoAnnounce", this)) return;

  if(killplr == -1)
    return(inherited(plr, clonk, killplr));
  
  if(!clonk)
  	return(inherited(plr, clonk, killplr));
  
  //Selfkill?
  if(plr == killplr)
    HHKS->SKMsg(plr);
  else
    HHKS->KTMsg(killplr, plr, clonk->~LastDamageType());
  
  //Killstatistik.
  //Zwar ungenau wenn mehrere Clonks eines Spielers im Spiel sind,
  //aber wann ist das schon der Fall?
  HHKS->KillStat(GetCursor(killplr),plr);
  
  inherited(plr, clonk, killplr);
}

protected func Destruction() {
  // Vorher töten, damit Relaunch ausgeführt wird.
  if(GetAlive(this)) Kill();
  if(ammobag) RemoveObject(ammobag);
  return _inherited(...);
}

/* Waffe/Feuern */

public func ReadyToFire() {     // nur aus bestimmten Aktionen feuern

   var a = GetAction();

   // In Gebäude
   if(Contained()) return(Contained()->~ReadyToFire());
   // Reitet
   if(IsRiding()) return(GetActionTarget()->~ReadyToFire());
   // Schiebt
   if(GetProcedure() eq "PUSH")
   	if(GetActionTarget()->~IsWeapon())
   		return(true);

   // Nur beim Laufen, Schwimmen oder Springen
   if(a eq "WalkArmed"
   || a eq "SwimArmed"
   || a eq "JumpArmed")
		return(true);
   
   // JetpackFlight fehlt noch, sowie Aim*... das steht in den Funktionalitäten
   return(_inherited());
}

private func CheckArmed() {     // Bewaffnung prÃ¼fen
  if (GetAction() eq "Walk") if (IsArmed2()) return(SetAction("WalkArmed"));
  if (GetAction() eq "Jump") if (IsArmed2()) return(SetAction("JumpArmed"));
  if (GetAction() eq "Swim") if (IsArmed2()) return(SetAction("SwimArmed"));
  if (GetAction() eq "WalkArmed") if (!IsArmed2()) return(SetAction("Walk"));
  if (GetAction() eq "JumpArmed") if (!IsArmed2()) return(SetAction("Jump"));
  if (GetAction() eq "SwimArmed") if (!IsArmed2()) return(SetAction("Swim"));
}

// Der Clonk ist bewaffnet (auch mit einer Waffe die nicht angezeigt wird)
// also Waffen mit denen man Zielen muss
public func IsArmed() {
  if(!Contents()) return(); 
  if(Contents()->~IsWeapon())
    return(true);
  return(false);
}
// Der Clonk hat einen Gegenstand in der Hand, der gezeichnet wird
public func IsArmed2() {
  if(!Contents()) return();
  if(Contents()->~IsDrawable() ||
    (IsArmed() && (Contents()->~GetFMData(FM_Aim) <= 0)))
    return(true);
  return(false);

}

public func JumpStart(bool bBackflip)
{
  // bewaffnet?
  CheckArmed();

  _inherited(bBackflip);
}

protected func ComNone()
  {
  SetComDir(COMD_None);
  }

private func IsJumping() {
  if(GetAction() S= "Dive")
    return(true);
  if(WildcardMatch(GetAction(),"Jump*"))
		return true;
	return _inherited();
}

public func StartSquatAiming() {     // Anfangen in der Hocke zu zielen
  // Abbrechen
  if(Contained()) return;
  if(GetAction() ne "WalkArmed" && GetAction() ne "Walk") return;

  SetXDir(0);

  if(Contents(0)->~GetFMData(FM_Aim) == 2 || Contents(0)->~GetFMData(FM_Aim) == -1)
    SetAction("AimSquatLow");
  else
    SetAction("AimSquat");

  SetPhase(AimAngle2Phase(90));
  SetComDir(COMD_Stop());

  InitCrosshair();
  
  if(!GetEffect("UpdateAiming", this))
    AddEffect("UpdateAiming", this, 1, 1, this);
 
  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

public func IsSquatAiming () { return(WildcardMatch(GetAction(), "AimSquat*")); }

public func AimOverride () {
	if(Contained()) return(Contained()->~AimOverride());
	
	return(_inherited());
}

public func GetTarget(int iAngle, int iMaxAngle, int iRange)
{
  // Parameter zurechtstutzen
  while(iAngle<0) iAngle+=360;
  while(iAngle>360) iAngle-=360;
  if(!iMaxAngle) iMaxAngle = 0;
  if(!iRange) iRange = 350;

  // Landschaft durchsuchen
  // es wird nur auf Lebewesen gezielt
  var targets = FindTargets(this, iRange, iMaxAngle, iAngle, false);
  var target;
  
  // nächstes Ziel finden...
	for(var t in targets) {
		if(!target) {
			target = t;
			continue;
		}
		if(ObjectDistance(t) < ObjectDistance(target)) {
			target = t;
			break;
		}
	}
  
  if(target) DebugLog("Target is %s","hazard",GetName(target));
  return(target);
}

/* Magieanzeige updaten */

// Auch Updaten wenn wir Fahrzeuge/Geschütztürme/etc bedienen
func Grab(object pTarget, bool fGrab)
{
	UpdateCharge();
	CheckArmed();
}

func Entrance() { UpdateCharge(); }

func CrewSelection(bool fDeselect, boolfCursorOnly)
{
	UpdateCharge();
}

public func UpdateCharge() 
{
  // Nur wenn ich Cursor bin
  if(GetCursor(GetOwner()) != this) return;

  if(GetOwner() < NO_OWNER) return;

  // in Gebäuden/Fahrzeugen
  if(Contained())
    if(Contained()->~UpdateCharge(this))
      return true;

  // reitet
  if(IsRiding())
    if(GetActionTarget()->~UpdateCharge(this))
      return true;

	var weapon = Contents();
		
  // an angefasstes Objekt weiterleiten
  if(GetAction() S= "Push")
	{
		var at = GetActionTarget();
	  if(at && at->~IsWeapon())
			if(at->GetUser() == this)
				weapon = at;
	}
		
  // HUD
  var hud = GetHUD(this);
  if(hud) hud->Update(weapon, AmmoStoring(),this); // <- performanzfresser!

  return(1);
}

public func AmmoStoring()
{
  // keiner da: neuen erstellen
  if(!ammobag) {
    ammobag = CreateObject(ABAG);
    //ammobag->Init(this());
  }
  return(ammobag);
}

public func OnAmmoChange(id AmmoID, int NewAmount)
{
	UpdateCharge();
}

/* Heilung */

public func StopHealing() {      // Heilen beenden
  SetComDir(COMD_Stop());
  SetAction("Walk");
  CheckArmed();
  Sound("Grab");
  while(GetEffect("*Heal*", this()))
    RemoveEffect("*Heal*", this());
}

// Heilt sich mit einem Medikit o.ä.
public func IsHealing () { return(WildcardMatch(GetAction(), "Heal*")); }

/* Bonis Testbereich */
//Mit dem betreten dieses Bereiches begeben sie sich in die Gefahrenzone.
//Weiterlesen nur mit Schutzhelm und funktionierendem Phaser erlaubt.

public func WeaponAt(&x, &y, &r) {

  if(Contained()) return(0);

  var a = GetAction();
  var p = GetPhase();

  // x < 0: Richtung hinter den Clonk
  // x > 0: Richtung vor den Clonk
  x=0; y=0; r=0;

  // hält Waffe in der Hand
  if(a eq "WalkArmed") {
    x=0;
    y=1000;
    return(1);
  }
  
  else if(a eq "JumpArmed") {
    x=500-125*p;
    y=1000;
    return(1);
  }
  else if(a eq "SwimArmed") {
    x=4000;
    return(1);
  }
  //Jetpack
  else if(a eq "JetpackFlight") {
  
    x=4000;
    r=35;
    return(1);
  }


  // zielt mit der Waffe
  else if(IsAiming()) {
    //if(p>4) r=(p-4)*11;
    //else    r=p*23-90;
    r = (Abs(crosshair->GetAngle())-90);
    x=-Sin(65+p*20,4000)-1500;
    y=-Sin(55+p*10,5000);
    if(a eq "AimLow")
      y += 4000;
    if(a eq "AimSquat")
    {
      x += 5000;
      y += 4500;
    }
    if(a eq "AimSquatLow")
    {
      x += 5000;
      y += 6000;
    }
    return(1);
  }
/*
  // hat Waffe auf dem Rücken
  else {
    r=90;
    x=-3000;
    y=-2000;

    if(a eq "Walk") {
      y-=2000;
    }
    else if(a eq "Jump") {
      y-=2000;
    }
    else if(a eq "Scale" || a eq "ScaleDown") {
      x-=1000-Cos(p*45,250);
    }
    else if(a eq "Hangle") {
      x-=Cos(p*32,750);
    }
    else if(a eq "Push") {
      r=100;
      x+=1000-Cos(p*90,350);
      y-=2000;
    }
    else if(a eq "Swim") {
      r=135;
      x=-1000+Sin(p*22,500);
      y=-4000-Cos(p*22,250);
    }
    else if(a eq "Dive") {
      r=135+p*10;
      x=-1000;
      y=-4000;
    }
    else */return(0);

/*    return(1);
  }*/
}

public func WeaponBegin(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(2, this(), number)/1000;
  y = EffectVar(3, this(), number)/1000;
}

public func WeaponEnd(&x, &y) {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  x = EffectVar(4, this(), number)/1000;
  y = EffectVar(5, this(), number)/1000;
}

public func GetWeaponR() {
  var number = GetEffect("ShowWeapon",this());
  if(!number)
    return(0);
  return(EffectVar(1, this(), number));
}

/* Munitionsaufsammelanzeige */

func FxAmmoCollectionStart(object pTarget, int iEffectNumber, int iTemp, id idAmmo, int iAmmoCount) {
  if(iTemp)
    return(1);
  EffectVar(0,pTarget,iEffectNumber) = 100;
  EffectVar(1,pTarget,iEffectNumber) = idAmmo;
  EffectVar(2,pTarget,iEffectNumber) = iAmmoCount;
}

func FxAmmoCollectionEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, id idAmmo, int iAmmoCount) {
  if(szNewEffectName S= "AmmoCollection")
    //return(-2);
    // wird nich addiert: alten löschen
    RemoveEffect(0,pTarget,iEffectNumber);
  return(0);
}

func FxAmmoCollectionAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, id idAmmo, int iAmmoCount) {
  var x=1;
  while(EffectVar(x,pTarget,iEffectNumber)) {
    if(EffectVar(x,pTarget,iEffectNumber) == idAmmo)
      break;
    else
      x++;
  }
  EffectVar(x++,pTarget,iEffectNumber) = idAmmo;
  EffectVar(x,pTarget,iEffectNumber) += iAmmoCount;

  EffectVar(0,pTarget,iEffectNumber) += 100;
  return(1);
}

func FxAmmoCollectionTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  var x=1, string = " ";
  while(EffectVar(x,pTarget,iEffectNumber))
    string = Format("%s|{{%i}} %4d",string,EffectVar(x++,pTarget,iEffectNumber),EffectVar(x++,pTarget,iEffectNumber));
  
  HelpMessage(GetOwner(pTarget),string,pTarget);
  if(iEffectTime >= EffectVar(0,pTarget,iEffectNumber))
    return(-1);
}

public func Incineration(int iPlr) {
  // Hazardclonks brennen ncht wirklich ;)
  Extinguish();
  AddFireEffect(this(),30,FIRE_Red,1);
	AddEffect("FireDamage",this,100,1,this,0,30,5,iPlr);
}

protected func FxFireDamageStart(object pTarget, int iEffectNumber, int iTemp, int iDuration, int iDamage, int iPlr)
{
	if(iTemp) return;
  EffectVar(0,pTarget,iEffectNumber) = iDuration;
  EffectVar(1,pTarget,iEffectNumber) = iDamage;
  EffectVar(2,pTarget,iEffectNumber) = iPlr;
}

protected func FxFireDamageTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	DoDmg(EffectVar(1,pTarget,iEffectNumber), DMG_Fire, pTarget, 1, EffectVar(2,pTarget,iEffectNumber)+1);
	
  if(EffectVar(0,pTarget,iEffectNumber) < iEffectTime)
    return -1;
}

protected func FxFireDamageEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  if(szNewEffectName S= "FireDamage") return -2;
}

protected func FxFireDamageAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, int iDuration, int iDamage, int iPlr)
{
  EffectVar(0,pTarget,iEffectNumber) = iDuration;
  EffectVar(1,pTarget,iEffectNumber) = iDamage;
	EffectVar(2,pTarget,iEffectNumber) = iPlr;
}

public func OnDmg(int iDamage, int iType) {
	// Ausrüstung will vielleicht mitreden...
	var before = _inherited(iDamage, iType);
	
	var mod = 0;
	if(iType == DMG_Fire)		mod = -5;
	if(iType == DMG_Energy)		mod = 25;
	if(iType == DMG_Bio)		mod = -10;
	
	return(mod+before);
}

public func OnHit(int iDmg, int iType, object pFrom)
{
  _inherited(iDmg, iType, pFrom);
  
  if(!GetAlive()) return();
  
  // Besondere Effekte
  if(iType == DMG_Bio)
    AddFireEffect(this(),10,RGB(20,255,30));
	
  // hurt
  if(iDmg > 12)
	  Hurt();
	
  // Aggro-Check
  if(pFrom)
    if(LocalN("shooter", pFrom))
      if(GetAggroLevel() > Aggro_Nothing)
        if(!GetAggroTarget())
          if(CheckTarget(LocalN("shooter", pFrom)))
            SetAggroTarget(LocalN("shooter", pFrom));
}

public func LastDamageType(int type) {
  if(type)
    LastDmgType = type;
  
  return(LastDmgType);
}

/* KI-Zeugs */

// Array mit Effektnummern
// Effektvariablen:
// 0: Kommandoname (string)
// 1: Zielobjekt (objekt)
// 2: Ziel-X (int)
// 3: Ziel-Y (int)
// 4: Verzögerung (int)
// 5: Callbackobjekt (object)
// 6: Aggrolevel
local aMacroCommandList;
// Wegpunktearray
local aPath;

// iAggro = -1 bedeutet keine Änderung am Aggrolevel
// Einen Makrobefehl setzen
public func SetMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{
  // kein ordentlicher Befehl -> nichts tun
  if(!szCommand) return();
  if(szCommand ne "MoveTo" && szCommand ne "Follow" && szCommand ne "Wait" && szCommand ne "None")
  {
    ErrorLog("Unknown macro command: %s", szCommand);
    return();
  }
  // Parameter evtl. vorbearbeiten oder besondere Checks
  if(szCommand eq "MoveTo")
    if(pTarget)
    {
      iX = pTarget->GetX();
      iY = pTarget->GetY();
    }
  if(szCommand eq "Follow")
    if(!pTarget)
    {
      ErrorLog("Too few arguments for FOLLOW (no target object)");
      return();
    }
  // Aktuelle Beschäftigung beenden
  var currCom = GetMacroCommand(0,0);
  if(currCom)
  {
    ClearScheduleCall(this(), Format("MacroCom%s", currCom));
    SetCommand(this(), "None");
    SetComDir(COMD_Stop);
  }
  // Liste löschen
  //Log("%s #%d: ClearMacroCom by SetMacroCom", GetName(), ObjectNumber());
  ClearMacroCommands();
  // Befehl eintragen
  var iEffect = AddEffect("MacroCommand", this(), 300, 0, this());
  aMacroCommandList[0] = iEffect;
  EffectVar(0, this(), iEffect) = szCommand;
  EffectVar(1, this(), iEffect) = pTarget;
  EffectVar(2, this(), iEffect) = iX;
  EffectVar(3, this(), iEffect) = iY;
  EffectVar(4, this(), iEffect) = iDelay;
  EffectVar(5, this(), iEffect) = pCallback;
  EffectVar(6, this(), iEffect) = iAggro;
  // Ausführung beantragen
  ClearScheduleCall(this(), "MacroCommandMoveTo");
  ClearScheduleCall(this(), "MacroCommandFollow");
  ClearScheduleCall(this(), "MacroCommandWait");
  //Log("%s #%d: SetMacroCom: '%s' %d/%d Target: %s #%d", GetName(), ObjectNumber(), szCommand, iX, iY, GetName(pTarget), ObjectNumber(pTarget));
  if(szCommand != "None")
    ScheduleCall(this(), Format("MacroCom%s", szCommand), 1);
  else
  {
    ClearMacroCommands();
    if(iAggro != -1) SetAggroLevel(iAggro,0,0,0,"SetMacroCommand");
  }
  return(1);
}

// Befehl an den Anfang setzen
public func AddMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{//Log("%s #%d: AddMacroCom", GetName(), ObjectNumber());
  // Befehlsliste zwischenspeichern
  var aComs = aMacroCommandList;
  // Und leeren
  var currCom = GetMacroCommand(0,0);
  if(currCom)
  {
    ClearScheduleCall(this(), Format("MacroCom%s", currCom));
    SetCommand(this(), "None");
    SetComDir(COMD_Stop);
  }
  aMacroCommandList = CreateArray();
  // Kommando sollen nicht gelöscht werden
  AddEffect("MacroCommandLocked", this(), 301);
  // Befehl einsetzen
  var ret = SetMacroCommand(pCallback, szCommand, pTarget, iX, iY, iDelay, iAggro);
  // Andere Befehle wieder eintragen
  if(ret)
  {
    for(var iCom in aComs)
      if(iCom)
        aMacroCommandList[GetLength(aMacroCommandList)] = iCom;
  }
  else
    aMacroCommandList = aComs;
  // Kommandos freigeben
  RemoveEffect("MacroCommandLocked", this());
  // Rückgabe
  return(ret);
}

// Befehl ans Ende setzen
public func AppendMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{
  // kein ordentlicher Befehl -> nichts tun
  if(!szCommand) return();
  if(szCommand ne "MoveTo" && szCommand ne "Follow" && szCommand ne "Wait" && szCommand ne "None")
  {
    ErrorLog("Unknown macro command: %s", szCommand);
    return();
  }
  // Parameter evtl. vorbearbeiten oder besondere Checks
  if(szCommand eq "MoveTo")
    if(pTarget)
    {
      iX = pTarget->GetX();
      iY = pTarget->GetY();
    }
  if(szCommand eq "Follow")
    if(!pTarget)
    {
      ErrorLog("Too few arguments for FOLLOW (no target object)");
      return();
    }
  // Befehl eintragen
  var iEffect = AddEffect("MacroCommand", this(), 300, 0, this());
  aMacroCommandList[GetLength(aMacroCommandList)] = iEffect;
  EffectVar(0, this(), iEffect) = szCommand;
  EffectVar(1, this(), iEffect) = pTarget;
  EffectVar(2, this(), iEffect) = iX;
  EffectVar(3, this(), iEffect) = iY;
  EffectVar(4, this(), iEffect) = iDelay;
  EffectVar(5, this(), iEffect) = pCallback;
  EffectVar(6, this(), iEffect) = iAggro;
  // Starten?
  if(GetLength(aMacroCommandList) == 1)
  {
    if(szCommand != "None")
      Call(Format("MacroCom%s", szCommand));
    else
      ClearMacroCommands();
  }
  return(1);
}

// Abfrage
public func GetMacroCommand(int iCommandNum, int iElement)
{
  // An der Stelle gibt es nichts?
  if(iCommandNum < 0 || GetLength(aMacroCommandList) <= iCommandNum) return();
  // Element zurückgeben
  return(EffectVar(iElement, this(), aMacroCommandList[iCommandNum]));
}

// Beenden (wenn, dann nur extern gebraucht)
public func FinishMacroCommand(bool fNoCallback, bool fNoSuccess, int iCommandNum)
{
  // An der Stelle gibt es nichts?
  if(iCommandNum < 0 || GetLength(aMacroCommandList) <= iCommandNum) return();
  // mit Callback -> Callback machen
  if(!fNoCallback)
  {
    if(!fNoSuccess)
      return(MacroComSuccess(iCommandNum));
    else
      return(MacroComSuccessFailed(iCommandNum));
  }
  // Ohne Callback -> kein Callback ;)
  RemoveMacroCommand(iCommandNum);
  // Nächstes Kommando starten
  if(GetMacroCommand() && !iCommandNum)
    if(GetMacroCommand() != "None")
      Call(Format("MacroCom%s", GetMacroCommand()));
  return(1);
}

// Löscht alle Makrobefehle (ohne Callbacks)
public func ClearMacroCommands()
{
  while(GetMacroCommand())
    RemoveMacroCommand();
  aMacroCommandList = CreateArray();
  ClearScheduleCall(this(), "MacroComMoveTo");
  ClearScheduleCall(this(), "MacroComWait");
  ClearScheduleCall(this(), "MacroComFollow");
  ClearScheduleCall(this(), "MacroComSuccessFailed");
  ClearScheduleCall(this(), "JumppadCheck");
}

local tellMe_tactics;

public func FxTellMeWhatYouDoTimer(object target, int number)
{
	var i=0;
	
	var coms = "Commands:|";
	while(GetCommand(0,0,i) && i < 6)
	{
		var szName = GetCommand(0,0,i);
		var pTarget = GetCommand(0,1,i);
		var iX = GetCommand(0,2,i);
		var iY = GetCommand(0,3,i);
		var szTarget = "";
		if(pTarget) szTarget = GetName(pTarget);
		
		var color = HSL(i*75,255,128);
		coms = Format("%s<c %x>%s -> %s (%d,%d)</c>|",coms, color, szName, szTarget, iX, iY);
		CreateParticle("JetSpark",AbsX(iX),AbsY(iY),0,0,60,color);
		++i;
	}
	if(GetCommand(0,0,++i))
		coms = Format("%s...|",coms);
	
	var macroComs="Macro Commands:|";
	for(i=0; i < GetLength(aMacroCommandList); ++i)
	{
		var iEffect = aMacroCommandList[i];
		var szCommand = EffectVar(0, this, iEffect);
		var pTarget = EffectVar(1, this, iEffect);
		var iX = EffectVar(2, this, iEffect);
		var iY = EffectVar(3, this, iEffect);
		var iDelay = EffectVar(4, this, iEffect);
		var pCallback = EffectVar(5, this, iEffect);
		var iAggro = EffectVar(6, this, iEffect);
		
		var x = iX;
		var y = iY;
		if(!x && pTarget) x=GetX(pTarget);
		if(!y && pTarget) y=GetY(pTarget);
		var szTarget = "";
		if(pTarget) szTarget = GetName(pTarget);
		
		macroComs = Format("%s%s -> %s (%d,%d)|",macroComs, szCommand, szTarget,x, y);
	}
	
	var tactics = "";
	if(tellMe_tactics)
		tactics = Format("Tactics:|%s",tellMe_tactics);
	
	Message(Format("%s%s%s",coms,macroComs,tactics),this);
}

public func DebugMessage(string msg,  string name) {
	if(name eq "tactics" && GetEffect("TellMeWhatYouDo",this))
		tellMe_tactics = msg;
	else
		_inherited(msg,name,Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9));
}

public func TellMeWhatYouDo(bool on)
{
	if(on)
		AddEffect("TellMeWhatYouDo",this,1,1,this);
	else
		RemoveEffect("TellMeWhatYouDo",this);
}

/* Makros */

// MoveTo
protected func MacroComMoveTo()
{
  // Erstes Kommando ist gar nicht MoveTo?
  if(GetMacroCommand(0,0) ne "MoveTo") return();
  // Kein Ziel? (bewirkt, dass der Clonk nicht nach 0,0 laufen kann!)
  if(!GetMacroCommand(0,2) && !GetMacroCommand(0,3))
    if(!GetMacroCommand(0,4))
      return(MacroComSuccessFailed(0, Macro_MoveToNoTarget));
  // Pfad suchen
  aPath = CreateArray();
  var start, end;
  start = FindWaypoint(GetX(), GetY());
  end = FindWaypoint(GetMacroCommand(0,2), GetMacroCommand(0,3));
  if(!start || !end) return(MacroComSuccessFailed(0, Macro_NoPath));
  aPath = FindPath(start, end, true);
  // Kein Pfad konnte gefunden werden?
  if(!aPath) return(MacroComSuccessFailed(0, Macro_NoPath));

  // ist der Endpunkt ein Transportpunkt? (ein Wegpunkt mit nur einem Ausgang
  // und dieser Ausgang zeigt nicht zum Eingang)
  if(end->GetPathCount() == 1) {
    var before = 0;
    var leng = GetLength(aPath);
    if(start != end)
      before = aPath[leng-2];
    if(end->GetPathTarget(0) != before) {
      // Ziel ändern (Regel: Pfad darf nicht auf Transportpunkt enden)
      aPath[leng] = end->GetPathTarget(0);
      // Ziel ändern auf folgenden Wegpunkt, Position entsprechend.
      EffectVar(1, this(), aMacroCommandList[0]) = end->GetPathTarget(0);
      EffectVar(2, this(), aMacroCommandList[0]) = GetX(end->GetPathTarget(0));
      EffectVar(3, this(), aMacroCommandList[0]) = GetY(end->GetPathTarget(0));
    }
  }
  // In Bewegung setzen (der erste Wegpunkt sollte durch MoveTo erreichbar sein, wenn nicht -> doof
  // Spezialhack: reitet!
  if(IsRiding()) SetAction("Walk");
  SetCommand(this(), "MoveTo", aPath[0]);
  AppendCommand(this(), "Call", this(), 1,0,0,0, "MacroComMoveToStep");
  // Automatischer Abbruch
  ScheduleCall(this(), "MacroComSuccessFailed", 500, 0, 0, Macro_PathImpossible);
  // Falls Startwegpunkt = Jumppadpunkt
  if(GetLength(aPath) > 1 && aPath[0]->GetPathCount() == 1)
    ScheduleCall(this(), "JumppadCheck", 5, 0, aPath[1], 2);
  // Aggrolevel
  if(GetMacroCommand(0, 6) != -1)
    SetAggroLevel(GetMacroCommand(0, 6),0,0,0,"MacroComMoveTo");
  //Log("%s #%d: MacroComMoveTo", GetName(), ObjectNumber());
}

protected func MacroComMoveToStep(object dummy, int iStep) // MoveTo-Schritt
{
  // Erstes Kommando ist gar nicht MoveTo?
  if(GetMacroCommand(0,0) ne "MoveTo") return();
  // Wird haben das Ende des Pfades erreicht?
  if(GetLength(aPath) == iStep)
  {//Log("%s #%d: MacroComMoveTo finished", GetName(), ObjectNumber());
    // Zum Ziel laufen und beenden
    var x = GetMacroCommand(0,2), y = GetMacroCommand(0,3);
    SetCommand(this(), "MoveTo", 0, x,y);
    AppendCommand(this(), "Call", this(), 0,0,0,0, "MacroComSuccess");
    return(1);
  }
  // Aus irgendeinem Grund ist kein Wegpunkt da?
  if(!aPath[iStep]) return(MacroComSuccessFailed(0, Macro_PathBroken));
  if(!aPath[iStep-1]) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Nächsten Wegpunkt begehen
  MoveAlongPath(aPath[iStep-1], aPath[iStep], iStep+1);
  AppendCommand(this(), "Call", this(), iStep+1,0,0,0, "MacroComMoveToStep");
  // Automatischer Abbruch
  ClearScheduleCall(this(), "MacroComSuccessFailed");
  // 200 Frames pro 100 Pixel Abstand sind angesetzt
  var breaktime = Max(ObjectDistance(aPath[iStep-1], aPath[iStep]) / 100 * 200, 100);
  ScheduleCall(this(), "MacroComSuccessFailed", breaktime, 0, 0, Macro_PathImpossible);
  //Log("%s #%d: MacroComMoveToStep", GetName(), ObjectNumber());
  return(1);
}

protected func MacroComMoveToStepFailed() // MoveTo-Schritt fehlgeschlagen
{//Log("%s #%d: MacroComMoveToStepFailed", GetName(), ObjectNumber());
  // Erstes Kommando ist gar nicht MoveTo?
  if(GetMacroCommand(0,0) ne "MoveTo") return();
  // Fehlschlag auf ganzer Linie
  MacroComSuccessFailed(0, Macro_PathImpossible);
  return(1);
}

// Follow
protected func MacroComFollow(bool fStarted)
{
  // Sonderbehandlung: wurde schon gestartet
  if(fStarted)
  {
    DebugLog(Format("%s #%d: MacroComFollow timer", GetName(), ObjectNumber()), "MacroComFollow");
    // Prüfen, ob an der Stelle iPos in der Kette "Follow" steht
    if(GetMacroCommand(1) ne "Follow" && GetMacroCommand(0) ne "Follow")
      // Steht nicht da, ergo muss sich ein anderes Kommando reingeschaltet haben -> nichts tun
      return();
    // Wir haben kein MoveTo mehr? Dann sind wir angekommen (wir starten einfach neu)
    if(GetMacroCommand() ne "MoveTo") return(MacroComFollow());
    // Unser Ziel ist irgendwie weg?
    if(!GetMacroCommand(1,1))
    {
      DebugLog(Format("%s #%d: MacroComFollow target lost", GetName(), ObjectNumber()), "MacroComFollow");
      // MoveTo löschen
      RemoveMacroCommand();
      // Fehlschlag ausgeben
      return(MacroComSuccessFailed(0, Macro_FollowTargetLost));
    }
    // Schauen, ob unser Bewegungsziel noch ok ist
    var x,y,target;
    x = GetMacroCommand(0,2);
    y = GetMacroCommand(0,3);
    target = GetMacroCommand(0,1);
    // Distanz > 30 Pixel?
    if(Distance(AbsX(x), AbsY(y), AbsX(target->GetX()), AbsY(target->GetY())) > 30)
    {
      DebugLog(Format("%s #%d: MacroComFollow new MoveTo", GetName(), ObjectNumber()), "MacroComFollow");
      // MoveTo löschen
      if(GetMacroCommand() eq "MoveTo") RemoveMacroCommand();
      // Neues Kommando setzen
      AddMacroCommand(0, "MoveTo", GetMacroCommand(0,1), 0, 0, 0, GetMacroCommand(0,6));
    }
    // Follow Weiterlaufen lassen
    return(ScheduleCall(this(), "MacroComFollow", 30, 0, true));
  }
  DebugLog(Format("%s #%d: MacroComFollow started", GetName(), ObjectNumber()), "MacroComFollow");
  // Erstes Kommando ist gar nicht Follow?
  if(GetMacroCommand(0,0) ne "Follow") return();
  // Kein Ziel? -> Fehlschlag
  if(!GetMacroCommand(0,1))
    return(MacroComSuccessFailed());
  // Wir sind dem Ziel noch nicht nahe genug?
  if(ObjectDistance(GetMacroCommand(0,1)) > 30)
    // Follow bedeutet viele MoveTos -> loslaufen bitte
    AddMacroCommand(0, "MoveTo", GetMacroCommand(0,1), 0, 0, 0, GetMacroCommand(0, 6));
  // Aggrolevel
  //if(GetMacroCommand(0, 6) != -1)
    //SetAggroLevel(GetMacroCommand(0, 6));
  // Alle 30 Frames aktualisieren wir
  return(ScheduleCall(this(), "MacroComFollow", 30, 0, true));
}

// Wait
protected func MacroComWait(bool fEnd)
{
  // Erstes Kommando ist gar nicht Wait?
  if(GetMacroCommand(0,0) ne "Wait") return();
  // Beenden?
  if(fEnd)
    return(MacroComSuccess(0));
  // Aggrolevel
  if(GetMacroCommand(0, 6) != -1)
    SetAggroLevel(GetMacroCommand(0, 6),0,0,0,"MacroComWait");
  // Dann warten wir jetzt eine Runde
  return(ScheduleCall(this(), "MacroComWait", GetMacroCommand(0,4), 0, true));
}

/* Makro-Funktionen */

// Makrobefehl an der Stelle iCommand ist erfolgreich
protected func MacroComSuccess(iCommand, int iCmd2)
{//Log("%s #%d: MacroComSuccess", GetName(), ObjectNumber());
  // Für Aufrufe aus SetCommand(... "Call");
  if(GetType(iCommand) == C4V_C4Object) iCommand = iCmd2;
  // An der Stelle gibt es nichts?
  if(iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return();
  // Hat super geklappt! Callbackobjekt benachrichtigen, sofern vorhanden
  var callback = GetMacroCommand(iCommand, 5);
  if(callback) callback->~OnMacroCommandSuccess(this());
  // Kommando aus der Liste entfernen
  RemoveMacroCommand(iCommand);
  // Evtl. Weg löschen
  aPath = CreateArray();
  // Nächstes Kommando starten
  if(GetMacroCommand()) Call(Format("MacroCom%s", GetMacroCommand()));
  return(1);
}

protected func MacroComTumble()
{//Log("%s #%d: MacroComTumble", GetName(), ObjectNumber());
  MacroComSuccessFailed(0, Macro_PathImpossible);
  CheckStuck();
}

// Fehlercodes:
static const Macro_NoPath = 0; // Keinen Weg gefunden
static const Macro_PathImpossible = 1; // Weg konnte abgelaufen werden
static const Macro_PathBroken = 2; // Pfad wurde beim Ablaufen unterbrochen (Wegpunkt gelöscht o.ä.)
static const Macro_FollowTargetLost = 3; // Target für Follow verloren
static const Macro_MoveToNoTarget = 4; // MoveTo hat kein Ziel bekommen

// Makrobefehl an der Stelle iCommand ist fehlgeschlagen
protected func MacroComSuccessFailed(int iCommand, int iReason)
{//Log("%s #%d: MacroComSuccessFailed: %d", GetName(), ObjectNumber(), iCommand);
  // An der Stelle gibt es nichts?
  if(iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return();
  // Hat gar nicht geklappt! Callbackobjekt benachrichtigen, sofern vorhanden
  var callback = GetMacroCommand(iCommand, 5);
  if(callback) callback->~OnMacroCommandFailed(this(), iReason);
  // Kommando aus der Liste entfernen
  RemoveMacroCommand(iCommand);
  if(!iCommand) SetCommand(this(), "None");
  // Evtl. Weg löschen
  aPath = CreateArray();
  // Nächstes Kommando starten
  if(GetMacroCommand()) Call(Format("MacroCom%s", GetMacroCommand()));
  return(1);
}

// Makrobefehle aus der Liste entfernen
private func RemoveMacroCommand(int iCommand)
{//Log("%s #%d: RemoveMacroCommand: %d", GetName(), ObjectNumber(), iCommand);
  // An der Stelle gibt es nichts?
  if(iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return();
  // Entfernen
  for(var i = 0, iEff, checked ; i < GetEffectCount("MacroCommand", this()) ; i++)
  {
    iEff = GetEffect("MacroCommand", this(), i);
    if(iEff == aMacroCommandList[iCommand])
    {
      RemoveEffect("MacroCommand", this(), i);
      continue;
    }
    if(GetEffect("MacroCommandLocked", this())) continue;
    for(var mac in aMacroCommandList)
      if(mac == iEff)
        checked = true;
    if(!checked) RemoveEffect("MacroCommand", this(), i, true);
    checked = false;
  }
/*  if(RemoveEffect(0, this(), aMacroCommandList[iCommand]))
    Log("Effect removed");*/
  var aNewMacroComList = CreateArray();
  i = 0;
  for(var iCom in aMacroCommandList)
  {
    if(i == iCommand) continue;
    aNewMacroComList[GetLength(aNewMacroComList)] = iCom;
    i++;
  }
  // Erfolgreich entfernt
  aMacroCommandList = aNewMacroComList;
  return(1);
}

public func FxMacroCommandStop(object pTarget)
{
  // Wenn wir gesichert sind, werden wir nicht entfernt
  if(GetEffect("MacroCommandLocked", pTarget)) return(-1);
}

/* Wegfindung */

public func FindPath(object pStart, object pEnd, bool fJetpack)
{
  return FindPathNaiveNoLoops(pStart, pEnd, fJetpack);
}

private func FindPathNaiveNoLoops(object pStart, object pEnd, bool fJetpack)
{
  var aPath = FindPathNaive(pStart, pEnd, fJetpack);

  if(aPath)
  {
    for(var i = 0; i<=GetLength(aPath); i++)
      for(var j = GetLength(aPath); j>i; j--) //rückwärts zählen
      {
        if(aPath[i] == aPath[j])
        {
          aPath = ArrayRemove(aPath,i,j);
          break;
        }
      }
  }
  return aPath;
}

// Entfernt aus dem Array [iStart, iEnd) ... also ab iStart aber exklusive iEnd
private func ArrayRemove(A, int iStart, int iEnd) {
  if (iStart == iEnd) return(A);
	
  var j = 0;
  var newA = [];
  for (var i = 0; i < iStart; i++)
    newA[j++] = A[i];
  for (var i = iEnd; i < GetLength(A); i++)
    newA[j++] = A[i];
	
  return(newA);
}

// gibt Pfad von Wegpunkt pStart bis pEnd als Array aus. An Stelle 0 ist pStart.
// wenn FindPath = 0, hat er keinen Pfad gefunden.
private func FindPathNaive(object pStart, object pEnd, bool fJetpack)
{
  // naive Wegfindung. Wir gehen davon aus, dass wenn der WP links von einem ist,
  // nach links gegangen werden muss und gehen einfach Schritt fuer Schritt in diese
  // Richtung.

  // maximale Iterationen fuer Wegfindung (abhaengig von Anzahl Wegpunkte)
  var iterationLimit = ObjectCount(WAYP);
  
  var ex = GetX(pEnd);
  var ey = GetY(pEnd);

  var pCurrent = pStart;

  var aWaypoints = CreateArray();
  
  var jetp = 0;
  var ammoload = DefinitionCall(JTPK,"GetFMData",FM_AmmoLoad);

  for(var j=0; j< iterationLimit; ++j) {

    aWaypoints[j] = pCurrent;

    // fertig!
    if(pCurrent == pEnd)
      return(aWaypoints);

    // ansonsten...
    var cx = GetX(pCurrent);
    var cy = GetY(pCurrent);
    var cangle = Angle(cx,cy,ex,ey);

    var pNext = 0;
    var pathcount = (pCurrent->WAYP::GetPathCount());
    var aBest = CreateArray();
    var good = 0;
    var neutral = 0;
    var bad = 0;

    // alle Nachbarknoten...
    for(var i=0; i<pathcount; ++i) {
      pNext = (pCurrent->WAYP::GetPathTarget(i));

      if(!Check4Jetpack(pCurrent,i,fJetpack,jetp,ammoload))
        continue;

      // Fuer die richtige Richtung zaehlt nicht der Winkel des naechsten Knotens, sondern
      // wenn pNext nur einen Nachfolger hat, zaehlt die Richtung in die der Nachfolger geht
      // (was rekursiv fortsetzbar ist)
      var jetpgedacht = 0;
      while((pNext->WAYP::GetPathCount()) == 1) {
        // Endknoten? Wir brauchen nicht weiteriterieren
        if(pNext == pEnd) break;
        // für den gedachten Pfad muss noch gecheckt werden, ob da nicht Jetpack benötigt wird
        var check = Check4Jetpack(pNext,0,fJetpack,jetp+jetpgedacht,ammoload);
        if(check)
          pNext = (pNext->WAYP::GetPathTarget(0));
        if(check == 2)
          jetpgedacht += ammoload;
      }

      // Endknoten? Super, fertig!
      if(pNext == pEnd) {
        aBest[0] = i;
        good = 1;
        break;
      }

      // ansonsten
      var nx = GetX(pNext);
      var ny = GetY(pNext);
      var nangle = Angle(cx,cy,nx,ny);
 
      // diffangle: Nimmt Werte von 0 (genau richtig) bis 180 (genau falsch) an
      var diffangle = Abs(Normalize(nangle-cangle,-180));
      // Schummeln: der WP von dem man grad gekommen ist, wird als "bad" eingestuft
      if(j>0)
        if(pNext == aWaypoints[j-1] || pNext == pCurrent)
          diffangle = 180;
      // möglichst ein Array aus Pfaden zusammenstellen, die etwa in die gewünschte Richtung
      // führen... (Differenz <= 55°)
      if(diffangle <= 55) {
        aBest[good] = i;
        ++good;
      }
      // dann vielleicht nicht ganz so zielführende...? (nur wenn kein Pfad in richtige Richtung führt)
      if(diffangle <= 110 && diffangle > 55 && !good) {
        aBest[neutral] = i;
        ++neutral;
      }
      // ansonsten dann ein Array aus nicht-zielführenden...
      if(diffangle > 110 && !neutral && !good) {
        aBest[bad] = i;
        ++bad;
      }
    }
    // Debug
    if(Debug("waypoints")) {
      var dbg = 0;
      var str = " ";
      var wayp = 0;
      if(good) { str = "good"; dbg = good; }
      else if(neutral) { str = "neutral"; dbg = neutral; }
      else { str = "bad"; dbg = bad; }
      if(dbg>1) {
        DebugLog("  Choosing path from %s...","waypoints",str);
        for(var d = 0; d<dbg; ++d) {
          wayp = (pCurrent->WAYP::GetPathTarget(aBest[d]));
          DebugLog("  waypoint (%d), angle %d","waypoints",ObjectNumber(wayp),Abs(Normalize(Angle(GetX(pCurrent),GetY(pCurrent),GetX(wayp),GetY(wayp))-cangle,-180)));
        }
      }
    }
    
    // zufällig aus möglichen Pfaden auswählen
    var chosenone = -1;
    if(good)          { chosenone = aBest[Random(good)]; }
    else if(neutral)  { chosenone = aBest[Random(neutral)]; }
    else if(bad)      { chosenone = aBest[Random(bad)]; }
    else {
      DebugLog("Failure: Selected waypoint leads into a dead end.","waypoints");
      return(false);
    }
    // ansonsten ok
    pNext = (pCurrent->WAYP::GetPathTarget(chosenone));

    // geschätzter Benzinverbrauch bei Jetpack hochzählen
    if(pCurrent->WAYP::GetPathJetpack(chosenone))
      jetp += ammoload;

    DebugLog("Next %s waypoint (%d), angle %d","waypoints",str,ObjectNumber(pNext),Abs(Normalize(Angle(GetX(pCurrent),GetY(pCurrent),GetX(pNext),GetY(pNext))-cangle,-180)));

    // Fertig
    if(pNext == pEnd) {
      DebugLog("Success: Complete path consists of %d waypoints.","waypoints",GetLength(aWaypoints));
    }    

    // kommt bei naechster Iteration ins Array
    pCurrent = pNext;
  }
  DebugLog("Failure: No path found after %d iterations.","waypoints",iterationLimit);
  return(false);
}

// nächsten Wegpunkt von einer Position aus suchen (geht nur, wenn PathFree!)
private func FindWaypoint(int iX, int iY)
{
  var wp;
  while(wp = FindObject(WAYP, AbsX(iX), AbsY(iY), -1, -1, 0,0,0,0, wp)) {
    if(PathFree(iX, iY, wp->GetX(), wp->GetY()) || GBackSolid(wp->GetX(), wp->GetY()))
      return(wp);
  }
  // Nix gefunden
  return();
}

// Vorbedingung für Jetpack prüfen
private func Check4Jetpack(object pCurrent, int path, bool fJetpack, int jetp, int ammoload)
{
  // kein Jetpackpath... dann ist ja gut
  if(!(pCurrent->WAYP::GetPathJetpack(path)))
    return(1);
  // ansonsten Vorbedingungen checken
  if(fJetpack)
    if(HasJetpack())
      //if(ammoload <= GetAmmo(GSAM)-jetp)
        return(2);
  return(0);
}

// Lässt den Clonk von einem Wegpunkt zum anderen laufen, wobei er die jeweilige Pfadeigenschaft beachten muss
private func MoveAlongPath(object pCurrentWp, object pNextWp, int iNextStep)
{
  // Rausfinden, welchen Weg wir gehen müssen
  for(var i = 0, path = -1 ; i < pCurrentWp->GetPathCount() ; i++)
    if(pCurrentWp->GetPathTarget(i) == pNextWp)
    {
      path = i;
      break;
    }
  // Kein Weg? -> Fehler
  if(path < 0) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Sonderbehandlung für Zielwegpunkte, die nur einen Pfad von sich wegführen haben (da könnten Jumppads o.ä. sein)
  ClearScheduleCall(this(), "JumppadCheck"); // Zur Sicherheit
  if(GetLength(aPath) > iNextStep && pNextWp->GetPathCount() == 1)
    ScheduleCall(this(), "JumppadCheck", 5, 0, pNextWp, iNextStep);
  // Jetpackfliegen
  if(pCurrentWp->GetPathJetpack(path))
    ScheduleCall(this(), "StartJetpack", pCurrentWp->GetPathJetpack(path), 0, pCurrentWp->GetPathJetpackFlag(path));
  // Flag rausfinden und entsprechend agieren
  var flag = pCurrentWp->GetPathFlag(path);
  if(flag == Path_MoveTo)
  {
    // Hinlaufen
    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Jump)
  {
    // Springen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if(dir < 0) dir = 0;
    SetDir(dir);
    Jump();
    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Backflip)
  {
    // Backflippen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if(dir < 0) dir = 0;
    SetDir(!dir);
    var iEff = AddEffect("ControlStack", this, 110, 5, this);
		if(GetDir() == DIR_Left)
		  EffectVar(0, this, iEff) = COMD_Right;
		else
		  EffectVar(0, this, iEff) = COMD_Left;
    Jump();
    ScheduleCall(0, "JumpStart", 1, 1, true);

    SetCommand(this(), "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Lift)
  {
    // Wir suchen den Lift und warten oder betreten ihn
    LiftControl(0, ObjectNumber(pCurrentWp), ObjectNumber(pNextWp));
    return(AddSpecialCommands(pCurrentWp, path));
  }
  // Unbekanntes flag, MoveTo versuchen
  SetCommand(this(), "MoveTo", pNextWp);
  return(AddSpecialCommands(pCurrentWp, path));
}

/* Sonderfunktionen */

protected func JumppadCheck(object pTargetWp, int iNextStep)
{
  // Pfad hat sich irgendwie geändert
  if(GetLength(aPath) <= iNextStep) return();
  var pNextWp = aPath[iNextStep];
  // Feststellen, ob pTargetWp immernoch unser Bewegungsziel ist
  if(aPath[iNextStep-1] != pTargetWp)
    return();
  // Schauen, ob wir näher an pNextWp dran sind
  if(ObjectDistance(pTargetWp) > ObjectDistance(pNextWp))
    // Wegpunkt wird übersprungen
    return(MacroComMoveToStep(0, iNextStep));
  // Weiterlaufen
  ScheduleCall(this(), "JumppadCheck", 5, 0, pTargetWp, iNextStep);
}

protected func StartJetpack(int iDir)
{
  // Kein Jetpack mehr? oO
  var jetpack = HasJetpack();
  if(!jetpack) return();
  // Richtung zünden
  if(iDir == Jetpack_Left) // Links
  {
    SetDir(DIR_Left);
    jetpack->ControlLeftDouble(this());
  }
  if(iDir == Jetpack_UpLeft) // Oben links
  {
    SetDir(DIR_Left);
    jetpack->ControlUpDouble(this());
  }
  if(iDir == Jetpack_UpRight) // Oben rechts
  {
    SetDir(DIR_Right);
    jetpack->ControlUpDouble(this());
  }
  if(iDir == Jetpack_Right) // Rechts
  {
    SetDir(DIR_Right);
    jetpack->ControlRightDouble(this());
  }
}

protected func LiftControl(object dummy, int pCurrentWp, int pNextWp)
{
  // Fässt den Lift schon an?
  if(GetAction() eq "Push")
  {
    var lift = GetActionTarget();
    if(!lift) return();
    // Nah genug am Ziel? Absteigen
    if(ObjectDistance(Object(pNextWp)) <= 50)
      return(AddCommand(this(), "MoveTo", Object(pNextWp)));
    // Liftplatten befehligen
    lift->~ControlCommand("MoveTo",0, Object(pNextWp)->GetX());
    // Warten
    AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, 0,0, "LiftControl");
    AddCommand(this(), "Wait", 0,0,0,0,0, 15);
    return(1);
  }
  // Liftplatte suchen
  var x1 = Object(pCurrentWp)->GetX();
  var x2 = Object(pNextWp)->GetX();
  if(x1 > x2) { x1 = x2; x2 = Object(pCurrentWp)->GetX(); }
  var lift = FindObject2(Find_Func("IsLift"), Find_InRect(AbsX(x1), -25, x2-x1, 50));
  // Kein Lift? -> Fehlschlag
  if(!lift) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Lift nah genug? -> Einsteigen
  if(ObjectDistance(lift) <= 50)
  {
    AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, 0,0, "LiftControl");
    AddCommand(this(), "Grab", lift);
    return(1);
  }
  // Warten
  AddCommand(this(), "Call", this(), pCurrentWp, pNextWp, 0,0, "LiftControl");
  
  AddCommand(this(), "Wait", 0,0,0,0,0, 15);
  return(1);
}

// Alles was der Wegpunkt als ArriveCommand hat
protected func AddSpecialCommands(object pCurrentWp, int path)
{
  if(!(pCurrentWp->GetArriveCommandCount(path))) return;
  for(var i= pCurrentWp->GetArriveCommandCount(path) - 1,command ; i > -1 ; i--)
    AddCommand(this(), pCurrentWp->GetArriveCommand(path, i), 
                       pCurrentWp->GetArriveCommand(path, i, 1),
                       pCurrentWp->GetArriveCommand(path, i, 2),
                       pCurrentWp->GetArriveCommand(path, i, 3),
                       pCurrentWp->GetArriveCommand(path, i, 4), 0,
                       pCurrentWp->GetArriveCommand(path, i, 5));
}

/* KI-Kampf */

// Setzt einen bestimmten Aggro-Wert für den Clonk
// 0 - keinerlei eigenmächtiges Handeln
// 1 - schießen auf Feinde, ohne dabei vom aktuellen Kommando abzuweichen
// 2 - auf Feinde wird geschossen und sie werden über kurze Strecken verfolgt
// 3 - Position (iX, iY) bewachen und dabei nur iDist weit weglaufen (wenn Feind verfolgt werden)
// Konstanten:
static const Aggro_Nothing = 0;
static const Aggro_Shoot   = 1;
static const Aggro_Follow  = 2;
static const Aggro_Guard   = 3;

public func SetAggroLevel(int iLevel, int iDist, int iX, int iY, string text)
{
//  if(GetOwner() < -1) Message("@SetAggroLevel: %d: %s", this(), iLevel, text);
  // > 3
  if(iLevel > 3) return();
  // Wir kommen von > 2
  var target;
  if(GetAggroLevel() >= 2 && iLevel < 2)
    if(target = EffectVar(1, this(), GetEffect("Aggro", this())))
      if(GetMacroCommand(1, 1) == target)
      {
        FinishMacroCommand(1,0,1);
        FinishMacroCommand(1);
      }
  // 0
  if(!iLevel) return(RemoveEffect("Aggro", this()));
  // 1, 2 und 3
  var effect = GetEffect("Aggro", this());
  if(!effect) effect = AddEffect("Aggro", this(), 50, 10, this());
  EffectVar(0, this(), effect) = iLevel;
  // Parameter
  // iDist, Default = 500
  if(!iDist)
    iDist = 500;
  EffectVar(2, this(), effect) = iDist;
  // iX und iY, Default = GetX() & GetY()
  if(!iX && !iY && iLevel == 3)
  {
    iX = GetX();
    iY = GetY();
  }
  EffectVar(3, this(), effect) = iX;
  EffectVar(4, this(), effect) = iY;
  return(1);
}

public func GetAggroLevel()
{
  var effect = GetEffect("Aggro", this());
  if(!effect) return();
  return(EffectVar(0, this(), effect));
}

// Setzt sofort das Angriffsziel
public func SetAggroTarget(object pTarget)
{
  if(GetAggroLevel() == Aggro_Nothing) return();
  EffectVar(1, this(), GetEffect("Aggro", this())) = pTarget;
  return(1);
}

public func GetAggroTarget()
{
  return(EffectVar(1, this(), GetEffect("Aggro", this())));
}

public func FxAggroTimer(object pTarget, int no)
{
	// keine Waffe geladen? Dann waffen durchchecken 
	var hasWeapons = false;
	var hasLoadedWeapon = false;
	for(var i = 0; i < ContentsCount(); i++)
	{
		if(Contents(i)->~IsWeapon())
		{
			hasWeapons = true;
			if(Contents(i)->~GetCharge() > 0)
			{
				hasLoadedWeapon = true;
				break;
			}
		}
	}
		
	if(hasWeapons && !hasLoadedWeapon)
	{
		// Zielen beenden
		if(IsAiming()) StopAiming();
		CheckIdleWeapon();
		return;
	}
	
  // Wir haben ein Ziel?
  if(EffectVar(1, this(), no)) { EffectCall(this(), no, "Fire"); return(1); }
  // Zielen beenden
  if(IsAiming()) StopAiming();
//  Message("@No target", this());
  // Ziel suchen
  var dir = GetDir()*2-1;
	var dist = EffectVar(2, this(), no);
  // Vorne
  var target = GetTarget(90*dir, 90, dist);
  // Hinten
  if(!target)
    if((!GetCommand() && !GetMacroCommand()) || EffectVar(0, this(), no) != 1)
      target = GetTarget(-90*dir, 90, dist);
  // Gefunden?
  if(!target)
  {
    // Nichts gefunden :(
    // -> Bescheid geben!
    if(EffectVar(99, this(), no))
    {
      if(Contained())
        Contained()->~HandleAggroFinished(this());
      else if(IsRiding())
        GetActionTarget()->~HandleAggroFinished(this());
      
      EffectVar(99, this(), no);
    }
    // -> Waffen durchchecken
    CheckIdleWeapon();
    return();
  }
  // Super
  EffectVar(1, this(), no) = target;
  EffectVar(99,this(), no) = true; // wir haben ein Ziel \o/
}

// Der Name dieser Funktion ist irreführend. Es ist die AI Behandlung wenn gerade ein Ziel da (EffectVar 1)
public func FxAggroFire(object pTarget, int no)
{
  // Zusatzhack: BR-Bombe!
  if(GetID(Contents()) == GBRB) return;
  
  // Nichts tun, wenn gerade verhindert
  if(!ReadyToFire() && !ReadyToAim()) return;
  
  var y = EffectVar(4, this, no);
  var x = EffectVar(3, this, no);
  var dist = EffectVar(2, this, no);
  var target = EffectVar(1, this, no);
  var level = EffectVar(0, this, no);
  var pathfree = true;
    
  // Fahrzeugsteuerung
  if(Contained())
  {
    if(!(Contained()->~HandleAggro(this, level, target, dist, x, y)))
      AddCommand(this, "Exit", 0,0,0,0,0,0,0, C4CMD_SilentSub);
    return;
  }
  if(IsRiding())
  {
    if(!(GetActionTarget()->~HandleAggro(this, level, target, dist, x, y)))
      SetAction("Walk");
    return;
  }
  
  // Zu weit von der Wachposition entfernt?
  if(level == Aggro_Guard)
  {
    if(Distance(GetX(), GetY(), x, y) > dist)
    {
      if(GetMacroCommand(1, 1) == target)
      {
        FinishMacroCommand(1,0,1);
        FinishMacroCommand(1);
      }
      AddMacroCommand(0, "MoveTo", 0, x,y, 0, level);
      EffectVar(1, this, no) = 0;
      //      Message("@Returning to guarded position", this());
      return;
    }
  }
  
  var maxdist = dist;
  if(!PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
  {
    if(level == Aggro_Shoot) maxdist = 0;
    if(level >= Aggro_Follow) maxdist = dist/2;
    pathfree = false;
  }
  
  // Ziel irgendwie weg?
  // (Pathfree wurde schon gecheckt)
  if(!CheckTarget(target,this,maxdist,0,0,true))
  {
      EffectVar(1, this, no) = 0;
      if(level == Aggro_Follow)
        ClearMacroCommands();
      if(IsAiming())
        StopAiming();
      return;
  }
  // Ich hab nix? °-°
  if(!Contents()) return; // Lauf, Forest, lauf!
  // Waffe in die Hand nehmen
  if(!SelectWeapon(level, target, false))
  {
    // Evtl. Feuermodus wechseln (dann muss erst nachgeladen werden, aber besser als nichts)
    if(!SelectWeapon(level, target, true))
    {
      // Bei Aggro_Follow können wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
      if(level == Aggro_Follow)
      {
//      Message("@Searching for weapons / ammo", this());
        // Waffen auffrischen?
        if(CustomContentsCount("IsWeapon") <= 1)
          SearchWeapon(Aggro_Shoot);
        // Munition auffrischen
        else
          SearchAmmo(Aggro_Shoot);
      }
      // ein Balrog, ein Feind gegen den ihr nichts ausrichten könnt...lauft!
      return;
    }
  }
  // Stufe 1 - nur in die grobe Richtung ballern, lieber nicht anhalten oder sowas

  // Schaue ich in die richtige Richtung?
  if(level > Aggro_Shoot)
  {
    if(GetX() < target->GetX())
    {
      if(GetDir() != DIR_Right)
        SetDir(DIR_Right);
    }
    else
    {
      if(GetDir() != DIR_Left)
        SetDir(DIR_Left);
    }
  }

  var weaponRange = Contents()->GetBotData(BOT_Range);
     
  var angle = Angle(GetX(), GetY(), target->GetX(), target->GetY());

  // Feind nicht innerhalb von Winkel in dem wir auf autoaim vertrauen
  var canHitTargetWithoutAiming = Inside(angle, 70, 120) || Inside(angle, 250, 290);

  // Zielen, muss auch mal sein
  if(!GetCommand() && !GetMacroCommand() || level != Aggro_Shoot || IsAiming())
  {
    var mustAimByWeapon = Contents()->GetFMData(FM_Aim)>0;
     
    // Bots sind etwas kurzsichtig, die können nicht weiter als 300px (~=ein Bildschirm auf niedriger Auflösung) sehen
    // der fairness halber...
    var targetInAimRange = ObjectDistance(target) < Min(weaponRange,300);

    if((!canHitTargetWithoutAiming || mustAimByWeapon ) && targetInAimRange && pathfree && weaponRange > 30)
    {
      if(!IsAiming()) StartSquatAiming();
      // nicht für ein else halten, think again!
      if(IsAiming())
      {  
        var tx = target->GetX();
        var ty = target->GetY();

        // ja so pi mal daum....
        if(Contents()->GetBotData(BOT_Ballistic))
          ty -= 15;

        DoMouseAiming(tx, ty);
      }
    }
  // ... oder halt auch nicht zielen
    else if(IsAiming())
      StopAiming();
  }
 
   // Gut. Feuern wir bereits?
  if(Contents()->IsRecharging() || Contents()->IsShooting()) return;
 
  // Feuer!
  var targetInShootRange = ObjectDistance(target) < Min(weaponRange,500);
  if(pathfree && (canHitTargetWithoutAiming || IsAiming()) && targetInShootRange)
  {
    Control2Contents("ControlThrow");
  }

//  Message("@My target: %s @%d/%d with level %d", this(), target->GetName(), target->GetX(), target->GetY(), level);
  // Stufe 2 - verfolgen!
  if(level >= Aggro_Follow)
    if(GetMacroCommand(1) ne "Follow" || GetMacroCommand(1, 1) != target)
      if(GetMacroCommand(0) ne "Follow" || GetMacroCommand(0,1) != target)
      {
        DebugLog("FxAggroFire - Adding Follow command","aggro");
        AddMacroCommand(0, "MoveTo", 0, GetX(),GetY(), 0, level);
        AddMacroCommand(0, "Follow", target, 0, 0, 0, level);
      }
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
  // Entfernung zum Ziel
  var dist = ObjectDistance(pTarget);
  // Keine Waffen in Inventar?
  if(!CustomContentsCount("IsWeapon")) return();
  // Bevorzugten Schadenstyp bestimmen
  var preftype = GetPrefDmgType(pTarget), type;
  // Alle durchgehen und passende prüfen
  var fav, favmode;
  for(var i=0,obj,mode ; obj = Contents(i) ; mode++)
  {
    // Nix Waffe
    if(!(obj->~IsWeapon())) { i++; mode = -1; continue; }
    // Feuermodus
    if(mode && !fFireModes) { i++; mode = -1; continue; }
    if(!(obj->GetFMData(FM_Name, mode))) { i++; mode = -1; continue; }
    if(mode == obj->GetFireMode() && mode) continue;
    // Nix gut
    if(obj->GetFMData(FM_Aim, mode)>0)
      if(iLevel == 1 || !(ReadyToAim() || IsAiming()))
        continue;
    // Keine Munition dafür?
    if(!(obj->GetCharge()) && !CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode)))
    {
      continue;
    }
    // EMP nur gegen Maschinen
    if(obj->GetBotData(BOT_EMP, mode))
      if(!(pTarget->~IsMachine()))
        continue;
    // Kein Favorit bisher?
    if(!fav)
    {
      fav = obj;
      type = fav->GetBotData(BOT_DmgType, mode);
      favmode = mode;
    }
    else
    {
      var favInRange = fav->GetBotData(BOT_Range, favmode) >= dist;
      var objInRange = obj->GetBotData(BOT_Range, mode) >= dist;
      
      var objHasPrefDmgType = obj->GetBotData(BOT_DmgType, mode) == preftype;
      var favHasPrefDmgType = preftype == type;
      var objHasBetterDmgType = objHasPrefDmgType && !favHasPrefDmgType;
      var neitherHasPrefDmgType = !objHasPrefDmgType && !favHasPrefDmgType;
      var favHasFarMorePower = fav->GetBotData(BOT_Power, favmode)-1 > obj->GetBotData(BOT_Power, mode);
      var objIsLongLoader = obj->GetBotData(BOT_Power, mode) != BOT_Power_LongLoad;
      var favIsLongLoader = fav->GetBotData(BOT_Power, favmode) == BOT_Power_LongLoad;
      var objIsLongLoaderThatReloads = objIsLongLoader && (obj->IsReloading() || !(obj->GetCharge()));
      var favIsLongLoaderThatReloads = favIsLongLoader && (fav->IsReloading() || !(fav->GetCharge()));
      
      var objIsMorePowerful = fav->GetBotData(BOT_Power, favmode) < obj->GetBotData(BOT_Power, mode) ||
                              favIsLongLoaderThatReloads && !objIsLongLoaderThatReloads;
      
      if(objInRange)
      {
        if(!favInRange || objHasBetterDmgType && !favHasFarMorePower || neitherHasPrefDmgType && objIsMorePowerful)
        {
          fav = obj;
          type = obj->GetBotData(BOT_DmgType, mode);
          favmode = mode;
        }
      }
    }
  }
	
	DebugLog(Format("Select Weapon: Favourite %s, Mode %v",GetName(fav),favmode),"hazard");
	
  // Auswählen
  if(!fav) return();
  // Feuermodus wechseln?
  if(fFireModes)
    if(favmode && favmode != fav->GetFireMode())
      fav->SetFireMode(favmode);
  if(ContentsCount() == 1) return(1);
  return(ShiftContents(0,0,fav->GetID()));
}

public func GetPrefDmgType(object pTarget)
{
  var test = 100;
  var current;
  var min = current = pTarget->~OnDmg(test, DMG_Projectile);
  var type = DMG_Projectile;
  
  if((current = pTarget->~OnDmg(test, DMG_Melee)) < min)
  {
    type = DMG_Melee;
    min = current;
  }
  if((current = pTarget->~OnDmg(test, DMG_Fire)) < min)
  {
    type = DMG_Fire;
    min = current;
  }
  if((current = pTarget->~OnDmg(test, DMG_Explosion)) < min)
  {
    type = DMG_Explosion;
    min = current;
  }
  if((current = pTarget->~OnDmg(test, DMG_Energy)) < min)
  {
    type = DMG_Energy;
    min = current;
  }
  if((current = pTarget->~OnDmg(test, DMG_Bio)) < min)
    type = DMG_Bio;
    
  return(type);
}

// Sucht nach Waffen und läuft dorthin
public func SearchWeapon(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist eine Waffe drin?
    if(pSpawn -> Contents() ->~ IsWeapon())
      // Die haben wir auch noch nicht?
      if(!FindContents(pSpawn->Contents()->GetID()))
      	// Einsammelbar?
    		if(pSpawn->CheckCollect(GetOwner(),this()))
        	// Hinlaufen
        	return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}

// Sucht nach Munition und läuft dorthin
public func SearchAmmo(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist Munition drin?
    if(pSpawn -> Contents() ->~ IsAmmo())
    	// Einsammelbar?
    	if(pSpawn->CheckCollect(GetOwner(),this()))
      	// Hinlaufen (wir sind gutgläubig und denken, dass wir die auch brauchen)
      	return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}

/* Waffenbehandlung wenn nicht im Kampf */

public func CheckIdleWeapon()
{

  if(Contents())
    // Hack - mit BR-Bombe tut er gar nichts
    if(Contents()->GetID() == GBRB) return false;
  // Keine Waffen im Inventar
  if(!CustomContentsCount("IsWeapon")) return false;
  // nachladende Waffe in der Hand
  if(Contents()->~IsWeapon())
    if(Contents()->IsReloading())
      return();

  // Inventar nach Waffe durchsuchen, die man Nachladen könnte
  var obj = 0;
  for(var i = 0; obj = Contents(i); ++i)
  {
    // Keine Waffe
    if(!(obj->~IsWeapon()))
    {
      continue;
    }
    
    for(var mode=1; obj->GetFMData(FM_Name, mode); ++mode)
    {
      var ammoId = obj->GetFMData(FM_AmmoID, mode);
      var ammoLoad = obj->GetFMData(FM_AmmoLoad, mode);
      
      // Waffe ist (mindestens zur hälfte) geladen
      if(obj->GetAmmo(ammoId) >= ammoLoad / 2)
      {
        break;
      }
      
      // EMP-Modi erstmal nicht laden
      if(obj->GetBotData(BOT_EMP, mode)) continue;
            
      // Waffe ist nachladbar
      if(CheckAmmo(ammoId, ammoLoad - obj->GetAmmo(ammoId)))
      {
        ShiftContents(this, 0, obj->GetID());
        // Feuermodus wechseln
        obj->SetFireMode(mode);
        // Und Muni reinhauen
        obj->Reload();
        return true;
      }
    }
  }
}

/* Inventarbehandlung (wird von KI-Spieler aufgerufen) */

// Inventar behandeln
public func CheckInventory()
{
  for(var i=0,obj ; obj = Contents(i) ; i++)
  {
    // Wenn man etwas hardcoden möchte, dann einfach die Funktion AI_Inventory im Objekt erstellen und true zurückgeben
    if(obj->~AI_Inventory(this))
      continue;
    // Waffe
    if(obj->~IsWeapon())
      continue;
    // Ausrüstungsgegenstand
    if(obj->~IsHazardGear())
    {
      if(HasGear(obj->GetGearType()))
      {
        DropObject(obj);
        continue;
      }
      // Ausrüstung wird prinzipiell als was gutes angesehen -> anlegen
      ActivateGear(obj);
      continue;
    }
    // Mine
    if(obj->~IsMine())
    {
      // Platzieren
      PlaceMine(obj);
      continue;
    }
    // Upgrade
    if(obj->~IsUpgrade())
    {
      var wpn = GetUpgradeableWeapon(obj);
      // Eine passende Waffe gefunden -> upgraden (Upgrades sind immer gut.)
      if(wpn) UpgradeWeapon(obj, wpn);
      continue;
    }
    // Munition
    if(obj->~IsAmmoPacket())
    {
      ActivateAmmo(obj);
      continue;
    }
    // Objekt ist wertlos für uns
    DropObject(obj);
  }
}

// Objekt fallen lassen (verzögert, damit die Schleife nicht durcheinander kommt
public func DropObject(object pObj)
{
  Schedule(Format("Exit(Object(%d), 0, 10);", ObjectNumber(pObj)), 1, 0, this());
  // Nicht wieder einsammeln
  var effect = AddEffect("CollectionException", pObj, 1, 36);
  EffectVar(0, pObj, effect) = this();
}

// Ausrüstung anlegen (verzögert, damit die Schleife nicht durcheinander kommt
public func ActivateGear(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Mine platzieren (verzögert, damit die Schleife nicht durcheinander kommt
public func PlaceMine(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Waffe suchen, die man upgraden kann
public func GetUpgradeableWeapon(object pObj)
{
  for(var i=0,obj ; obj = Contents(i) ; i++)
    if(obj->~IsUpgradeable(pObj->GetID()))
      return(obj);
}

// Waffe upgraden (verzögert, damit die Schleife nicht durcheinander kommt
public func UpgradeWeapon(object pObj, object pWeapon)
{
  ScheduleCall(pObj, "UpgradeObject", 1, 0, pWeapon);
}

// Munition aufnehmen (verzögert, damit die Schleife nicht durcheinander kommt
public func ActivateAmmo(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Hardgecodete Objekte

// Support-Drohne
// Zufällige Waffe für eine Drohne aussuchen
private func GetRandomDroneWeapon()
{
  var aWps = CreateArray();
  for(var i=0,obj ; obj = Contents(i) ; i++)
    if(obj->~IsWeapon())
      if(obj->GetFMData(FM_Aim)<=0)
        aWps[GetLength(aWps)] = obj;
  // Zufällig zurückgeben
  if(!GetLength(aWps)) return();
  return(aWps[Random(GetLength(aWps))]);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	var x, ammo;
	while(ammo = Contents(x++,ammobag)) {
		if(!(ammo ->~IsAmmo())) continue;
		extra[GetLength(extra)] = Format("DoAmmo(%i,%d)", GetID(ammo), GetAmmo(GetID(ammo)));
	}
}
