/*-- Crate --*/

#strict

public func IsBulletTarget() { return(!Random(5)); }
public func IsCraneGrabable() { return(1); }
public func CanBeLaserRecycled() { return(2); }

func Damage() {
  if(GetDamage() > 20)
    {
    CastParticles("Splinter", 5, 100, 0,0, 50, 75, RGBa(255,255,255,0), RGBa(255,255,255,0));
    Sound("CrateCrack");
    while(Contents())
    	Exit(Contents(), 0, 0, Random(360), RandomX(-5,5), RandomX(-5,5), RandomX(-15,15));
    RemoveObject();
    }
}
