#strict
#include FRCA

local pLast, pNext;

private func GetForceFieldTime() { return(100000); }
private func GetMaxDamage() { return(100000); }

protected func Initialize()
{
  ScheduleCall(this(), "UpdatePhase", 1);
  return(_inherited());
}

protected func Destroy()
{
  if (pLast) ScheduleCall(pLast, "UpdatePhase" ,1);
  if (pNext) ScheduleCall(pNext, "UpdatePhase", 1);
  return(_inherited());
}

private func UpdatePhase()
{
  SetPhase(0);
  if (pLast) SetPhase(3);
  if (pNext) SetPhase(1);
  if (pLast && pNext) SetPhase(2);
  SetSolidMask(GetPhase() * GetDefCoreVal("Width", "DefCore"), 0, GetDefCoreVal("Width", "DefCore"), GetDefCoreVal("Height", "DefCore"));
}

private func CheckStuck()
{
  var pClonk,iXChange,iX,iY;

  // Alle feststeckenden Clonks in der Nähe suchen
  while(pClonk=FindObject(0,-20,-20,40,40,OCF_CrewMember(),0,0,NoContainer(),pClonk))
  {
    iX=GetX(pClonk);
    iY=GetY(pClonk);
    while(Stuck(pClonk) && Inside(GetX(pClonk)-GetX(),-20,20))
      {
      if(!(iXChange=BoundBy(GetX(pClonk)-GetX(),-1,1))) iXChange=1;
      // zur Sicherheit...
      if(!Inside(GetX(pClonk)+iXChange,0,LandscapeWidth())) break;
      SetPosition(GetX(pClonk)+iXChange,GetY(pClonk),pClonk);
      }
    // verschieben fehlgeschlagen: rückgängig machen
    if(Stuck(pClonk)) SetPosition(iX,iY,pClonk);
  }
}

protected func Damage(int iChange)
{
  // Effekt
  Sound("Electric");
  CastParticles("MSpark", 5, 25, 0,0, 50, 100, RGBa(100,100,255,128), RGBa(0,0,255,0));
  
  // Lebenszeit verringern
//  EffectVar(0, this(), GetEffect("ForceFieldPSpell", this())) += GetForceFieldTime() / GetMaxDamage() * iChange;
}
