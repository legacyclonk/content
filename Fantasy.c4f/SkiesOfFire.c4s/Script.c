/*-- Feurige Himmel --*/

#strict

func Initialize() 
{
  SpreadDragons();
//  AddEffect("IntLightning", 0, 1, 100);
}

func InitializePlayer(int player)
{
  var clonk = GetCrew(player);
  clonk->DoMagicEnergy(clonk->GetPhysical("Magic")/2000);
}

func SpreadDragons()
{
  // Verteilt die Drachen gleichmäßig in der gesamten Landschaft,
  // nimmt an, dass am oberen Rand genug Freiraum ist
  var pDragon;
  while (pDragon = FindObject(DRGN,0,0,0,0,0,0,0,0,pDragon))
  {
    pDragon->SetPosition(128 + Random(LandscapeWidth() - 256), 128 + Random(LandscapeHeight() - 256));
    while (pDragon->Stuck() && (pDragon->GetY() > 128))
      pDragon->SetPosition(pDragon->GetX(), pDragon->GetY() - 1);
  }
}

/*global func FxIntLightningTimer()
{
  var x = Random(LandscapeWidth() ), y = Random(LandscapeHeight() );
  while(GBackSemiSolid(x, y) )
  {
    x = Random(LandscapeWidth() );
    y = Random(LandscapeHeight() );
  }
 
  LaunchLightning(x, y, RandomX(10, 20), Random(5), RandomX(10, 20), Random(5) );
}*/

