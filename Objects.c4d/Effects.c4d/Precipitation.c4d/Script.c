#strict

local iMat, iLength, iStrength;

func Precipitation()
{
  if (Random(50) > iStrength) return(0);
  var mat = iMat;
  if (GetTemperature()<0)
    if (mat==Material("Water")) 
      mat=Material("Snow");

  var vx = GetWind(0, 3) / 10, vy = 150 - Min(GetMaterialVal("WindDrift", "Material", mat), 100)*3/2;
             
  InsertMaterial(mat, RandomX(GetX(), GetX() + iLength) % LandscapeWidth() - GetX(), -GetY(), vx, vy);
  InsertMaterial(mat, RandomX(GetX() + iLength / 3, GetX() + iLength * 2 / 3) % LandscapeWidth() - GetX(),-GetY(), vx, vy);
  InsertMaterial(mat, RandomX(GetX() - iLength / 3, GetX() + iLength * 4 / 3) % LandscapeWidth() - GetX(),-GetY(), vx, vy);
}

func Movement()
{
  SetXDir(BoundBy(GetWind(0, 3), -100, 100));
  if (GetX() > LandscapeWidth()-20) 
    SetPosition(25,-1);
  if (GetX() < 20) 
    SetPosition(LandscapeWidth()-25,-1);
}

func Activate(inMat, inLength, inStrength)
{
  SetAction("Process");
  iMat = inMat; iLength = inLength; iStrength = inStrength;
  return(1);                                      
}

protected func UpdateTransferZone()
  {
  // hack: Locals alter Spielstände aktualisieren
  if (Local(1))
    {
    iMat = Local();
    iLength = Local(1);
    iStrength = Local(2);
    }
	}
