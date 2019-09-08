#strict
#appendto CATA

func Projectile() {
  var pProjectile=Contents();
  var iRandom = RandomX(-iPhase/4,+iPhase/4);
  var iX = (GetDir()*2-1)*12;
  var iY = -10;
  var iR = Random(360);
  var iXDir =  +6*(GetDir()*2-1)*iPhase/6+iRandom;
  var iYDir = -9*iPhase/6+iRandom;
  var iRDir = 30;
  Exit(pProjectile,iX,iY,iR,iXDir,iYDir,iRDir);
  // Sicht verfolgen wenn kein automatischer Schuss
  if (GetActionTarget(1))
    SetPlrView(GetOwner(GetActionTarget(1)), pProjectile);
}