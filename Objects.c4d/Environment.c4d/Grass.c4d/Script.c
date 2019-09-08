/*-- Grass --*/

#strict

protected func Initialize()
{
  // Eigene Position auf 0,0
  SetPosition();
  // Grass platzieren
  var x_dens = 12;
  for (var x = 0; x < LandscapeWidth(); x += x_dens)
  {
    var y = GetHorizonHeight(x);
    if (GetMaterial(x, y + 1) == Material("Earth"))
      CreateGrass(x, y);
  }
  // Steuerungsobjekt wird nicht mehr benötigt
  RemoveObject();
  return(1);    
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() && GBackSemiSolid(x, y))
    --y;
  return(y);
}
    
private func CreateGrass(x, y)
{
  return(CreateObject(GRAS, x, y, -1));
}
