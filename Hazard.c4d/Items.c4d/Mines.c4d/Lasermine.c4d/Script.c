#strict
#include MINE

local pLaser, iLastExplode;

protected func OnActivate() {
    pLaser = CreateObject(LASR,0,0,-1);
    SetClrModulation(RGBa(255,0,0,255),pLaser);
    pLaser->SetPosition(GetX()+Sin(GetR(),3),GetY()-Cos(GetR(),3));
    pLaser->Set(GetR(),2,200,0,this(),this());
    pLaser->SetContinuous(3);
    pLaser->SetController(-1);
    // Laserminen haften und sollen bei Explosionen nicht wegfliegen
    SetCategory(C4D_Vehicle);
}

protected func OnActive() {             // Mine ist nun aktiv
    SetClrModulation(RGBa(255,255,255,100));
    SetClrModulation(RGBa(255,0,0,200),pLaser);
}

public func WeaponEnd(int &x, int &y)
{
	x = Sin(GetR(),1);
//	y = Cos(GetR(),1);
}

public func LaserStrike(object pHit) {
    // harmlos sein bis sie aktiviert ist
    if(GetAction() ne "Active") return(0);

    if(GetActTime()-iLastExplode < 10) return(0);
    
    // wenn sich das Ziel nicht bewegt, ignorieren
    if(pHit->GetXDir() == 0 && pHit->GetYDir() == 0) return(0);

    // ausrechnen in welche Richtung geschleudert wird
    var rand = 5;
    var angle = Angle(0,0,GetXDir(pHit),GetYDir(pHit))+GetR();
    while(angle >= 360) angle -= 360;
    while(angle < 0)    angle += 360;
    if(angle >= 180) rand *= -1;

    Explode(15,CreateObject(WIPF,AbsX(GetX(pHit))+Cos(GetR(),rand),AbsY(GetY(pHit))-Sin(GetR(),rand),-1));
    DoDamage(5);

    iLastExplode = GetActTime();
    return(1);
}

protected func Damage(int iChange) {            // kriegt Schaden
    if(GetAction() ne "Idle") {
      SetClrModulation(RGBa(255,255,255,200-200*GetDamage()/MaxDamage()));
      SetClrModulation(RGBa(255,0,0,220-220*GetDamage()/MaxDamage()),pLaser);
    }
    
    if(GetDamage() >= MaxDamage())
      Explode(15);
}

protected func Destruction() {
    // nicht vergessen, den Laser zu löschen
    if(pLaser) RemoveObject(pLaser);
}

private func MaxDamage() { return(30); }
private func ExploDist() { return(10); }
// Status
public func IsEquipment() { return(1); }
