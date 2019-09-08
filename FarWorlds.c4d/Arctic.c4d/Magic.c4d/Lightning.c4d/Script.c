#strict

public func Activate(pCaster,none,pAmulet) {
  Sound("Thunder*");
  var iX=GetX(pCaster)+GetVertex(0,0,pCaster);
  var iY=GetY(pCaster)+GetVertex(0,1,pCaster);
  var iDir=-20+25*GetDir(pCaster);
  LaunchLightning(iX,iY,iDir,15,-20,36 );
  LaunchLightning(iX,iY,iDir,15,-20,41 );
  LaunchLightning(iX,iY,iDir,15,-20,46 );
  RemoveObject();
  return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("464"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
