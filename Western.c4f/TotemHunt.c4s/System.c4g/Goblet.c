/*-- Kelch --*/

#strict 2
#appendto GBLT

// Heliung verdreifacht
public func FillCheck()
{
  iTimer++;
  if(!Contained()) {
    if(Inside(GetR(), 20, 340))
      Hit();
    if(!(iTimer%5)) {
      var iX = Sin(GetR(), RandomX(-4,4) );
      var iY = -Cos(GetR(), RandomX(-4,1) );
      CreateParticle("NoGravSpark", iX, iY,0,0,RandomX(25, 55),RGBa(255,255,0,120));
    }
    return(1);
  }
  if(iTimer >= 25) {
    if(GetOCF(Contained()) & OCF_CrewMember)
      DoEnergy(+1, Contained());
    iTimer=0;
  }
  return(1);
}