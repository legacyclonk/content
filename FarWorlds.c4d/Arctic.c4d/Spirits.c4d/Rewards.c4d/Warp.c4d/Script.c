/*-- Lokator --*/

#strict

/* Translokation */

public func Activate(pClonk) {
  [$TxtTranslocate$]
  // Clonk anhalten
  SetComDir(COMD_Stop(),pClonk);
  // Nur wenn der Clonk steht und sich im Freien befindet
  if(GetAction(Contained())ne"Walk") return(0);
  if(Contained(Contained())) return(0);
  // Menü erzeugen
  CreateMenu(RLKT,pClonk,this());
  AddMenuItem("$TxtTranslocate$","WarpIgloo",_WRI,pClonk,0,pClonk);
  AddMenuItem("$TxtRandomwarp$","WarpRandom",MGWP,pClonk,0,pClonk);
  return(1);
}

/* Warp */

func WarpIgloo(idItem,pClonk) {
  // einmal nach hause bitte
  var home = FindObjectOwner(IGLO,GetOwner(pClonk),0,0,-1,-1);

  // Sound und so.
  Sound("Magic1");

  // Richtungsfaktor ermitteln
  var dir = GetDir(pClonk);
  if(dir == DIR_Left() ) dir = -1;

  // Löcher erzeugen
  var startwarp = CreateObject(WARP, BoundBy(65 * dir, -GetX(), LandscapeWidth() - GetX()), 10, -1);
  var endwarp = CreateObject(WARP, AbsX(GetX(home)), AbsY(GetY(home)), -1);

  // Löcher verbinden
  startwarp->WARP::Connect(endwarp, home);

  // Fertich
  return(RemoveObject());
}

func WarpRandom(idItem,pClonk) {
  // den Originalwarp missbrauchen
  var pWarp = CreateObject(MGWP,0,0,-1);
  if(pWarp->MGWP::Activate(pClonk))
    return(RemoveObject());
}