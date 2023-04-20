/*-- Schockwelle --*/

#strict

local iR, iMax;

protected func Initialize()
{
  // Standards
  iR = 20;
  iMax = 100;
  SetAction("Exist");
}

public func SetMaxSize(int iSize)
{
  iMax = iSize;
}

protected func Grow()
{
  // Größer werden
  iR += 10;
  //Bei gewisser Größe löschen
  if(iR >= iMax)
    return(ScanAndRemove());

  // Partikelsichtbarkeit
  CreateParticle("EMPShock", 0,0,0,0, (iR + RandomX(-iR/3,iR/2)) * 5, RGB(255,255,255));

  //Ein wenig was rumcasten
  for(var i=0; i<2; ++i) {
    var pLaser = CreateObject(LASR, 0, 0, -1);
    pLaser->SetClrModulation(RGBa(28, 143, 241, 128));
    pLaser->Set(Random(360), 10, iR/2, 3);
  }

}

protected func ScanAndRemove() {
  // 4 great performance: erst zum Schluss alles absuchen
  // Nach Maschinen suchen
  for(var obj in FindObjects( Find_Distance(iR/2+10), Find_NoContainer(), Find_Func("IsMachine") ))
  {
     if(ObjectDistance(obj) < iR)
       obj->~EMPShock();
  }
  RemoveObject();
}
