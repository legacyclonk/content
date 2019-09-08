/*-- Bunte Fische --*/

#strict

protected func Initialize() 
{
  // Alle Fische anfangs umfärben
  var pFish;
  while(pFish=FindObject(FISH,0,0,0,0,0,0,0,0,pFish))
    Colorize(pFish);
}

public func Colorize(pObj) 
{
	SetColorDw(HSL(Random(256),255,100+Random(60)),pObj);
}
