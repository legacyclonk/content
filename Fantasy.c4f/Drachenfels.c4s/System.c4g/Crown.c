/* Von jedem einsammelbare, mehr regenerierende Krone */

#strict
#appendto CRWN

private func DoMagicEnergy(int iAmount, object pObj, a,b,c)
  {
  // Mehr Mana-Regeneration
  return (_inherited(iAmount+10, pObj, a,b,c));
  }

protected func RejectEntrance() { }
