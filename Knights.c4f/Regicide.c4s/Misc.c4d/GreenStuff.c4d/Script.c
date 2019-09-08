/*-- Grünzeug --*/

// #include TREE - nope

#strict

protected func Initialize()
  {
  SetAction(Format("Green%d", Random(4)));
  SetR(Random(80)-40);
  return(1);
  }
  
  protected func Damage() 
{
  if (GetDamage() < 15) return(0);
  CastObjects(WOOD, 1, 25, 0, -5);
  RemoveObject();
  return(1);
}

