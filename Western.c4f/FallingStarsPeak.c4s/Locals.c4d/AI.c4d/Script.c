/* Künstliche Intelligenzen */

#strict

#appendto BNDT

local iOwner;
local ordrData1, ordrData2, pOrdrTarget;

public func SetAI(szName, iInterval)
{
  return(AddEffect(Format("AI%s", szName), this(), 1, iInterval, this()));
}

func FxAIBanditStart(pThis, iNumber, fTmp)
{
  if(fTmp) return();
  ordrData1 = GetX(); ordrData2 = GetY();
  CreateContents(AMBO);
  CreateContents(AMBO);
  CreateContents(WINC);
  iOwner=-2;
}

func FxAIBanditTimer()
{
  if(GetCommand()) SetCommand(this(), "None");
  // Immer genug Munition
  if(!FindContents(AMBO)) CreateContents(AMBO);
  // Gegner in Reichweite für Nahkampf?
  var pTarget;
  while(pTarget = FindObject(0, -15,-15,30,30, OCF_Alive, 0, 0, pTarget)) if(Hostile(GetOwner(pTarget), GetOwner())) break;
  if (pTarget) if (!Random(3))
    if (Contained() == Contained(pTarget))
      return(FightWith(pTarget));

  // Distanz von der Verteidigungsposition ermitteln
  var iDx=Abs(GetX()-ordrData1), iDy=Abs(GetY()-ordrData2);
  // Keine feindliche Einheit in Reichweite
  pOrdrTarget=FindEnemyUnit();
  if (!pOrdrTarget) return(ExecuteWatch());
  // Winchester einsetzen (wenn möglich)
  if (IsRifleEquipped())
    if (ExecuteAttackRifle(pOrdrTarget))
      return(1);
  // Weiter
  return(1);
}

func FxAIBanditStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();	
  // Nichts fallen lassen	
  while(Contents()) RemoveObject(Contents());
  LocalN("idWeapon") = 0;
  // nur eine Muni schachtel
  CreateContents(AMBO);
  iBandits--;
  UpdateBandits();
  AddEffect("Disappear", this(), 1, 1, 0, 0, 500);
  //RemoveObject(pTarget);
  
}

public func FindEnemyUnit(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
public func ExecuteWatch(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
public func IsRifleEquipped(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
public func ExecuteAttackRifle(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }

global func GetOwner(pObj) 
{
  if(!pObj) pObj = this();
  if(LocalN("iOwner", pObj)) return(LocalN("iOwner", pObj));
  return(_inherited(pObj));
}

global func Hostile(iPlr1, iPlr2)
{
  if(iPlr1==-1 || iPlr2==-1) return(0);
  if(iPlr1==-2 || iPlr2==-2) return(iPlr1 != iPlr2);
  return(_inherited(iPlr1, iPlr2));
}
