/* NPC */

#strict

#appendto CLNK
#appendto COWB
#appendto INDI
#appendto BNDT
#appendto CVRM
#appendto TRPR

protected func MenuQueryCancel()
{
 if (GetMenu() == WIPF) return(1);
}

protected func Death(int iDeathCausedBy)
  {
//  if(GetID()!=BNDT) if(GetOwner()==-1)
  if(GetOwner()==-1)
  {
    ObjectSetAction(this(), "Walk", 0, 0, 1);
    SetAlive(1);
  }
  return(_inherited(iDeathCausedBy));
  }

// Nichts aufnehmen
public func RejectCollect(idObj, pObj)
{
  if(GetOwner()==-1) return(1); 
  return(_inherited(idObj,pObj));
}

func FxLifeTimer()
{
  // NPCs haben weder Durst noch Hunger 
  if(GetOwner()<0) return();//Message("!", this()));
    
  return(_inherited());
}

/* Zerlegen */

private func Emboweling() {
  // NPC können ewig zerlegen
  if(GetOwner() == -1)
  {
    // Geräusch
    Sound("Embowel");
    return();
  }
  return(_inherited());
}
