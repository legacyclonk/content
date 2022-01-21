/*-- HUD --*/

#strict 2

local ammoobjs,				// Array aus 2HUD-Objekten zur Anzeige der Ammo im Munitionsbeutel. Uneindeutiger Name wegen Rückwärtskompatibilität (QuakeR)
	    info,			          // 2HUD Hilfsobjekt um den Ladestand der Waffe anzuzeigen. Uneindeutiger Name wegen Rückwärtskompatibilität
			itemInfoHUD;        // 2HUD Hilfsobjekt um den Beschreibungstext für Nicht-Waffen anzuzeigen
			
local allAmmo,				    // alle Definitionen die Ammo sind
			ammovalues,         // Füllstand der Munitionen (seit dem letzten Update). (Array in der gleichen Reihenfolge wie die allAmmo)
      charge, recharge,		// wie weit es geladen ist
			currentWeapon,	    // momentane Waffe
			currentAmmoId,      // momentane AmmoId
			currentAmmoCount,   // momentane Ammo count die in der Waffe ist (seit dem letzten update)
			currentClonk;       // momentan ausgewähltes Crewmember

static const HUD_Layer_AmmoBase = 1;
static const HUD_Layer_Recharge = 2;
static const HUD_Layer_AmmoHighlight = 3;
	  
global func GetHUD(object obj, id theID)
{
  if(!obj)
    if(!(obj = this))
      return;
		
  var owner = GetOwner(obj);
  if(owner <= NO_OWNER) return;
  
	// Scriptspieler bekommen kein HUD
	if(GetPlayerType(owner) == C4PT_Script)
		return;
  
  if(!theID) theID = 1HUD;
  
  var bar = FindObjectOwner(theID,owner);
  if(!bar)
  {
    bar = CreateObject(theID, 0,0, owner);
  }
  return bar;
}
	  
protected func Initialize()
{
	allAmmo = CreateArray();
	ammovalues = CreateArray();
  
  // Main Hud (Ammo progress bar)
	var yoffset = -55;
  SetPosition(105, yoffset);
  SetGraphics("Row", 0, GetID(), HUD_Layer_AmmoBase, GFXOV_MODE_Base);
  SetGraphics("RowHighlight", 0, GetID(), HUD_Layer_AmmoHighlight, GFXOV_MODE_Base/*, 0, GFX_BLIT_Additive*/);
  SetVisibility(VIS_None);
  // Ammo Hud
  info = CreateObject(2HUD, 0,0, GetOwner());
  info->SetPosition(50, yoffset);
	info->SetGraphics(0, 0, NONE,1,GFXOV_MODE_IngamePicture);
	info->SetObjDrawTransform(750,0,0,0,750,0,0,1);
  info->SetVisibility(VIS_None);
	// Item Hud
	itemInfoHUD = CreateObject(2HUD,0,0,GetOwner());
  itemInfoHUD->SetPosition(140, yoffset + 30);
	
  // Ammobag
  InitAmmoBagHUD();
}

protected func Destruction()
{
	if(info) info->RemoveObject();
	if(itemInfoHUD) itemInfoHUD->RemoveObject();
	for(var i=0; i < GetLength(ammoobjs); ++i)
	{
		if(ammoobjs[i]) ammoobjs[i]->RemoveObject();
	}
}

private func InitAmmoBagHUD()
{
	// remove ammobag display
	var def = 0;
	allAmmo = CreateArray();
	ammovalues = CreateArray();
	var arrI = 0;
	// get all ammo
	for(var i=0; def = GetDefinition(i,C4D_StaticBack); ++i)
	{
		if(DefinitionCall(def,"IsAmmo"))
		{
			allAmmo[arrI] = def;
			ammovalues[arrI] = 0;
			arrI++;
		}
	}
	ammoobjs = CreateArray();
	
	// place all ammo objects
	for(var i=0; i < GetLength(allAmmo); ++i)
	{
		// ...now
		ammoobjs[i] = CreateObject(2HUD,0,0,GetOwner());
		ammoobjs[i]->SetPosition(50,160+i*28);
		ammoobjs[i]->SetGraphics(0, 0, allAmmo[i],1,GFXOV_MODE_IngamePicture);
		ammoobjs[i]->SetObjDrawTransform(750,0,0,0,750,0,0,1);
		ammoobjs[i]->SetVisibility(VIS_None);
	}
}

/* Timer */

protected func Timer()
{
  // Spieler weg?
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner()))
	{
		RemoveObject();
		return;
	}
}

/* Aufrufe */

