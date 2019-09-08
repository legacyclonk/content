/*-- Zapnest --*/

#strict

/* Aufschlag */

protected func Hit()
  {
  return(BreakUp(28));
  }

public func BreakUp(int num, bool alchemy)
{
  if (alchemy)
  {
  var offsetx=(2*GetDir(Contained(this))-1)*15;
  var offsety= 8;
  }
  var x,y;
  if(!num) num=28;
  for (var i=0;i<num;++i)
  {
    x=offsetx+Random(11)-5;
    y=offsety+Random(11)-5;
    if (!GBackSolid(x,y))
      CreateObject(ZAP1,x,y,-1);
  }
  RemoveObject();
}

public func IsAlchemContainer() { return(true); }
public func AlchemProcessTime() { return(100); }

public func Exhaustion() { BreakUp(14,1); return(1); }