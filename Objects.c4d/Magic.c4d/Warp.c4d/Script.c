/* Warp */

#strict

public func Activate(caster) {
  // Zaubernden Clonk finden
  var clonk = caster;
    
  if (Contained(clonk))
    if (Contained(clonk)->~IsGolem())
      clonk = Contained(clonk);

  // Und warpen.
  return(Warp(clonk));
}

private func Warp(clonk)
{ 
  // Warpposition finden
  var ox, oy;
  GetWarpPosition(ox, oy);
  
  // Keine passende Warpposition
  if(ox == -1 && oy == -1) return(0, Message("$TxtNoPlace$",clonk) );

  // Objekt wird vorsichtshalber nach draußen versetzt
//  Exit(clonk);
  
  // Die akustische Kulisse darf nicht fehlen
  Sound("Magic1");
  
  // Richtungsfaktor ermitteln
  var dir = GetDir(clonk);
  if(dir == DIR_Left() ) dir = -1;
  
  // Warplöcher erzeugen
  var startwarp = CreateObject(WARP, BoundBy(65 * dir, -GetX(), LandscapeWidth() - GetX()), 10, -1);
  var endwarp = CreateObject(WARP, AbsX(ox), AbsY(oy), -1);
  
  // Warplöcher verbinden
  startwarp->WARP::Connect(endwarp);
 
  // Fertisch
  RemoveObject();
  return(1);
}

private func GetWarpPosition(&x, &y)
{
  var i;
  while(1)
  {
    var obj = PlaceAnimal(WIPF);
    if(!obj)
      x = y = -1;
    else
      { x = GetX(obj); y = GetY(obj); }
    var n = (ObjectDistance(obj) < 100) && ((i++) < 100);
    RemoveObject(obj);
    if(!n) break;
  }
}

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("144"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
