/*-- Kristalltal --*/

#strict

func Initialize() 
{
  SetSkyParallax(0,14,30);
  for(var i=Random(3)+1; i>0; --i)
  {
    var iX=Random(LandscapeWidth()), iY=0;
    while(1)
    {
      ++iY;
      if(GBackSemiSolid(iX, iY)) break;
    }
    if(!FindObject(MWKS, iX, iY))
      AddEffect("PotionSupply", CreateObject(MWKS, iX, iY, -1), 1, 650);
  }  
}

func InitializePlayer(iPlr) {
  var iX=Random(LandscapeWidth());
  var iY;
  while(1)
  {
    ++iY;
    if(GBackSemiSolid(iX, iY)) break;
  }
  SetPosition(iX, iY-10, GetCrew(iPlr));
  DoMagicEnergy(50, GetCrew(iPlr));
  
  Sound("PlayerJoin");
  PlacementEffects(GetCrew(iPlr)->GetX(), GetCrew(iPlr)->GetY());
  
  return(1);
}

global func FxPotionSupplyTimer(pTarget)
{
  if(!Random(50) || GetCon(pTarget) !=100) return();
	var i,p;
	for(i=0; i<ObjectCount(0,0,0,0,0,0,0,0,pTarget); ++i)
	{
		if(Contents(i, pTarget)->~IsPotion()) p++;
	}
	if(p>3) return(0);
  var rnd=Random(8);
  if(!rnd--) if(!FindContents(PFIR, pTarget)) CreateContents(PFIR, pTarget);
  if(!rnd--) if(!FindContents(PFIS, pTarget)) CreateContents(PFIS, pTarget);
  if(!rnd--) if(!FindContents(PHEA, pTarget)) CreateContents(PHEA, pTarget);
  if(!rnd--) if(!FindContents(PIMM, pTarget)) CreateContents(PIMM, pTarget);
  if(!rnd--) if(!FindContents(PMAN, pTarget)) CreateContents(PMAN, pTarget);
  if(!rnd--) if(!FindContents(PMON, pTarget)) CreateContents(PMON, pTarget);
  if(!rnd--) if(!FindContents(PSTO, pTarget)) CreateContents(PSTO, pTarget);
  if(!rnd--) if(!FindContents(PWIP, pTarget)) CreateContents(PWIP, pTarget);
  return(1);
}  

func PlacementEffects(x, y, color1, color2) 
{
  CastParticles("PxSpark", 50, 100, x, y, 5*8, 5*50, RGB(196, 196, 196), RGB(255, 255, 255));

  for (var i = 0; i < 5; i++)
    DrawParticleLine("PSpark", x-Random(400)+200,0, x,y, 8, 150 + 100 * (Random(3) == 0), RGBa(255,50+Random(100),50+Random(100),50), RGBa(50+Random(100),255,50+Random(100),100));
  
  return(1);
}