public func Update(object weapon, object ammobag, object who)
{
	// Nur den Cursor updaten
	if(who)
	{
		if(GetCursor(GetOwner(who)) == who)
		{
			UpdateWeapon(weapon);
			
			if(ammobag)
				UpdateAmmoBag(ammobag, weapon);
		}
	}
}

// Waffen-HUD updaten
private func UpdateWeapon(object weapon)
{
	if(!weapon || !weapon->~IsWeapon())
	{
		SetVisibility(VIS_None);
		info->SetVisibility(VIS_None);
		Message("",this);
		Message("",info);
		charge = 0;
		recharge = 0;
		SetCurrentWeapon(0);
		
		// garkein Inventar
		if(!weapon)
		{
			itemInfoHUD->SetVisibility(VIS_None);
			Message("",itemInfoHUD);
		}
		// Inventar, aber keine Waffe
		else
		{
			itemInfoHUD->SetVisibility(VIS_Owner);
			CustomMessage(Format("@<c ffff00>%s</c>|%s",GetName(weapon),GetDesc(weapon)), itemInfoHUD, GetOwner());
		}
	}
	// Waffe...
	else
	{
		if(weapon->IsRecharging())
			recharge = weapon->GetRecharge();
		else
			recharge = 0;
		
		charge = weapon->GetCharge();
		SetVisibility(VIS_Owner);
		info->SetVisibility(VIS_Owner);
		itemInfoHUD->SetVisibility(VIS_None);
		Message("",itemInfoHUD);
		
		var ammoid = weapon->GetFMData(FM_AmmoID);
		var ammoload = weapon->GetFMData(FM_AmmoLoad);
		var modusname = weapon->GetFMData(FM_Name);
		var ammocount = weapon->GetAmmo(ammoid);
		
		var ammodiff = 0;
		if(currentWeapon == weapon && currentAmmoId == ammoid)
		{
			ammodiff = ammocount - currentAmmoCount;
		}

		SetCurrentWeapon(weapon);

		currentAmmoCount = ammocount;		
		if(ammodiff)
			AddEffect("AmmoUpdateNotification", info, 300, 1, this, 0, ammodiff, 750);
		
		var color = "ffff00";
		if(!ammocount) color = "ff0000";
		CustomMessage(Format("@%s", modusname), this, GetOwner(), 0, 10);
		CustomMessage(Format("@<c %s>%d/%d</c>", color, ammocount, ammoload), info, GetOwner(), 118 + GetNumberCount(ammoload)*6, 31);
		
		if(!SetGraphics(Format("Row%i",ammoid), 0, GetID(), HUD_Layer_AmmoBase, GFXOV_MODE_Base))
		{
			SetGraphics("Row", 0, GetID(), HUD_Layer_AmmoBase, GFXOV_MODE_Base);
		}
		info->SetGraphics(0, 0, ammoid,1,GFXOV_MODE_IngamePicture);
	}
	
	UpdateCharge();
	UpdateRecharge();
}

// Setzt neue Waffe als aktive Waffe (oder 0 wenn keine) und startet dazu grafische Effekte
private func SetCurrentWeapon(object weapon)
{
	if(weapon == currentWeapon)
	{
		var ammoid = NONE;
		if(weapon) ammoid = weapon->GetFMData(FM_AmmoID);
		if(ammoid == currentAmmoId) return;
	}
	
	if(currentWeapon)
	{
		RemoveEffect(Format("Animation%i",currentAmmoId),this);
		SetGraphics("RowHighlight", 0, GetID(), HUD_Layer_AmmoHighlight, GFXOV_MODE_Base, 0, GFX_BLIT_Additive);
	}
	
	if(weapon)
	{
		var ammoid = weapon->GetFMData(FM_AmmoID);
		AddEffect(Format("Animation%i",ammoid),this,300,1,this);
		currentAmmoId = ammoid;
	}
	else
	{
		currentAmmoId = NONE;
	}
	
	currentWeapon = weapon;
}

// Gibt Anzahl der Ziffern zurück
private func GetNumberCount(int value)
{
	var result = 0;
	var bigNumba = 1;
	while(value >= bigNumba)
	{
		bigNumba *= 10;
		result++;
	}
	return result;
}

// Reload: Balken strecht sich nach rechts
private func UpdateCharge()
{
	SetObjDrawTransform(charge, 0, -1000 * (1000-charge)/20,0, 1000, 0, this, HUD_Layer_AmmoBase);
}

// Recharge: roter Balken wandert von links nach rechts
private func UpdateRecharge()
{
  if(recharge)
  {
    SetGraphics("Red", this, GetID(), GFXOV_MODE_Action, HUD_Layer_Recharge, "Red");
    SetObjDrawTransform(1000, 0, 50000 -1000 * (100-recharge),0, 1000, 0, this, HUD_Layer_Recharge);
  }
  else
	{
    SetGraphics(0,0,0, HUD_Layer_Recharge);
  }
}

