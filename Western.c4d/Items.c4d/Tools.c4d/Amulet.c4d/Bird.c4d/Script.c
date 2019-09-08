/*-- Vogelamulett --*/

#strict 2

/* Aktivieren: Anlegen */

public func Activate(object pClonk) {
  [$TxtPutOn$|Condition=IsFullCon]
  var pObj;
  // Nur für Indianer
  if(!(pClonk->~IsIndian())) return(0);
  // Da Amulette nur unter folgenden Bedingungen abnehmbar sind
  if(Contained(pClonk)) {
    if(!(GetCategory(Contained(pClonk)) & (C4D_Structure|C4D_Vehicle))) return(0);
  }
  else if(!(pClonk->~IsWeaponReady())) return(0);
  // Ist das Amulett vollständig?
  if(!IsFullCon()) return(0);
  // Zudem wird das davor angelegte Amulett nochmal eingesammelt
  Exit(this());
  if(pObj = FindObject(0,0,0,0,0,0,"WearAmulet",pClonk)) pObj->~UnWear();
  SetAction("WearAmulet",pClonk);
  AddEffect("WearBless",pClonk,182,0,0,GetID());
  Message("$MsgPutOn$",pClonk,GetName());
  Sound("Connect");
  // Maus ignorieren und keine Clonks treffen
  SetCategory(C4D_Living|C4D_MouseIgnore);
  SetAlive(0); // aber nicht lebend
  return(1);
}

public func IsFullCon() { return(GetCon() == 100); }

/* Verbesserungseffekt */

protected func FxWearBlessStart(object pTarget, int iEffectNumber, int iTemp)
{
  // Temporäre Aufrufe ignorieren
  if(iTemp) return(0);
  // Höhere Sprungkraft (+ Extra-Atemluft)
  IncPhysical("Jump",  14000,3,pTarget);
  IncPhysical("Breath",20000,3,pTarget);
  return(1);
}

protected func FxWearBlessStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return(0);
  // Attribute wiederherstellen
  ResetPhysical(pTarget,"Jump");
  ResetPhysical(pTarget,"Breath");
  // Bei Tod Amulett verlieren
  var pAmulet = FindObject(0,0,0,0,0,0,"WearAmulet",pTarget);
  if(pAmulet && iReason==4) ObjectSetAction(pAmulet,"Idle");
  // Damit es danach noch aus jeglichen Gebäuden herrausgenommen werden kann
  SetCategory(C4D_Object,pAmulet);
  return(1);
}

private func IncPhysical(string szPhysical, int iInc, int iMode, object pObject)
{
  return(SetPhysical(szPhysical,GetPhysical(szPhysical,0,pObject)+iInc,iMode,pObject));
}

/* Aktionen */

// Damit Gegner/andere Clonks dein Amulett nicht irgendwie benutzen können
public func CheckCaller(object pCaller, id iID, object pClonk) { return(pCaller == pClonk); }

/* Ablegen */

protected func AttachContextUnWear(object pCaller, object pClonk)
{
  [$TxtUnWear$|Image=AMBR|Condition=CheckCaller]
  // Nur wenn der Clonk bereit ist
  if(pClonk->~IsWeaponReady()) UnWear();
  // Wenn er in einem Gebäude bzw. Fahrzeug ist, sollte er es auch ablegen können
  if(Contained(pClonk))
    if(GetCategory(Contained(pClonk)) & (C4D_Structure|C4D_Vehicle))
      UnWear();
  // Hier nicht abnehmbar
  return(0);
}

public func UnWear()
{
  var pClonk = GetActionTarget();
  SetAction("Idle");
  RemoveEffect("WearBless",GetActionTarget());
  // Maus nicht mehr ignorieren, kann wieder treffen
  SetCategory(GetCategory(0,GetID(this())));
  // Wenn vom Clonk einsammelbar, dann soll er es gleich einsammeln
  if(!(pClonk->~RejectCollect(GetID(this()),this())))
    Enter(pClonk,this());
  // Sollte er ansonsten in einem Gebäude sein
  else if(Contained(pClonk))
    Enter(Contained(pClonk),this());
  return(true);
}

/* Zerkleinern von Knochen (Energiegewinn) */

