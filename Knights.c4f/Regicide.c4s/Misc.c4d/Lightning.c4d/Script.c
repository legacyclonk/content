/*-- Blitz-Zauber --*/

#strict

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj, fBlast;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this())) return(1);

  var obj = CreateObject(_LGT, 0, 0, GetOwner(pClonk));
  obj->Launch(pClonk, GetDir(pClonk), GetX(pClonk), GetY(pClonk), 0);
  Sound("Thunder*");
  RemoveObject();
  return(1);
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  var obj = CreateObject(_LGT, 0, 0, GetOwner(pCaller));
  obj->Launch(pCaller, GetDir(pCaller), GetX(pCaller), GetY(pCaller), (900-Abs(iAngle*10)));
  Sound("Thunder*");

  // Objekt löschen
  RemoveObject();
  return(1);
  }
/*
public func Activate(pClonk)
{
  Sound("Thunder*");
  var x = GetX(pClonk) + GetVertex(0, 0, pClonk);
  var y = GetY(pClonk) + GetVertex(0, 1, pClonk);
  var xstep = -20 + 25 * GetDir(pClonk);
  var xrange = 15;
  LaunchLightning(x, y, xstep, xrange, -20, 41);
  RemoveObject();
  return(1);
}*/

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("46"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