// Munitionsanzeige updaten
private func UpdateAmmoBag(object ammobag, object weapon)
{

	var ammoid = NONE;
	if(weapon)
		if(weapon->~IsWeapon())
			ammoid = weapon->GetFMData(FM_AmmoID);

	var clonk = GetCursor();
		
	for(var i=0; i < GetLength(ammoobjs); ++i)
	{
		var aobj = ammoobjs[i];
	
		// unsichtbar bei NoAmmo
		if(NoAmmo() || !ammobag) {
			aobj->SetVisibility(VIS_None);
			aobj->Message("",aobj);
			continue;
		}
		// ansonsten Munitionsstand anzeigen
		aobj->SetVisibility(VIS_Owner);
			
		var ammo = 0;
		if(ammobag)
			ammo = ammobag->GetAmmo(allAmmo[i]);
		
		var ammodiff = 0;
		if(clonk == currentClonk)
		{
			ammodiff = ammo - ammovalues[i];
		}
		ammovalues[i] = ammo;
		if(ammodiff)
			AddEffect("AmmoUpdateNotification", aobj, 300, 1, this, 0, ammodiff, 750);
		
		var color = "eeeeee";
		if(!ammo) color = "777777";
		if(allAmmo[i] == ammoid) {
			color = "ffff00";
			if(!ammo) color = "ff0000";
		}
		aobj->CustomMessage(Format("@<c %s>%d</c>", color, ammo), aobj, GetOwner(), 25, 31);
	}
	
	currentClonk = clonk;
}

/* Ammocount geändert-Effekt - Das Ammo-Icon poppt auf wenn Munition hinzugefügt wird
   (und andersrum wenn welches entfernt wird) */

func FxAmmoUpdateNotificationStart(object target, int number, int temp, int ammodiff, int size)
{
	if(temp) return;
	
	EffectVar(0,target,number) = 3*size*ammodiff/Abs(ammodiff)/4+size/4; // size ... -size/2
	EffectVar(1,target,number) = size;
}

func FxAmmoUpdateNotificationAdd(object target, int number, string name, int timer, int ammodiff, int size)
{
	EffectVar(0,target,number) = 3*size*ammodiff/Abs(ammodiff)/4+size/4; // size ... -size/2
}

func FxAmmoUpdateNotificationEffect(string name, object target, int number, int newNumber)
{
	if(name == "AmmoUpdateNotification") return -2;
}

func FxAmmoUpdateNotificationTimer(object target, int number, int timer)
{
	var diff = EffectVar(0,target,number);
	var size = EffectVar(1,target,number);
	
	SetObjDrawTransform(size + diff, 0, 0, 0, size + diff, 0, target, 1);
	
	if(!diff) return -1;
	
	var sqrtDiff = diff/Abs(diff) * Sqrt(Abs(diff));
	EffectVar(0,target,number) -= sqrtDiff;
}

/* Hübsche Overlay-Effekte für bestimme Ammo-Typen (erweiterbar) */

func FxAnimationENAMTimer(object target, int number, int timer)
{
	var lightning = EffectVar(0,target,number);
	SetGraphics(Format("Row%iHighlight%d",ENAM,Random(4)+1), 0, GetID(), HUD_Layer_AmmoHighlight, GFXOV_MODE_Base, 0, GFX_BLIT_Additive);
	if(!Random(35)) lightning-=60;
	else lightning+=5;
	lightning = BoundBy(lightning,120,240);
	SetClrModulation(RGBa(255,255,255,lightning),this,HUD_Layer_AmmoHighlight);
	SetObjDrawTransform(charge, 0, -1000 * (1000-charge)/20,0, 1000, 0, this, HUD_Layer_AmmoHighlight);
	EffectVar(0,target,number) = lightning;
}

func FxAnimationGSAMTimer(object target, int number, int timer)
{
	SetGraphics(Format("Row%iHighlight",GSAM), 0, GetID(), HUD_Layer_AmmoHighlight, GFXOV_MODE_Base, 0, GFX_BLIT_Additive);
	SetClrModulation(RGBa(255,255,255,20),this,HUD_Layer_AmmoHighlight);
	var xSkew = Cos(FrameCounter()*12,200);
	var plusHeight = Cos(FrameCounter()*9,100)+Sin(FrameCounter()*22,35);
	var yOffset = 500 -GetDefHeight()*plusHeight/4;
	SetObjDrawTransform(charge, xSkew, -1000 * (1000-charge)/20, 0, 800 + plusHeight, yOffset, this, HUD_Layer_AmmoHighlight);
}