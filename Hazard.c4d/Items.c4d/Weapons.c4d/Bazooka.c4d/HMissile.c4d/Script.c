/* Rakete */

#strict
#include MISS

local pTarget;

public func Acceleration() { return(4); }
public func MaxTime() { return(70); }
public func MaxSpeed() { return(160); }

public func HomingBegin() { return(15); }
public func MaxHomingDist() { return(400); }
public func MaxHomingAngle() { return(60); }

/* Timer */

private func Traveling()
{
  // kommt nich weiter
  if(GetActTime() >= MaxTime()) return(Hit());

  // Effekte
  Smoking();

  // beschleunigen
  Accelerate();

  // nur lenken wenn schon etwas fliegend
  if(GetActTime() < HomingBegin()) return();

  // lenken
  if(pTarget) FollowTarget();

  if(!pTarget) 
    if(pTarget = FindTarget()) {
       // Effekt bei Zielfindung
       Sound("BipBipBip");
    }
}

private func FindTarget() {

	var targets = FindTargets(this,MaxHomingDist(),MaxHomingAngle(),GetR(),false);
	var target;
  
	// teuerstes Objekt anivisieren
	for(var t in targets) {
		if(!target) {
			target = t;
			continue;
		}
		if(t->GetValue() > target->GetValue()) {
			target = t;
			break;
		}
	}

	return(target);
}


private func FollowTarget() {

    var iDAngle = Angle(GetX(),GetY(),GetX(pTarget),GetY(pTarget));
    var iAngle = GetR();
    
    var iDiff = Normalize(iDAngle - iAngle,-180);
    var iTurn = Min(Abs(iDiff),6);

    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}

public func NoDecoDamage() { return(true); }
