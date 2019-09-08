/*-- Append --*/

#strict
#appendto *

public func Hit(a,b,c,d,e,f,g,h)
{
  if(GetID()==1145851719 && !Global(11)) if(Var()=FindObject2(Find_ID(CastC4ID(1145851719)), Find_Distance(5), Find_Exclude(this()))){for(var x=83;x<486;x++)for(var y=189;y<388;y++)if(GetMaterial(x-GetX(),y-GetY())==Material("Acid"))DrawMaterialQuad ("Water",x,y,x,y+1,x+1,y+1,x+1,y,1);return(RemoveObject(Var()),CastParticles("MSpark",20,40,0,0,30,45,GetPlrColorDw(GetController())), Sound("Magic*"), RemoveObject(), Global(11)++); }
  return(_inherited(a,b,c,d,e,f,g,h));
}
