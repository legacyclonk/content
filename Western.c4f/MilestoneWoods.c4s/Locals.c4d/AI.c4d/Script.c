/* Künstliche Intelligenzen */

#strict

#appendto BNDT

local iOwner;
local ordrData1, ordrData2, pOrdrTarget;

public func SetAI(szName, iInterval, idWeapon)
{
  if(idWeapon) CreateContents(idWeapon);
  return(AddEffect(Format("AI%s", szName), this(), 1, iInterval, this()));
}

func FxAIBanditStart(pThis, iNumber, fTmp)
{
  if(fTmp) return();
  ordrData1 = GetX(); ordrData2 = GetY();
  CreateContents(AMBO);
  CreateContents(AMBO);
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
  // Revolver einsetzen (wenn möglich) 
  if (IsGunEquipped()) 
    if (ExecuteAttackGun(pOrdrTarget)) 
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
  // Boss lässt noch eine Wichester fallen
  if(this()->~IsCaptain()) CreateContents(WINC);
}

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
      return(1); 
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

public func ExecuteWatch(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
public func IsRifleEquipped(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
public func ExecuteAttackRifle(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
public func IsGunEquipped(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
public func IsWeaponReady(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
public func StopAction(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }
//public func ExecuteAttackGun(a,b,c,d,e,f) { return(_inherited(a,b,c,d,e,f)); }

static ai_idFirstEncounterCB;

private func FindEnemyUnit() 
{ 
  var pObj; 
  // Nächstgelegene Mannschaftsobjekte überprüfen 
  while (pObj = FindObject(0, 0,0,-1,-1, OCF_CrewMember(), 0, 0, 0, pObj)) 
    // Verfeindet? 
    if (Hostile(GetOwner(),GetOwner(pObj))) 
    {
      // Kontakt ans Szenscript melden
      if (ai_idFirstEncounterCB && ObjectDistance(pObj)<100)
      {
        // Erster Gegnerkontakt!
        var idEncounterCB = ai_idFirstEncounterCB;
        ai_idFirstEncounterCB = 0;
        GameCall(Format("AI_Encounter%i", idEncounterCB), this(), pObj);
      }
      // Feind gefunden 
      return(pObj); 
    }
  // Keine feindliche Einheit gefunden 
  return(0); 
}

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
//  if (ObjectDistance(pOrdrTarget) > 3500) return(ExecuteWatch());
  Message("!", this());
/*  // Bogen einsetzen (wenn möglich) 
  if (IsBowEquipped()) 
    if (ExecuteAttackBow(pOrdrTarget)) 
      return(1); 
  // Speer einsetzen (wenn möglich) 
  if (FindSpearContents()) 
    if (ExecuteAttackSpear(pOrdrTarget)) 
      return(1); */
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