public func AttachContextDecompose(object pCaller, object pClonk) {
  [$MnuDecompose$|Image=SKUL|Condition=FindDecomposeItemCheck]
  if(!WildcardMatch(GetAction(pClonk),"*Walk*")) return(0);
  AddEffect("IntDecompose",pClonk,1,1,0,GetID(),FindDecomposeItem(pClonk),this());
}

public func FindDecomposeItemCheck(object pCaller, id iID, object pClonk)
{
  var i, pObj;
  // Nur wenn es der Clonk selbst ist
  if(pCaller != pClonk) return(0);
  // Kann man mit diesem Amulett zaubern?
  if(!AmuletCanMagic()) return(0);
  // Der Clonk muss draußen sein
  if(Contained(pClonk)) return(0);
  // Nicht bei voller Zauberenergie
  if(GetMagicEnergy(pClonk) >= (GetPhysical("Magic",0,pClonk)/1000)) return(0);
  // Ist ein Objekt zur Zerkleinerung vorhanden?
  return(FindObject2(Find_Container(pClonk),Find_Func("IsManaContainer")));
}

public func FindDecomposeItem(object pClonk)
{
  var i, pObj;
  // Der Clonk muss draußen sein
  if(Contained(pClonk)) return(0);
  // Nicht bei voller Zauberenergie 
  if(GetMagicEnergy(pClonk) >= (GetPhysical("Magic",0,pClonk)/1000)) return(0);
  // Das nächste zerkleinerbare Objekt im Inventar suchen
  for(i = 0; pObj = Contents(i,pClonk); i++) {
    // Ist dieses zerkleinerbar?
    if(!pObj->~IsManaContainer()) continue;
    // Ist ein anderes bereits zerkleinertes Objekt derselben ID im Inventar vorhanden?
    if(pObj = FindObject2(Sort_Func("GetCon"),Find_Func("IsManaContainer"),Find_Container(pClonk),Find_Not(Find_OCF(OCF_Fullcon))))
      // Verfügbar -> Nehmen!
      return(pObj);
    // Ansonsten das jeweilige gefundene nehmen
    return(Contents(i,pClonk));
  }
}

public func FxIntDecomposeStart(object pTarget, int iEffectNumber, int iTemp, object pAlchemContainer, object pAlchemManager)
{
  // Temporäre Aufrufe ignorieren
  if(iTemp) return(0);

  var szAction;

  // Objekt zum Zerkleinern und Amulett aufnotieren
  EffectVar(0,pTarget,iEffectNumber) = pAlchemContainer;
  EffectVar(1,pTarget,iEffectNumber) = pAlchemManager;

  // Manaausbeute nach Mystikpunkten festlegen
  EffectVar(4,pTarget,iEffectNumber) = (GetClanMystic(GetClan(pTarget))+200)*(pAlchemContainer->~ManaProcessTime())/400;
  // Häuptlinge erhalten eine höhere Manaausbeute
  if(pTarget->~IsChief()) EffectVar(4,pTarget,iEffectNumber) = EffectVar(4,pTarget,iEffectNumber)*4/3;

  // Vorhandene Zeit
  EffectVar(3,pTarget,iEffectNumber) = EffectVar(4,pTarget,iEffectNumber)*GetCon(pAlchemContainer)/100;

  // Aktion festlegen
  if(CheckAction("Process",pTarget)) szAction = "Process";
  else szAction = "FlatUp";
  ObjectSetAction(pTarget, szAction);
  // Nochmal aufnotieren
  EffectVar(2,pTarget,iEffectNumber) = szAction;

  // Objekt vor sich hinlegen
  Exit(pAlchemContainer,GetX(pTarget)+GetDir(pTarget)*20-10,GetY(pTarget)+12);
  // Nicht einsammelbar
  SetObjectLayer(pAlchemContainer,pAlchemContainer);
  // Damit der Clonk nach dem Zerkleinern nicht weiterläuft
  SetComDir(COMD_Stop,pTarget);

  // Bereit zum Zerkleinern
  return(1);
}

