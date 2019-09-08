// Wind in der Höhle

#strict

global func GetWind(x,y,fGlobal)
  {
  if (!fGlobal && GBackSolid(x,y)) return(_inherited(0,0,true)/-2);
  return(_inherited(x,y,fGlobal));
  }
