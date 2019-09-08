#strict

static iStep;

func Initialize() {
  AddEffect("Water",0,32,1);
  SetSkyParallax(0,16,12);
}

func InitializePlayer(iPlr) {    
  // Basismuschel suchen
  var pBase = FindObject(TRTS, GetX(GetCrew(iPlr,0)), GetY(GetCrew(iPlr,0)), -1, -1);  
  // Material erzeugen
  for (var i = 0; i < 5; ++i)
    pBase->CreateContents(CNKT);
  for (var i = 0; i < 3; ++i)
    pBase->CreateContents(LNKT);
  // Zusätzliche Clonks sollen gleich in der Muschel Schutz suchen
  for (var i = 0; i < GetCrewCount(); ++i)
    if (i > 0)
      SetCommand(GetCrew(iPlr,i), "Enter", pBase);
}

global func FxWaterTimer(pObj,iNum,iA)
{
  iA*=2;
  SetGamma(
    RGB(0,   0,   SinX(iA,20,100)),
    RGB(100, 100, CosX(iA,140,160)),
    RGB(200, 200, CosX(iA,220,255))
  );
}

global func SinX(iA,iMin,iMax) { return(Sin(iA,(iMax-iMin)/2)+(iMax+iMin)/2); }
global func CosX(iA,iMin,iMax) { return(Cos(iA,(iMax-iMin)/2)+(iMax+iMin)/2); }
