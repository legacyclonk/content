/*-- Keksteig --*/

#strict

public func Bake()
{
  CastObjects(COKI, 3, 3);
  ChangeDef(COKI);
  return(1);
}

// Beim Aufprall zerplatzen

protected func Hit()
{
  Sound("DoughSplat");
  CastParticles ("DoughSlice", Random(3)+4, 13, 0,0, 30,35, RGB(220,175,85), RGB(205,165,80));
  return(RemoveObject(this()));
}

// Kann im Backtrog gemixt und im Ofen gebacken werden
public func IsDough() { return(1); }