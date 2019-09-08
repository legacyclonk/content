/*-- Leerer Kelch --*/

#strict

local iFill, iTimer;

func FillCheck()
  {
  ++iTimer;
  if(!(iTimer%5))
    {
    var iX = Sin(GetR(), RandomX(-4,8) );
    var iY = -Cos(GetR(), RandomX(-4,4) );
    if(!Contained())
      CreateParticle("NoGravSpark", iX, iY,0,0,RandomX(25, 55),RGBa(255,255,0,120));
    iTimer=0;
    }
  if(GetMaterial() == Material("Water"))
    {
    if((iFill+=ExtractMaterialAmount(0,2, Material("Water"), 6))>=10 )
      ChangeDef(GBLT);
    }
  }

public func IsMagicProduct() { return(true); }
