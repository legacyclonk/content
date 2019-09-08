#strict
#appendto FLAG

public func Wind2Fly()
{
  var clnk;
  if(clnk=FindObject(0,-100,-100,200,200,OCF_CrewMember()))
   if(!FindObjectOwner(LI9Z,GetOwner(clnk),-10,-10,20,20)) 
    CreateObject(LI9Z,0,0,GetOwner(clnk))->LI9Z::Aufleuchten();
  SetDir(BoundBy(GetWind()/10+4,0,8));
  return(1);
}

public func Entrance()
{
  var light;
  while(light=FindObject(LI9Z,-10,-10,20,20,0,0,0,0,light))
    RemoveObject(light);
  return(1);
}

protected func Aufleuchten()
{
  SetPlrViewRange(25);
  return(1);
}