public func FxIntDecomposeTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  var pAlchemContainer = EffectVar(0,pTarget,iEffectNumber),
      pAlchemManager = EffectVar(1,pTarget,iEffectNumber),
      szAction = EffectVar(2,pTarget,iEffectNumber);

  // Knochen weg
  if(!pAlchemContainer) return(-1);
  if(Contained(pAlchemContainer)) return(-1);
  if(ObjectDistance(pAlchemContainer,pTarget) > 18) return(-1);
  // Amulett weg
//  if(!pAlchemManager) return(-1);
  if(pAlchemManager != pTarget && GetAction(pAlchemManager) != "WearAmulet") return(-1);

  // Zeit zuende
  if(iEffectTime > EffectVar(3,pTarget,iEffectNumber)) {
    RemoveObject(pAlchemContainer);
    return(-1);
  }

  if(GetAction(pTarget) != szAction) return(-1);
  if(szAction == "FlatUp") SetPhase(4,pTarget);

  if(!(iEffectTime % 4)) {
    if(!DoMagicEnergy(1,pTarget)) return(-1);
    CastObjects(SPRK,1,20,GetX(pAlchemContainer),GetY(pAlchemContainer));
    SetCon((EffectVar(3,pTarget,iEffectNumber)-iEffectTime)*100/EffectVar(4,pTarget,iEffectNumber),pAlchemContainer);
  }
  return(1);
}

public func FxIntDecomposeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Temporäre Aufrufe ignorieren
  if(fTemp) return(0);
  var pAlchemContainer = EffectVar(0,pTarget,iEffectNumber);

  if(GetAction(pTarget) == EffectVar(2,pTarget,iEffectNumber)) {
    ObjectSetAction(pTarget,"KneelUp");
    if(!pAlchemContainer) return(1);
    if(ObjectDistance(pAlchemContainer,pTarget) <= 18)
      if(!(pTarget->~RejectCollect(0,pAlchemContainer)))
        Enter(pTarget,pAlchemContainer);
  }

  if(!pAlchemContainer) return(1);

  // Das Objekt sollte möglichst wiedereinsammelbar sein
  SetObjectLayer(0,pAlchemContainer);

  if(Contained(pAlchemContainer)) return(1);
  if(!Stuck(pAlchemContainer)) return(1);

  for(var i = 0; i <= 10; i++) {
    if(!Stuck(pAlchemContainer)) break;
    if(!GetY(pAlchemContainer)) {
      SetPosition(GetX(pAlchemContainer),GetY(pAlchemContainer)+i,pAlchemContainer);
      break;
    }
    SetPosition(GetX(pAlchemContainer),GetY(pAlchemContainer)-1,pAlchemContainer);
    if(i == 10)
      if(Stuck(pAlchemContainer))
        SetPosition(GetX(pAlchemContainer),GetY(pAlchemContainer)+i,pAlchemContainer);
  }
  return(1);
}

/* Zaubern */

public func AttachContextUse(object pCaller, object pClonk)
{
  [$TxtMagic$|Image=MCMS|Condition=CheckCaller]
  Magic(pClonk);
}

private func Magic(object pClonk)
{
  // Das Menu ist jederzeit zu öffnen
//  if(!(pClonk->~IsWeaponReady())) return(0);
  // Clonk soll anhalten
  SetComDir(COMD_Stop,pClonk);
  // Menü öffnen
  var iExtra, idMagic, i;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=C4MN_Extra_Components; else iExtra=C4MN_Extra_ComponentsLiveMagic; }
  else                  { if(ObjectCount(NMGE)) iExtra=C4MN_Extra_None;       else iExtra=C4MN_Extra_LiveMagicValue;      }
  CreateMenu(MCMS,pClonk,this(),iExtra,"$MnuNoSpells$",ObjectNumber(pClonk));
  for(i = 0; idMagic = GetSpell(i); i++)
    // Check, ob der Spruch geladen ist
    if(FindDefinition(idMagic))
      AddMenuItem("$TxtMagic$: %s","DoMagic",idMagic,pClonk,0,pClonk);
  // Letzten Spruch auswählen
  SelectMenuItem(2,pClonk);

  // Der Häuptling verfügt über zusätzliche Zauber
  if(pClonk->~IsChief()) {
    for(; i < 5; i++) AddMenuItem(0,0,0,pClonk);
    for(i = 0; idMagic = GetChiefSpell(i); i++)
      if(FindDefinition(idMagic))
        AddMenuItem("$TxtMagic$: %s","DoMagic",idMagic,pClonk,0,pClonk);
  }
}

