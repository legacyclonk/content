/*-- Fisch --*/

#strict
#include FISH

// Bait-Verhalten anders
// Zieht Energie beim Essen ab
// Verwandelt sich bei Tot natürlich in toten schwarzen Fisch

/* TimerCall */

private func Activity() {
  var pBait;
  // Ein Köder in der Nähe?
  if (pBait=FindObject(0,-1000,-1250,2000,1500, OCF_InLiquid(), "Bait" ) )
    // Schwarze Fische sind nicht wählerisch
    SetCommand(this(),"Follow",pBait);

  // Schwimmverhalten
  if (!GBackLiquid (0, -8)) return (SetComDir (COMD_Down ()));
  if (Random(2)) return(1);
  if (SEqual(GetAction(),"Walk")) WalkDir();
  if (Not( SEqual(GetAction(),"Swim") )) return(1);
  if (Random(2)) return(TurnRight(),SetComDir(COMD_Right()));
  return(TurnLeft(),SetComDir(COMD_Left()));
}

/* Aktionen von außerhalb */

public func Eat(pClonk) {
  pClonk->~Feed(-50);
  RemoveObject();
}

/* Aktionen */

protected func Death() {
  ChangeDef(QDFH);
  SetComponent(MEAT,0);
  SetComponent(FAT1,0);
  SetComponent(FSHB,0);
  SetComponent(DFLM,3);
  SetAction("Dead");
}