/*--Kamikaze --*/

#strict
#include ALN2

local hurt;

/* Aktionen */

func StartLuring()
{
  if(hurt)
    return();
  inherited();
}

// Beiﬂen
private func Bite(object pPrey)
{
  // Beiﬂen!11
  DoDmg(3+Random(10)*GetCon()/100, DMG_Explosion, pPrey);
  // Beiﬂsperre, damit man nicht dauerh¸pft
  AddEffect("NoBite", this(), 99, 50, this());
  Sound("JumperBite");
  
  //alien "scharf"? :D
  if(hurt)
  {
    if(glob) //extradmg mit Glob
    {
      DoDmg(20, DMG_Bio, pPrey);
    }
    //Kaboom. :D 
    Detonate();
  }
}

// Schleim schleudern
private func GlobTo(object pPrey)
{
  return(); //kann er nicht :(
}

public func Birth()
{
  // Miniwinzig!
  SetCon(50);
}

protected func Death(int iByPlayer)
{
  // Zersplattern!
  Sparks(10, RGBa(255, 205, 25, 20));
  // Quieken!
  Sound("JumperDeath");
  GameCallEx("OnAlienDeath", this, iByPlayer);
  // Mit Knall Verschwinden!
  if(hurt)
    Detonate();
  else
    RemoveObject();
}

func CheckLiquid()
{
  if(!GBackLiquid(0,-3))
    return();
  if(GetMaterial(0,-3) == Material("Water"))
  {
    ExtractMaterialAmount(0,0, Material("Water"), GetMass());
    CastPXS("Lava",GetMass(),GetMass());
    Death(-1);
  }
}

public func OnDmg(int iDmg, int iType) {
  //Flame
  if(iType == DMG_Fire) {
    return(25);
  }
  else
    return(inherited(iDmg, iType));
}

//Treffer
func OnHit() {
  hurt = true;
  AddEffect("Hurt",this(),1,4,this(),GetID());
}

private func Detonate() {
  var r = 25;
  if(glob)
    r += 10;
  Schedule(Format("Explode(%d)",r),1);
}

func FxHurtTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  var x,y;
  if(GetAction() eq "Hangle")
    y = 2;
  if(GetAction() eq "Scale")
    x = (GetDir()*2-1) * 5;
    CreateParticle("Thrust",RandomX(-4,4)+x,RandomX(-4,4)+y,GetXDir()/2,GetYDir()/2,RandomX(100,300),RGBa(255,200,200,100),0,0);
  CreateParticle("Fire",RandomX(-2,2)+x,Random(2)+y,0,RandomX(-6,-12),RandomX(100,180),RGBa(255,RandomX(224,255),0,160));
  CreateParticle("Fire2",RandomX(-2,2)+x,Random(2)+y,0,RandomX(-8,-14),RandomX(100,180),RGBa(255,255,255,128));
}

func FxHurtEffect (string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
	if(szNewEffectName S= "Hurt") return(-1);
}
