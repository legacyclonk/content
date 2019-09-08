#strict

public func PlaceClonks(x,y,wdt,hgt,plr) {
	var tree=PlaceVegetation(PLM2,x,y,wdt,hgt,1);

  	for(var clnk;clnk=FindObjectOwner(0,plr,0,0,0,0,OCF_CrewMember(),0,0,clnk);)
	  SetPosition(GetX(tree),GetY(tree)-14,clnk);

	CreateContents(FLAG,GetCrew(plr,0));
  CreateContents(CNKT,GetCrew(plr,1));
  return(1);
}

public func InitializePlayer(iPlr) {
  SetFoW(1,iPlr);
  for(var pClnk;pClnk=FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pClnk);)
    pClnk->Initialize();
  return(PlaceClonks(0,450,900,480,iPlr));
}

public func Initialize() {
  SetGamma(RGB(3,1,0),RGB(95,83,68),RGB(213,212,187));
  SetSkyParallax (0,33,57,0,0,0,0); 
  for(var y=0; y<LandscapeHeight(); (y+=200) && (x=0))
	for(var x=0; x<LandscapeWidth(); x+=180)
		if(GetMaterial(x,y)==-1) CreateObject(SU9Z,x,y,-1);
}

// Vulkan soll DuroLava verwenden
public func VolcanoMaterial() { return(Material("DuroLava")); }
