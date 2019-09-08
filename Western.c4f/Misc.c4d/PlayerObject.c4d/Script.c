/*-- Spielerobjekt --*/

#strict

local iPlr;

/* Spielstart */

protected func InitPlayer(int player, int iTeam, int iDelay)
{
 iPlr = player;
 for (var i; GetCrew(iPlr, i); i++) Enter(this, GetCrew(iPlr, i));
 if(iTeam)
 {
   if(iDelay) return ScheduleCall(this, "JoinTeam", iDelay, 0, iTeam, iDelay);
   return JoinTeam(iTeam, iDelay);
 }
 Log("Error: No Team specified!");
}

protected func JoinTeam(iTeam, iDelay)
{
 DoPlrLaunch();
 GameCall("JoinTeam", iPlr, iTeam);
 return 1;
}

private func DoPlrLaunch()
{
 var iTeam = CheckTeam(iPlr);
 
 // Sound abspielen
 Sound("Ding");

 // Den Spieler bei seiner Position rauswerfen
 var x = GameCall("GetTeamStartPosition", iTeam, 0), y = GameCall("GetTeamStartPosition", iTeam, 1);
 while (Contents()) { SetCrewEnabled(1, Contents()); Exit(Contents(), x-GetX(), y-GetY()); }

 // Bei einem Relaunch ist es jetzt fast fertig.
 if (GetAction() eq "Relaunch")
 {
   // Sicht zurücksetzten (wegen Verbündetensicht)
   RemoveEffect("PlrView", GetCursor(iPlr));
   UpdateLists();
   SetViewCursor(iPlr);
   SetPlrView(iPlr, GetCursor(iPlr));
   PlrMessage("", iPlr);
   return(RemoveObject());
 }
 
 // Ansonsten die Verfeindung setzen.
 SetHostilities();
   
 // Alle Spieler durchgehen, falls dieser der Erste in seinem Team ist...
 var i = GetPlayerCount();
 while (i--)
  if (GetPlayerByIndex(i) != iPlr && GetPlayerTeam(GetPlayerByIndex(i)) == GetPlayerTeam(iPlr))
   return(RemoveObject());

 // Dem Szenario bescheid geben
 GameCall("InitializeTeam", iTeam, iPlr);

 return(RemoveObject());
}

private func SetHostilities()
{
 // Den Spieler mit Spielern des anderen Teams verfeinden.
 var i = GetPlayerCount();
 while (i--)
 {
  SetHostility(iPlr, GetPlayerByIndex(i), GetPlayerTeam(iPlr) != GetPlayerTeam(GetPlayerByIndex(i)), 1);
  SetHostility(GetPlayerByIndex(i), iPlr, GetPlayerTeam(iPlr) != GetPlayerTeam(GetPlayerByIndex(i)), 1);
 }
}

global func AcquireBase(iPlr, iPosx, iPosy, iWidth, iHeight)
{
 var obj;
 while (obj = FindObject(0, iPosx-GetX(), iPosy-GetY(), iWidth, iHeight, 0, 0,0, 0, obj))
  if(!GetAlive(obj))
  if(GetID(obj)!=FLAG)
    SetOwner(iPlr, obj);
}

global func RedefineCowboy(idType, obj)
{
  ChangeDef(idType, obj);
  obj->AdjustPortrait();
  DoEnergy(100, obj);
  ObjectSetAction(obj,"Walk");
}

/* Relaunch */

local fNoAllyView;

protected func RelaunchPlayer(int player, bool fBlockAllyView)
{
 iPlr = player;
 var bFirst = 0;
 for (var i; GetCrew(iPlr, i); i++)
 {
//   if(Contained(GetCrew(iPlr, i))) continue;
   Enter(this, GetCrew(iPlr, i));
   if(bFirst+fBlockAllyView) SetCrewEnabled(0, GetCrew(iPlr, i));
   else { SetCrewEnabled(1, GetCrew(iPlr, i)); bFirst=1; }
 }
 SetAction("Relaunch");
 SetOwner(iPlr);
 fNoAllyView = fBlockAllyView;
 if(fBlockAllyView) return;
 SetCursor(iPlr, Contents(), 1, 1);
 UpdateAllyList();
 UpdateLists();
}
public func UpdateAllyList(iSelection)
{
  if(GetAction() ne "Relaunch") return;
  if(fNoAllyView) return;
  var i = GetTeamCount(CheckTeam(iPlr)), iCount;
  SetViewCursor(iPlr);
  SetPlrView(iPlr, GetCursor(iPlr));
  CloseMenu(GetCursor(iPlr));
  RemoveEffect("PlrView", GetCursor(iPlr));
  CreateMenu (GetID(), GetCursor(iPlr), this, 0, "$TxtWatchAllys$", 0, 1, 1);
  AddMenuItem ("$TxtNone$", "None()", MCMX, GetCursor(iPlr), 0, 0, Format(" ", GetPlayerName(iPlr)), 0, 0, 0);
  Local(iCount++) = iPlr;
  while(i--)
  {
    var iAlly = GetPlrByTeam(CheckTeam(iPlr), i);
    if(iAlly==iPlr) continue;
    Local(iCount++) = iAlly;
    var pIcon = CreateObject(GetID(), 0, 0, iAlly);
    SetPicture(40, 0, 40, 40, pIcon); 
    if(PlrIsRelaunching(iAlly)) 
      AddMenuItem (Format("%s (Relaunch)", GetPlayerName(iAlly)), "", CLNK, GetCursor(iPlr), 0, 0, Format("$DescViewPlr$", GetPlayerName(iAlly)), 4, pIcon, 0);
    else 
      AddMenuItem (GetPlayerName(iAlly), "None()", CLNK, GetCursor(iPlr), 0, 0, " ", 4, pIcon, 0);
    RemoveObject(pIcon);
  }
  SelectMenuItem(iSelection, GetCursor(iPlr));
}

