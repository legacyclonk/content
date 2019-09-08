#strict

func Initialize() {

  for(var i=0; i<30; ++i) {

    var pScroll = PlaceInMaterial(SCRL,Material("Earth"));

    var j = 0, idDef, idMagic;
    while(idDef=GetDefinition(j++,131072))
      if(!Random(j)) idMagic=idDef;

    pScroll->SetSpell(idMagic);
  }
  return(1);
}

func InitializePlayer(int iPlr) {
  var bag = CreateObject(ALC_,10,10,-1);
  var hut = FindObjectOwner(AHUT,iPlr);
  if (!bag || !hut) return();
  bag->SetPosition(hut->GetX() - 15, hut->GetY() + 16);
  DoAlchem(IBON,2,bag);
  DoAlchem(ISPH,1,bag);
  DoAlchem(IGOL,3,bag);
  DoAlchem(ILOA,1,bag);
  DoAlchem(IMUS,4,bag);
  DoAlchem(INEC,1,bag);
  DoAlchem(IASH,3,bag);
  DoAlchem(IROC,3,bag);
}