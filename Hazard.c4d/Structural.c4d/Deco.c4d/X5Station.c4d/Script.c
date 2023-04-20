/*-- Raketenabschussbasis --*/

#strict


func Initialize() 
{
  SetAction("Be");
}

public func Init2(int iDir) {

  SetDir(iDir);

  var r = -2*(iDir==1)+1;
  var light;
  var x=-286+iDir*572, y=-187;
  var c=RGBa(255,255,255,50);

  var rocket = CreateObject(X5RK,x+r*272,171,GetOwner());
  rocket->SetAction("Clamped");
  rocket->SetDir(iDir);
  
  light = AddLightCone(1000,c,this());
  light->ChangeOffset(x+r*21,y+79);
  light->ChangeR(122*r);
  light->ChangeSizeXY(3000,6000);

  light = AddLightCone(1000,c,this());
  light->ChangeOffset(x+r*168,y+196);
  light->ChangeR(128*r);
  light->ChangeSizeXY(3000,4000);
  
  light = AddLightCone(1000,c,this());
  light->ChangeOffset(x+r*166,y+74);
  light->ChangeR(80*r);
  light->ChangeSizeXY(3000,4000);

  light = AddLightCone(1000,c,this());
  light->ChangeOffset(x+r*391,y+108);
  light->ChangeR(270*r);
  light->ChangeSizeXY(4000,3000);

  light = AddLightCone(1000,c,this());
  light->ChangeOffset(x+r*414,y+213);
  light->ChangeR(252*r);
  light->ChangeSizeXY(5000,2000);
  
  // hinter der Rakete
  //light = AddLightCone(1000,c,this());
  //light->ChangeOffset(x+r*345,y+213);
  //light->ChangeR(270*r);
  //light->ChangeSizeXY(2000,7000);
  
  
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetDir(%d)", GetDir());
}
