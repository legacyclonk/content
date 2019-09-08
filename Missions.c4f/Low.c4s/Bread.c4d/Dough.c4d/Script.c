/*-- Brotteig --*/

#strict

public func Bake()
{
  ChangeDef(BRED);
  return(1);
}

// Beim Aufprall zerplatzen

protected func Hit()
{
  Sound("DoughSplat");
  CastParticles ("DoughSlice", Random(3)+4, 13, 0,0, 30,35, RGB(170,115,60), RGB(145,95,50));
  return(RemoveObject(this()));
}

// Kann im Backtrog gemixt und im Ofen gebacken werden

public func IsDough() { return(1); }