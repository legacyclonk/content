/*-- Thron --*/

#strict

local pReviveObject;

/* Initialisierung */

protected func Initialize()
  {
  UpdateState();
  return(1);
  }


/* Eingangssteuerung */

private func IsOccupied()
  {
  return(FindContents(KING));
  }

private func UpdateState()
  {
  if (IsOccupied())
    if (GetAction() eq "Empty") return(SetAction("Occupied")); else;
  else
    if (GetAction() ne "Empty") return(SetAction("Empty"), pReviveObject=0);
  return(0);
  }

protected func ActivateEntrance(pClonk)
  {
  // Ausgangsanfrage
  if (Contained(pClonk)) return(OnExit(pClonk));
  // Eingangsanfrage (nur Könige)
  if (GetID(pClonk) == KING) return(OnEntrance(pClonk)); else if (IsOccupied()) return(CheckChallenge(pClonk));
  // Abgelehnt!
  return(); 
  }

private func OnEntrance(pClonk)
  {
  // Schon besetzt
  if (IsOccupied()) return(CheckChallenge(pClonk));
  // Betreten
  Enter(this(),pClonk);
  Sound("ThroneEnter");
  // Besitzerwechsel?
  var iOwnOwner = GetOwner();
  if (iOwnOwner != GetOwner(pClonk))
    {
    var pRule;
    SetOwner(GetOwner(pClonk));
    if ((pRule = FindObject(_KTK)) && iOwnOwner>=0) pRule->PlayerForfeit(iOwnOwner, GetOwner(pClonk));
    }
  UpdateState();
  return(1);
  }

private func OnExit(pClonk)
  {
  Exit(pClonk,0,4);
  UpdateState();
  return(1);
  }

private func CheckChallenge(pClonk)
  {
  // Ein feindlicher Clonk von außen fordert den Insassen
  var pKing = FindContents(KING); if (!pKing) return();
  if (!Hostile( GetOwner(pClonk), GetOwner(pKing))) return();
  // Insasse kommt heraus (aber der Herausforderer verliert sein Enter-Kommando)
  OnExit(pKing);
  SetCommand(pClonk, "None");
  // Damit man nicht einfach so wieder hrein kann, wird auch ein wenig zugeschlagen
  Punch(pKing, 5); Sound("Punch*");
  return();
  }


/* Ausrichtung */
  
protected func ControlRight()
  {
  [$CtrlRotateThrone$]
  Sound("Lever1");
  SetDir(DIR_Right());
  return(1);
  }

protected func ControlLeft()
  {
  [$CtrlRotateThrone$]
  Sound("Lever1");
  SetDir(DIR_Left());
  return(1);
  }


/* Magiereload */

protected func Reloading()
  {
  var pKing = FindContents(KING);
  if (!pKing) return(UpdateState());
  if (DoMagicEnergy(1, pKing)) CreateParticle("PxSpark",0,0,0,0,10,RGB(0,0,255));
  return(1);
  }


/* Status */

protected func RejectEntrance(pContainer) { return(1); } // Kann nirgends hineinversetzt werden
protected func NoSell() { return(1); } // Kann nicht verkauft werden



/* Zerstörung */

protected func Destruction()
  {
  var pRule;
  if ((pRule = FindObject(_KTK)) && GetOwner()>=0) pRule->PlayerForfeit(GetOwner(), -1);
  return(1);
  }



/* Wiederbelebung */

public func PlayerEliminated(iPlr)
  {
  // Einen neuen Clonk für den Spieler erzeugen, und in ein Relaunchobjekt im Thron versetzen
  var pClonk = CreateObject(KNIG, 0,0, iPlr); if (!pClonk) return();
  MakeCrewMember(pClonk, iPlr);
  var pRelauncher = CreateObject(_RLN, 0,0, iPlr);
  if (!pRelauncher) return(0, RemoveObject(pClonk));
  Enter(this(), pRelauncher);
  return(Enter(pRelauncher, pClonk));
  }

protected func ContainedUp(pKing)
  {
  [$CtrlRevive$|Image=_RLN]
  if (!pKing) return();
  // Schon am Wiederbeleben? Statusanzeige
  if (pReviveObject) return(PlayerMessage(GetOwner(pKing), "$MsgReviveProcess$: %d%", pKing, GetReviveProcess()));
  // Nichts zum Wiederbeleben?
  if (!ContentsCount(_RLN)) return(Sound("Error"), Message("$MsgNothingToRevive$", this()));
  // Wiederbelebungsmenü anzeigen
  CreateMenu(KING, pKing, this(), 0, "$MnuRevive$", 0, 1);
  var pReviveObj;
  while (pReviveObj = FindObject(_RLN, 0,0,0,0, 0, 0,0, this(), pReviveObj))
    {
    var iClonkOwner = GetOwner(pReviveObj);
    var pClonk = GetHiRank(iClonkOwner);
    AddMenuItem(GetPlayerName(iClonkOwner), "MnuRevive", GetID(pClonk), pKing, GetRank(pClonk), iClonkOwner, "$MnuReviveInfo$", 1);
    }  
  }

protected func ContainedDown(pKing)
  {
  [$CtrlAbort$|Image=DOOR]
  // Wiederbelebung: Abbrechen
  if (pReviveObject) return(SetAction("Occupied"), pReviveObject=0);
  // Ansonsten: Reguläres Verlassen
  return(OnExit(pKing));
  }

protected func MnuRevive(idItem, iPlr)
  {
  // König muss noch im Thron sein
  var pKing = FindContents(KING);
  if (!pKing) return();
  // Wiederbelebungsobjekt suchen
  var i;
  while (pReviveObject = Contents(i++))
    if (GetID(pReviveObject) == _RLN)
      if (GetOwner(pReviveObject) == iPlr)
        break;
  if (!pReviveObject) return();
  // Benötigte Flagge suchen
  if (!pReviveObject->SacrificeMade())
    {
    var pFlag = FindContents(FLAG);
    if (!pFlag) pFlag = FindContents(FLAG, pKing);
    if (!pFlag)
      {
      pReviveObject = 0;
      Sound("Error");
      return(Message("$MsgNoFlag$"));
      }
    RemoveObject(pFlag);
    pReviveObject->SacrificeMade()=1;
    }
  // Wiederbelebung starten
  return(SetAction("Relaunching"));
  }

public func GetReviveProcess()
  {
  if (!pReviveObject) return();
  return(pReviveObject->GetRelaunchProcess());
  }

protected func RelaunchProcess()
  {
  // Nix mehr zum Relaunchen da?
  if (Contained(pReviveObject) != this() || !IsOccupied()) return(SetAction("Empty"), pReviveObject=0);
  // Weitermachen
  return (pReviveObject->DoRelaunchProgress());
  }

public func RelaunchDone(pRelaunchObj)
  {
  // OK, fertig
  pReviveObject = 0;
  return(SetAction("Occupied"));
  }
