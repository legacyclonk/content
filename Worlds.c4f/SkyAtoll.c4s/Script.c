#strict

func Initialize() {

  SetSkyParallax (0,17,19,0,0,0,0); 
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));

  //Nebel
  for(var i=0;i<180;++i) 
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()),0,0,RandomX(900,1700));
}

func InitializePlayer(iPlr) {
  var pClnk;

  SetFoW(1,iPlr);
  for(var pClnk;pClnk=FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pClnk);)
    pClnk->Initialize();

  //Clonks werden unten rechts am Bildschirmrand erzeugt
  while(pClnk=FindObjectOwner(0,iPlr,0,0,0,0,OCF_CrewMember(),0,0,pClnk))
	SetPosition(LandscapeWidth()-55,LandscapeHeight()-112,pClnk);

  //Jeder Spieler bekommt eine Flagge und einen Bausatz wenn das Ritterpack nicht aktiviert ist
  //ansonsten kriegt er statt dem Bausatz ein Zeltpack und Baupläne für Waffen und Burg
  CreateContents(FLAG,GetCrew(iPlr,0));
  if(!CreateContents(TENP,GetCrew(iPlr,1)))
    CreateContents(CNKT,GetCrew(iPlr,1));
  if(GetCrew(iPlr,2)) {
    CreateContents(CPPL,GetCrew(iPlr,2));
    CreateContents(WPPL,GetCrew(iPlr,2));
  }

  //...außerdem ein Luftschiff
  if(!FindObject(BLMP,LandscapeWidth()-80,LandscapeHeight()-130,60,60))
    CreateObject(BLMP,LandscapeWidth()-50,LandscapeHeight()-100,-1);
}