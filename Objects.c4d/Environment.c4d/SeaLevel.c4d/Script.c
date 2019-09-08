/*-- Meeresspiegel --*/

#strict

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  // Abflüsse platzieren
  var x = -150;
  while ((x += 200) < LandscapeWidth())
    CreateDrain(x, GetHorizonHeight(x));
  // Steuerungsobjekt wird nicht mehr benötigt
  RemoveObject();
  return(1);    
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() || GBackSemiSolid(x, y))
    --y;
  return(y);
}
    
private func CreateDrain(x, y)
{
  if (GBackLiquid(x, y + 1))
    return(CreateObject(DRAI, x, y, -1));
  return(0);
}
