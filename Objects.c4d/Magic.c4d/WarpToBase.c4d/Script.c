/* Warp */

#strict

public func Activate(caster, real_caster) {
  // Zaubernden Clonk finden
  var clonk = caster;
  if (real_caster) clonk = real_caster;

  // Und warpen.
  return(Warp(clonk));
}

private func Warp(clonk)
{
  // Basis?
  var home;
  if(!(home = FindBase(GetOwner(), 0) )) return(0, Message("$TxtNobase$",clonk) );
  
  // Objekt wird vorsichtshalber nach draußen versetzt
//  Exit(clonk);
  
  // Sound und so.
  Sound("Magic1");
  
  // Richtungsfaktor ermitteln
  var dir = GetDir(clonk);
  if(dir == DIR_Left() ) dir = -1;

  // Löcher erzeugen
  var startwarp = CreateObject(WARP, BoundBy(65 * dir, -GetX(), LandscapeWidth() - GetX()), 10, -1);
  var endwarp = CreateObject(WARP, AbsX(GetX(home)), AbsY(GetY(home)), -1);
  
  // Löcher verbinden
  startwarp->WARP::Connect(endwarp, home);
 
  // Fertisch
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("111"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

public func GetResearchBase () { return (MGWP); }