protected func DoMagic(id idSpell, object pClonk)
{
  if(!pClonk) return(0);

  var iAction = GetAction(pClonk);
  // Nur wenn der Clonk bereit ist
  if(iAction != "Walk" && iAction != "SpearWalk" &&
     iAction != "Jump" && iAction != "SpearJump" &&
     iAction != "Ride" && iAction != "RideStill" &&
     iAction != "Swim" && iAction != "Dive" &&
     iAction != "KneelDown" && iAction != "KneelUp") {
    Sound("Error",0,pClonk,100,GetController(pClonk)+1);
    PlayerMessage(GetOwner(pClonk),"$MsgNotReadyToMagic$",pClonk,GetName(pClonk));
    return(0);
  }

  // Nicht in Tieren zaubern (Borgen)
  if(Contained(pClonk))
    if(GetCategory(Contained(pClonk)) & C4D_Living) {
      Sound("Error",0,pClonk,100,GetController(pClonk)+1);
      PlayerMessage(GetOwner(pClonk),"$MsgNotReadyToMagic$",pClonk,GetName(pClonk));
      return(0);
    }

  // Nicht genug Zauberenergie
  if(!CheckMana(idSpell,pClonk)) {
    Sound("Error",0,pClonk,100,GetController(pClonk)+1);
    PlayerMessage(GetOwner(pClonk),"$MsgNotEnoughEnergy$",pClonk);
    return(0);
  }

  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(idSpell,pClonk)) {
    Sound("Error",0,pClonk,100,GetController(pClonk)+1);
    PlayerMessage(GetOwner(pClonk),"$MsgNotEnoughIngredients$",pClonk,GetNeededAlcStr(idSpell,pClonk));
    return(0);
  }

  // Zauberobjekt erschaffen und aktivieren
  if(!(CreateObject(idSpell,0,0,GetOwner(pClonk))->~Activate(pClonk)))
    // Zauber fehlgeschlagen
    return(0);

  // Zauberaktion durchführen
  if(pClonk->~IsRiding()) ObjectSetAction(pClonk,"RideMagic");
  else if(iAction == "Walk" || iAction == "SpearWalk" ||
          iAction == "KneelDown" || iAction == "KneelUp")
    ObjectSetAction(pClonk,"Magic");

  // Zauberenergie abziehen
  DoMagicEnergy(-GetValue(0,idSpell),pClonk);

  // alchemische Zutaten abziehen
  if(ObjectCount(ALCO)) ReduceAlchem(idSpell,pClonk);

  // Fertig
  return(1);
}

protected func RejectEntrance() 
{ 
  if(GetID(Contained()) == NTIP) if(GetCon() < 100) {
    Message("$MsgNotReady$",this(),GetName(this()));
    return(1);
  }
  if(GetAction() == "WearAmulet") return(1); 
}

protected func Departure(pObj)
{
  if(GetID(pObj) == NTIP) if(GetCon() < 100) {
    Message("$MsgNotReady$", pObj, GetName(this()));
    Enter(pObj);
    return(1);
  }
  return(0);
}

protected func Destruction() { RemoveEffect("WearBless",GetActionTarget()); }

public func GetSpell(iIndex)
{
// Von nun an gibt es neue Zauber
/*
  if(!iIndex--) return(MGDW);
  if(!iIndex--) return(MWND);
  if(!iIndex--) return(MGTJ);
  if(!iIndex--) return(MWP3);
  if(!iIndex--) return(SMGC);
*/
  if(!iIndex--) return(MWP3);
  if(!iIndex--) return(MWND);
  if(!iIndex--) return(MLGT);
  if(!iIndex--) return(MGTJ);
  return(0);
}

/* Die Zauber des Chiefs */
public func GetChiefSpell(int iIndex) { return(GetActionTarget()->~GetSpell(iIndex)); }

/* Das ist ein Amulett */
public func IsIndianAmulet() { return(1); }

/* Hiermit kann man zaubern und Knochen zerkleinern */
public func AmuletCanMagic() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Es kann nicht ohne das Schamanenzelt gezaubert werden */
public func GetResearchBase() { return(NTIP); }