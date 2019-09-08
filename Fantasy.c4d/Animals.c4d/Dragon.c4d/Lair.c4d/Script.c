/*-- Drachenhorst --*/

#strict

protected func Initialize()
{
  // Positionierung durch FindConstructionSite
  var posx = GetX();
  var posy = GetY();
  if (FindConstructionSiteX(GetID(), posx, posy))
  {
    SetPosition(posx, posy - 28);  
  }
  
  // sonst: Positionierung an Erdoberfläche
  else
  {
    var begin_x = GetX(), begin_y = GetY();
    SetSolidMask();
    while(GBackSemiSolid(0, GetObjHeight()/2 - 10) && GetY() > 0)
      SetPosition(GetX(), GetY() - 1);
    if(GBackSemiSolid(0, GetObjHeight()/2 - 10) )
      SetPosition(begin_x, begin_y);
    else
      SetPosition(GetX(), GetY() + 5);
    SetSolidMask(5, 140, 80, 10, 5, 55);
  }
  
  // Zufällige Größe (für interessanteres Aussehen)
  //DoCon(-Random(20)); Dann verlieren wir leider unsere SolidMask... :/
  
  // Dracheneier
  var egg_count = 1 + Random(2);
  for(var i = 0; i < egg_count; ++ i)
  {
    // Zufaellige Position, aber nicht zu nah an anderen Eiern
    var x = -GetObjWidth()/2 + 10 + Random(GetObjWidth()-20);
    var invalid = false;
    for(var j = 0; j < i; ++ j)
      if(Abs(Var(j)->GetX() - (GetX() + x)) < 20)
        invalid = true;

    if(invalid) { --i; continue; }

    Var(i) = CreateObject(DEGG, x, GetObjHeight()/2 - 2);
    Var(i)->SetLair(this() );
    Var(i)->SetR(Random(360) );

    while(Stuck(Var(i)) )
      Var(i)->SetPosition(Var(i)->GetX(), Var(i)->GetY() - 1);
  }
}

