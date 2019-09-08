/*-- Sonnenlicht --*/

#strict

public func Check4Clonk()
{
  var clnk;
     if(clnk=FindObject(0,-200,-200,400,400,OCF_CrewMember()))
       if(!FindObjectOwner(LI9Z,GetOwner(clnk),-10,-10,20,20))
         CreateObject(LI9Z,0,0,GetOwner(clnk))->LI9Z::Aufleuchten();
  return(1);
}
