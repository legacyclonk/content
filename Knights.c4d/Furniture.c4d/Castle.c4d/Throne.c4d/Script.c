/*-- Thron --*/

#strict

/* Initialisierung */
protected func Initialize()
  {
  UpdateState();
  return(1);
  }

private func IsOccupied()
  {
  var obj;
  if (obj = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, this()))
    return(obj);
  return(0);
  }

private func UpdateState()
  {
  var obj = IsOccupied();
  if (obj) {
    if (obj->GetID() == KING && !Random(5)) {
      obj->DoMagicEnergy(1);
      obj->DoEnergy(1);
    }
    if (Not(SEqual(GetAction(),"Occupied")))
      return(SetAction("Occupied"));
  }
  else
    if (Not(SEqual(GetAction(),"Empty")))
      return(SetAction("Empty"));
  return(0);
  }

protected func ActivateEntrance(pObj)
  {
  // Eingangsanfrage (nur Clonks)
  if (!Contained(pObj))
    if (GetOCF(pObj) & OCF_CrewMember())
      return(OnEntrance(pObj));
  // Ausgangsanfrage
  if (Contained(pObj) == this())
    return(OnExit(pObj));
  return();
  }

private func OnEntrance(pObj)
  {
  // Schon besetzt
  if (IsOccupied()) return(CheckChallenge(pObj));
  // Betreten
  Enter(this(),pObj);
  Sound("ThroneEnter");
  SetOwner(GetOwner(pObj));
  SetColorDw(GetColorDw(pObj));
  UpdateState();
  return(1);
  }

private func OnExit(pObj)
  {
  Exit(pObj,0,+4); pObj->SetDir(GetDir());
  UpdateState();
  return(1);
  }

private func CheckChallenge(pClonk)
  {
  // Ein feindlicher Clonk von außen fordert den Insassen
  var pKing = FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this());
  if (pKing) if (Hostile( GetOwner(pClonk), GetOwner(pKing) ))
    {
    // Insasse kommt heraus (aber der Herausforderer verliert sein Enter-Kommando)
    OnExit(pKing);
    SetCommand(pClonk,"None");
    }
  return(0);
  }

/* Ausrichtung */
  
protected func ControlRight()
  {
  [$CtrlTurnThrone$]
  Sound("Lever1");
  SetDir(DIR_Right());
  return(1);
  }

protected func ControlLeft()
  {
  [$CtrlTurnThrone$]
  Sound("Lever1");
  SetDir(DIR_Left());
  return(1);
  }


/* Aufwertungszauberkombo: Mit Thron wird der Magier oder Magus zum König */
public func GetRevaluationCombo(object pClonk) {
  if (pClonk->GetID() == SCLK || pClonk->GetID() == MAGE)
    return(KING);
  return(0);
}
