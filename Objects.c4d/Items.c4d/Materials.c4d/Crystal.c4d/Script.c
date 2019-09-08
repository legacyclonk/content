/*--- Kristall ---*/

#strict

protected func Hit()
{
  Sound("Crystal*");
  return(1);
}

/* Verwendung */

public func Apply(object obj)
{
  var used = false;
  var times;
  var maxtimes = (GetCon() - 50) / 5;

  for(var i = 0; i < maxtimes; ++ i)
  {
    if(DoMagicEnergy(1, obj) )
    {
      used = true;
      if(GetCon() > 55)
        DoCon(-5);
      else
        RemoveObject();
    }
  }
  return(used);
}

/* Verkauf entsprechend Groesse */

protected func CalcValue()
{
  return( (GetCon() - 50) / 10 + (5 - (GetCon() / 20)) );
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(150); }
