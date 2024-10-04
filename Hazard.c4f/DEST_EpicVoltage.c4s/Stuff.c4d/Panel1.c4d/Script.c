/*-- Fahrzeugbesteller --*/

#strict 2

local location;
local controller;

local objId;
local team;
local spawnX, spawnY;

protected func Set(iType) {
	location = iType;
	
	if(location == 1)
	{
		objId = SHTL;
		team = 1;
		spawnX = 293;
		spawnY = 245;
	}
  if(location == 2)
	{
		objId = TANK;
		team = 1;
		spawnX = 240;
		spawnY = 510;
	}
	if(location == 3)
	{
		objId = SHTL;
		team = 2;
		spawnX = 2770;
		spawnY = 245;
	}
	if(location == 4)
	{
		objId = TANK;
		team = 2;
		spawnX = 2800;
		spawnY = 510;
	}
	if(location == 5)
	{
		objId = HNG2;
		team = -1;
		spawnX = 1520;
		spawnY = 600;
	}
	
	SetGraphics(0, 0, objId, GFX_Overlay, GFXOV_MODE_IngamePicture);
	SetObjDrawTransform(1000,0,0,0,1000,-85000,0,GFX_Overlay);
}

private func CheckTeam()
{
	if(team == -1) return true;
	return GetPlayerTeam(controller) == team;
}

public func Grabbed(object pByObject, bool fGrab)
{
  if(fGrab)
    ControlUp(pByObject);
  return(1);
}

public func ControlUp(dis) {
	controller = GetController(dis);
	
	if(location == 5)
		CreateDoorMenu(dis);
	else
		CreateBuyMenu(dis);
}

public func CreateBuyMenu(dis) {
	if(!dis) return false;
	if(!CheckTeam())
	{
		Sound("Error");
		return false;
	}
	CreateMenu(CPP1, dis, this, C4MN_Extra_Value);
	
	AddMenuItem(Format("$Buy$",Format("%s", GetName(0, objId))),"BuyStuff",objId,dis,0,0);
	AddMenuItem(GetName(0, _SXT),"Abort",_SXT,dis,0);
}

public func CreateDoorMenu(dis) {
	if(!dis) return false;
	
	CreateMenu(CPP1, dis, this, C4MN_Extra_Value);
	
	AddMenuItem("$BlcLeft$","BuyDoor",objId,dis,0,-80);
	AddMenuItem("$BlcRight$","BuyDoor",objId,dis,0,+80);
	AddMenuItem(GetName(0, _SXT),"Abort",_SXT,dis,0);
}

private func BuyDoor(a, xoffs)
{
	var obj = BuyStuff(a, xoffs);
	if(!obj) return;
	
  var konsole = FindObject(SWTH,AbsX(spawnX+xoffs*3/4),AbsY(spawnY-10));
  konsole->Set(obj);
  obj->Lock(true);
	var team = 2;
	if(xoffs > 0) team = 1;
	obj->SetTeam(team);
	var gas = FindObject(GAS2);
	if(gas)
	{
		gas->AddTarget(obj, true, 500, false, team);
	}
}

private func BuyStuff(a, xoffs)
{
	if(GetWealth(controller) >= GetValue(0,objId))
	{
		if(!FindObject2(Find_ID(objId),Find_AtPoint(AbsX(spawnX)+xoffs, AbsY(spawnY)-10)))
		{
			var pObj = CreateObject(objId, AbsX(spawnX)+xoffs, AbsY(spawnY),-1);
			VehicleSpawnEffect(pObj);
			DoWealth(controller,-GetValue(0,objId));
			Sound("RadioStatic");
			Sound("Cash");
			return pObj;
		}
	}
	Sound("Error");
}

public func Abort() {
  Sound("RadioStatic");
}
