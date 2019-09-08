#strict

protected func Activate(object pCaster, object pRealcaster)
{
  var iX = (GetDir(pCaster) * 2 - 1) * RandomX(100, 120);
  var iY = RandomX(100, 200);
  var iDY = 0;
  var thickness = RandomX(16, 28);
  var mat = "Lava";
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("VolcanoNSpell", 0, 150)) return(iResult != -1 && RemoveObject());
  // in der Luft soll kein Vulkan erzeugt werden
  while (!GBackSolid(iX,iDY) && iDY+GetY() <= LandscapeHeight()) iDY+=5;
  // wenn das Landstück unter der Stelle wo der Vulkan
  // erzeugt werden soll nicht hoch genug ist (100px), wie vorher mit Y = LandscapeHeight() zaubern
  if (!GBackSolid(iX,iY+iDY))
    if (!GBackSolid(iX,(iY=100)+iDY))
    {
      iY = AbsY(LandscapeHeight());
      iDY= 0;
    }
    
/*
  // ANDERE MAT-KOMBINATIONEN DEAKTIVIERT

  // Zaubernden Clonk ermitteln
  var clonk = pCaster;
  if (pRealcaster) clonk = pRealcaster;
    
  // Kombinieren?
  var obj;
  if (obj = FindContents(ABRL, clonk)) {
    mat = "Acid"; 
    thickness = RandomX(12, 17);
  } else if (obj = FindContents(OBRL, clonk)) {
    mat = "Oil";
    thickness = RandomX(8, 10);
  } else if (obj = FindContents(WBRL, clonk)) {
    mat = "Water";
  }
  RemoveObject(obj);

*/

  
  // Vulkan ausbrechen lassen
  LaunchVolcano(BoundBy(GetX() + iX,0,LandscapeWidth()),BoundBy(GetY() + iY + iDY,0,LandscapeHeight()), thickness, mat);
  Sound("Lava*");
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("322"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
