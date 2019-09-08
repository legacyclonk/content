/*-- Landmine --*/

#strict 2
#include TRAP

protected func Check()
{
  // Nach geeigneten Lebewesen suchen
  var pLiving = FindObject2(Sort_Distance(),Find_AtPoint(),Find_Or(Find_OCF(OCF_Alive),Find_Func("IsHorse")),Find_NoContainer());
  if(pLiving) return(Trap(pLiving));
}

private func Trap(object pToTrap)
{
  if(Contained()) return(Explode(32));
  Explode(32);
  var iOwner = GetOwner();
  var pObj, iCount, iR, iXDir, iSpeed;
  for(iCount = RandomX(20,30); iCount > 0; iCount--) {
    iR = Random(271)+45;
    iSpeed = RandomX(30,75);
    pObj = CreateObject(BALP,Sin(iR,6),Cos(iR,6),iOwner);
    iXDir = Sin(iR,iSpeed);
    SetXDir(iXDir+RandomX(-3,3),pObj);
    SetYDir(Cos(iR,iSpeed)-20+RandomX(-3,3),pObj);
    SetRDir((10+Random(21))*iXDir/Abs(iXDir),pObj);
  }
  return(1);
}

protected func Incineration() { return(Trap()); }

/* Kann auch von Kavalleristen bedient werden */
public func CanArm(pClonk) { return(pClonk->~IsCavalryman() || pClonk->~IsTrapper()); }

/* Kann in der Waffenschmiede hergestellt werden */
public func IsArmoryProduct() { return(1); }

/* Ist zwar eine Falle, kann aber nicht vom Trapper hergestellt werden */
public func IsTrap() { return(this() != 0); }