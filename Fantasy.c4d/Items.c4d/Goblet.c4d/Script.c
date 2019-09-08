/*-- Kelch --*/

#strict

local iTimer;

func Initialize()
  {
  return(1);
  }

func FillCheck()
  {
  if(!Contained())
      if(Inside(GetR(), 20, 340))
        Hit();
    if(!(iTimer%5))
    {
    var iX = Sin(GetR(), RandomX(-4,4) );
    var iY = -Cos(GetR(), RandomX(-4,1) );
    if(!Contained())
      CreateParticle("NoGravSpark", iX, iY,0,0,RandomX(25, 55),RGBa(255,255,0,120));
    }
  iTimer++;
  if(iTimer>=17)
    {
    if(GetOCF(Contained()) & OCF_CrewMember())
      DoEnergy(+1, Contained());
    iTimer=0;
    }
  return(1);
  }

func Hit()
  {
  if(!GBackLiquid())
    {
    for(var i=15; i>0; i--)
      {
      var iX=Sin(GetR(), RandomX(4,8)), iY=-Cos(GetR(), RandomX(4,8)), iXDir=GetWind()*30/100, iYDir=-Cos(GetR()+RandomX(-2,2), Random(7));
      InsertMaterial(Material("Water"), iX, iY, iXDir, iYDir);
      }
    SetEmpty();
    }
  }

private func SetEmpty()
  {
  ChangeDef(EGBL);
  return(1);
  }
  
// Mit Kelch: Aufwertung zum Magus
public func GetRevaluationCombo() { return(MAGE); }
