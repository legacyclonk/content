#strict

public func Initialize() {
  SetGamma(RGB(0,0,0),RGB(68,68,83),RGB(147,147,173));
  for(var y=0; y<LandscapeHeight(); (y+=510) && (x=0))
	for(var x=0; x<LandscapeWidth(); x+=530)
		ObjectSetAction(CreateObject(_CSN,x,y,-1),Format("Wait%d",Random(4)));
}

public func InitializePlayer(iPlr) {
  SetFoW(1,iPlr);
  for(var pClnk;pClnk=FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pClnk);)
    pClnk->Initialize();
}