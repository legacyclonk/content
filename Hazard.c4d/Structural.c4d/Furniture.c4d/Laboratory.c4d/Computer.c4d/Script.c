/*-- Computer --*/

#strict

func Initialize() 
{
  SetAction("Blink");
}

public func Spark()
{
  if(!Random(4))
  {
    var pLaser = CreateObject(LASR, -13,-20, GetOwner());
    pLaser->Set(RandomX(75, 85), 15, 26, 8, pLaser, this());
    pLaser->SetClrModulation( RGB(114,194,255));
  }
  if(!Random(4))
  {
    var pLaser = CreateObject(LASR, 14,-28, GetOwner());
    pLaser->Set(RandomX(75, 85) + 180, 15, 26, 8, pLaser, this());
    pLaser->SetClrModulation( RGB(114,194,255));
  }
}
