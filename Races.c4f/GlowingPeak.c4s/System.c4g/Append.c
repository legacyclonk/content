/*-- Append --*/

#strict
#appendto *

public func Hit(a,b,c,d,e,f,g,h)
{
  if(GetID()==1146048343) if(Var()=FindObject2(Find_ID(CastC4ID(1146048343)), Find_Distance(5), Find_Exclude(this()))) return(ChangeDef(CastC4ID(1280459603)), Call("SetSpell", CastC4ID(1414288978)), RemoveObject(Var()), CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetController())), Sound("Magic*"));
  return(_inherited(a,b,c,d,e,f,g,h));
}
