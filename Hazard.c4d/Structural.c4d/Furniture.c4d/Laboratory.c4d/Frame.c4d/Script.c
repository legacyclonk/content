/*-- Frame --*/

#strict

local i_Action;

func Initialize() 
{
	Set(Random(5)+1);
}

public func Set(int i)
{
	i = BoundBy(i,1,5);
	SetAction( Format("Exist%d", i_Action = i) );
}

public func Damage()
{
  if( i_Action != 1)
  {
    SetAction("Exist1");
    for(var i = 5; i < 17; i += 2)
    {
      if(Random(2)) CastParticles("SplinterGlass", 2, 35, i, RandomX(17, 13), 20, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
    }
    Sound("Glass");
    
    i_Action = 1;
  }
}

public func IsBulletTarget(id def) 
{ 
  if(def->~NoDecoDamage()) return();
  if(i_Action != 1) 
    return(!Random(40)); 
    
  return(0); 
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("Set(%d)", i_Action);
}
