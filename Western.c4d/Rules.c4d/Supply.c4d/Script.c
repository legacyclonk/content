/*-- Nachbestellung --*/

#strict

local pPole1,pPole2;

private func Initialize()
{
  SetAction("Initializing");
}

protected func Initialized()
{
  //Grenzmasten schon da? Sonst erzeugen
  var cGoal;
  if(cGoal = FindObject(TSCG))
  {
    pPole1 = LocalN("pStartPole", cGoal);
    pPole2 = LocalN("pEndPole", cGoal);
  }
  else
    {
    // Die Masten stehen jeweils 20 Pixel vom Kartenrand entfernt, an der obersten möglichen Position
    var Sy, Ey;
    Sy = GetPolePosition(20); // <- PolePosition, harhar..
    Ey = GetPolePosition(LandscapeWidth()-20);
    // Masten erstellen
    if(Sy > 0)
      {
      pPole1 = CreateObject(TEL1, AbsX(20), AbsY(Sy), -1);
      AddEffect("NoDmg", pPole1, 1, 0, 0, TSCG);
      pPole1->BorderPole(0);
      }
    if(Ey > 0)
      {
      pPole2 = CreateObject(TEL1, AbsX(LandscapeWidth()-20), AbsY(Ey), -1);
      AddEffect("NoDmg", pPole2, 1, 0, 0, TSCG);
      pPole2->BorderPole(1);
      }
    }
  // Alles ok
}

public func GetPolePosition(int iX)
{
  var y = -1;
  // Von oben nach unten durch die Karte wandern und schauen, ob dort Boden ist
  for(var i = 0 ; i < LandscapeHeight() ; i++)
    if(GBackSolid(AbsX(iX), AbsY(i)) && y < 0)
      y = i;
  return(y);
}

global func HomebaseProduce(iPlr)
{
  var PlayerSec=Format("Player%d", iPlr%4 +1);
  for(var i=0 ; GetScenarioVal("HomeBaseMaterial", PlayerSec, i) ; i+=2)
    DoHomebaseMaterial(iPlr, GetScenarioVal("HomeBaseMaterial", PlayerSec, i), GetScenarioVal("HomeBaseMaterial", PlayerSec, i+1));
}

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(), iPlr);
  return(1);
}
