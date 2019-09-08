/*-- Sheriff --*/

#strict
#include COWB

protected func Initialize()
{
  SetGraphics(0, this(), SHRF);	
  return(_inherited());	
}

local fGrabbedInfo;

// Der Sheriff benutzt immer die InfoSektion eines Cowboys
protected func Recruitment()
{
  if(!fGrabbedInfo) // Ein Hack für Peter
    {
    fGrabbedInfo = true;
    SetColorDw(RGB(255,255));
    SetGraphics(0, this(), SHRF);
    SetPortrait("1", this(), SHRF);
    AddEffect("CheckImprison", this(), 1, 10, this());
    }
  // Weiter im Text
  return(_inherited());
}

// Anzahl verfügbarer Portraits
private func GetPortraitCount() { return(1); }

func FxCheckImprisonTimer(pTarget, iNumber)
{
  if(EffectVar(0, pTarget, iNumber)) return(RemoveObject(EffectVar(0, pTarget, iNumber)));
  if(GetCursor(GetOwner())!=pTarget) return();
  if(FindOffice())
  {
    var obj = FindBandit(400);
    if(obj)
    {
      EffectVar(0, pTarget, iNumber) = CreateObject(SIMP, 0, 0, GetOwner());
      EffectVar(0, pTarget, iNumber)->SetVisibility(VIS_Owner());
      ObjectSetAction(EffectVar(0, pTarget, iNumber), "Imprison",obj); 
    }
  }
}

func FxCheckImprisonStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();
  if(EffectVar(0, pTarget, iNumber)) RemoveObject(EffectVar(0, pTarget, iNumber));
}

/* Banditen einsperren */

public func ContextImprison()
{
  [$CtxImprison$|Image=SHSI|Condition=ImprisonCond]
  var bandit, office;
  // Banditen / Büro suchen
  bandit = FindBandit(50);
  office = FindOffice();
  // Eines ist nicht da?
  if(!bandit) return(PlayerMessage(GetOwner(), "$MsgNoBandit$", this()));
  if(!office) return(PlayerMessage(GetOwner(), "$MsgNoOffice$", this()));
  // Einsperren, den Wicht
//  office->Imprison(bandit, this());
  RemoveEffect("AIBandit", bandit); // Kämpft nicht mehr
  RemoveEffect("AIBanditBossSaloon", bandit);
  RemoveEffect("OrderDefend", bandit);
  RemoveEffect("AIBanditNoMove", bandit); // Kann sich wieder bewegen
  SetPhysical("Walk", 70000, 2, bandit);
  SetCommand(bandit, "Enter", office);
  AddCommand(bandit, "MoveTo", office);
  bandit->SetAction("Walk");
  LocalN("iOwner", bandit) = 0;
  // Cooler Spruch
  Message("$MsgImprisoned$", this());
  RemoveEffect("WatchEnergy", this());
  g_fSheriffSpoken = 4;
//  ChangeDef(SHRF);
  SetAction("Walk");
  SetObjectLayer(0, FindObject(_TLK, 0, 0, 0, 0, 0, "Attach", this()));
//  SetCrewEnabled(0);
//  SetCommand(this(), "MoveTo", 0, 1351, 307);
  return(1);
}

public func ImprisonCond()
{
  // Überpüft, ob man in der Lage ist, Banditen einzusperren
  return(FindBandit(50) && FindOffice());
}

public func FindBandit(int iRadius)
{
  // Sucht einen Banditen in der Nähe, den man einsperren könnte
  var bandit;
  while(bandit = FindObject(0, -iRadius/2, -iRadius/2, iRadius, iRadius, OCF_Alive(), 0,0, NoContainer(), bandit))
   // Ist ein Bandit und Hier: muss Boss sein
   if(bandit->~IsBandit() && bandit->~IsCaptain())
    // Spieler sind verfeindet
    if(Hostile(GetOwner(), GetOwner(bandit)))
     // Clonk ist geschwächt
     if(GetEnergy(bandit)<=GetPhysical("Energy", 0, bandit)/2000)   
      // juchei
      return(bandit);
}

public func FindOffice()
{
  // Sucht ein Büro in der Nähe, in das man Banditen sperren kann
  var office;
  while(office = FindObject(OfficeID(),0,0,0,0, OCF_Fullcon(), 0,0,0, office))
    // Büro ist verbündet
    if(!Hostile(GetOwner(), GetOwner(office)))
      // juchei
      return(office);
}

public func RejectCollect(idObj, pObj) { if(idObj==AMBO) return(0); return(1); }
public func Ejection(pObj) { if(GetID(pObj)==AMBO || GetID(pObj)==WINC || GetID(pObj)==HDCF)return(Enter(this(), pObj)); }

private func OfficeID() { return(SHOF); }

// Ist ein Sheriff
public func IsSheriff() { return(1); }