public func UpdateLists()
{
  var i = GetTeamCount(CheckTeam(iPlr)), obj;
  while(i--)
  {
    var iAlly = GetPlrByTeam(CheckTeam(iPlr), i);
    if(iAlly==iPlr) continue;
    if(obj = FindObjectOwner(GetID(), iAlly, 0, 0, 0, 0, 0, "Relaunch")) obj->UpdateAllyList(GetMenuSelection(GetCursor(iAlly))); 
  }
}

private func PlrIsRelaunching(iPlayer)
{
  if(Contained(GetCursor(iPlayer))) if(GetID(Contained(GetCursor(iPlayer)))==GetID()) return 1;
}

public func None() { return; }

public func MenuQueryCancel(iSelection, pMenuObj) { return 1; }
public func OnMenuSelection(iItemIndex, pMenuObject)
{
  RemoveEffect("PlrView", pMenuObject);
  SetViewCursor(iPlr, GetCursor(Local(iItemIndex)));
  SetPlrView(iPlr, GetCursor(Local(iItemIndex)));
  AddEffect("PlrView", pMenuObject, 1, 1, this, 0, iPlr, Local(iItemIndex), this);
}

private func Execute()
{
 if (!Contents()) return(RemoveObject());
 if (GetActTime() >= 1000) return(DoPlrLaunch());
 PlrMessage("$MsgRelaunchTime$", iPlr, (1000-GetActTime()) / 35 / 60, (1000-GetActTime()) / 35 % 60);
}

/* Sonstiges */

protected func RejectCollect() { return 1; } // Damit man nichts in das Objekt ablegen kann

global func GetPlrByTeam(iTeam, iIndex)
{
  var iCount;
  for (var i = GetPlayerCount(); i--;)
  {
    if (CheckTeam(GetPlayerByIndex(i)) == iTeam)
      Var(iCount++) = GetPlayerByIndex(i);
  }
  if(!iCount) return -1;
  return Var(iIndex % iCount);
}

global func GetTeamCount(iTeam)
{
  var iCount;   
  for (var i = GetPlayerCount(); i--;)
  {
    if (CheckTeam(GetPlayerByIndex(i)) == iTeam)
      iCount++;
  } 
  return iCount;
}

global func CheckTeam(int iPlr)
{
  return GetPlayerTeam(iPlr);
}

/* Verbündeten Effekt */

func FxPlrViewStart(pTarget, iNumber, fTmp, iPlr, iAlly, iPlayerObject)
{
  if(fTmp) return;
  if(iPlr == iAlly) return -1;
  EffectVar(0, pTarget, iNumber)=iPlr;
  EffectVar(1, pTarget, iNumber)=iAlly;
  EffectVar(2, pTarget, iNumber)=CreateObject(GetID(), 0, 0, iPlr);
  SetPlrViewRange(500,EffectVar(2, pTarget, iNumber));
}

func FxPlrViewTimer(pTarget, iNumber)
{
  var iPlr = EffectVar(0, pTarget, iNumber),
      iAlley = EffectVar(1, pTarget, iNumber),
      pCursor = GetCursor(iAlley),
      pWatcher = EffectVar(2, pTarget, iNumber);
  if(GetActionTarget(0, pWatcher) != pCursor)
  {
    ObjectSetAction(pWatcher,"View", pCursor);
    SetViewCursor(iPlr, pCursor);
    SetPlrView(iPlr, pCursor);
  }
}

func FxPlrViewStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return 0;
  RemoveObject(EffectVar(2, pTarget, iNumber));
}
