/*--- Flagge! ---*/

#strict

local base, team, cteam;

local carrier;

/* TimerCall */

private func Wee()
{
  if(WildcardMatch(GetAction(), "*Fly*")) return(Wind2Fly());
  if(WildcardMatch(GetAction(), "*Attach*")) return(Clonk2Rotate());
}

private func Wind2Fly()
{
  if(GetWind() > 0)
    SetDir(DIR_Right);
  else
    SetDir(DIR_Left);
}

private func Clonk2Rotate()
{
  if(!GetAlive(carrier)) return(AttachTargetLost());

	var at = GetActionTarget();
	
  SetDir(at->GetDir());

  if(at->GetDir() == DIR_Left())
  {
    SetR(at->GetR()+15-at->GetXDir());
  }
  if(at->GetDir() == DIR_Right())
  {
    SetR(at->GetR()-15-at->GetXDir());
  }
}

protected func Activate(pBase, iTeam, rgb)
{
  base = pBase;
  SetAction("Fly", base);
  Wind2Fly();

	carrier = 0;
  team = iTeam;
  SetColorDw(rgb);

  // bei Dunkelheit besser zu sehen
  var tmp = AddLightAmbience(30,this());
  var r,g,b,a;
  SplitRGBaValue(rgb,r,g,b,a);
  tmp->ChangeColor(RGBa(r,g,b,a+30));
  SetVisibility(VIS_All,tmp);

  AddEffect("Collect", this(), 101, 5, this());
}

protected func Entrance(object obj)
{
	var canCarryFlag = obj->~CanCarryFlag();
	
	// kann keine Flagge haben - die Flagge wird bei Eintritt fallengelassen (default)
	if(!canCarryFlag)
	{
		AttachTargetLost();
	}
	// komischer Spezialfall (z.b. für Fahrstuhl) - Clonk nimmt Flagge mit rein (so wie früher)
	else if(canCarryFlag < 0)
	{
		return;
	}
	// Flagge klebt außen dran (z.B. Panzer, Shuttle)
	else
	{
		SetAction("Attach", obj);
		SetActionData(obj->~GetFlagAttachVertex());
		AddEffect("WaitForCarrierExit", this(), 101, 1, this());
	}
}

private func FxWaitForCarrierExitTimer(target, no)
{
	if(!carrier) return -1;
	
	if(!Contained(carrier))
	{
		SetAction("Attach",carrier);
		return -1;
	}
}

/* Einsamml0rn!!11 */

protected func Collected(pClonk)
{
  Log("$HasTheFlag$", GetPlayerName(GetOwner(pClonk)), GetTeamName(team));
  SetAction("Attach", pClonk);
  
	carrier = pClonk;
  cteam = GetPlayerTeam(GetOwner(pClonk));
  
  // game call: FlagCaptured(flagTeam, captureTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // captureTeam: The team that captured the flag
  // clonk: the clonk who did it
  GameCallEx("FlagCaptured",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
  Sound("FlagCaptured",1);
  return(1);
}

protected func CheckCollect(pClonk)
{
  if(WildcardMatch(GetAction(), "*Attach*")) return();
  if(GetPlayerTeam(GetOwner(pClonk)) == team)
    {
    if(GetAction() ne "Lost") CheckFlag(pClonk);
    else Return2Base(pClonk);
    return();
    }
  if(FindObject(GetID(), 0,0,0,0,0, 0, pClonk)) return();  // Mehr als 2 Teams ftw!

  return(Collected(pClonk));
}

protected func AttachTargetLost()
{
  SetAction("Lost");
  SetActionTargets();
	carrier = 0;
  SetDir();
	Exit();
  // falls sie festsitzt, wird sie sofort zurückgebracht
  if(GBackSolid()) {
    RemoveObject();
	return();
  }
  Log("$FlagLost$", GetTeamName(team));
  GameCallEx("FlagLost",team);
}

/* Zurückfliegen und Punkte vergeben */

public func IsAtHome() {
	if(GetActionTarget() == base)
		return(true);
}

public func GetCarrier() {
	if(WildcardMatch(GetAction(), "*Attach*")) {
      return(carrier); 
	}
}

protected func Return2Base(pClonk, nolog)
{
  SetR();
  SetRDir();
  SetAction("Fly", base);
	carrier = 0;
  if(!nolog)
  {
    Log("$ReturnedTheFlag$", GetPlayerName(GetOwner(pClonk)));
    Sound("FlagReturned",1);
  }
  // game call: FlagReturned(flagTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // clonk: the clonk who did it
  GameCallEx("FlagReturned",team, GetPlayerTeam(GetOwner(pClonk)));
}

protected func CheckFlag(pClonk)
{
  var flag = FindObject(GetID(), 0,0,0,0,0, 0,pClonk);
  if(!flag) return();

  Log("$CapturedTheFlag$", GetPlayerName(GetOwner(pClonk)));
  DoWealth(GetOwner(pClonk), 50); // Geld!
  DoTeamScore(GetPlayerTeam(GetOwner(pClonk)), 1);
  flag->~Return2Base(0,1);
  // game call: FlagScored(flagTeam, scoreTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // scoreTeam: the team that just scored
  // clonk: the clonk who did it
  GameCallEx("FlagScored",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
  Sound("TeamChange",1);
}

// verbessertes Einsammeln
public func FxCollectTimer(target, no)
{
  if(WildcardMatch(GetAction(target),"*Attach*")) return();
  var clonk;
  while(clonk = FindObject(0, -20, -20, 40, 40, OCF_CrewMember(), 0,0, NoContainer(), clonk))
    if(clonk->GetOCF() & OCF_Alive())
      if(CheckCollect(clonk))
        return();
}

/* Kaputt! */

public func Destruction()
{
  if(!base) return;
  // Flaggen dürfen nicht einfach weg sein
  var nFlag = CreateObject(FLA2, 0,0, GetOwner());
  nFlag->Activate(base, team, GetColorDw());
  // game call: FlagReturned(flagTeam)
  // flagTeam: The team to which the flag belongs to
  GameCallEx("FlagReturned",team);
  Log("$FlagReturned$", GetTeamName(team));
	Sound("FlagReturned",1);
}

public func NoWarp() { return(1); }

public func GetBase() { return(base); }

public func GetTeam() { return (team); }
