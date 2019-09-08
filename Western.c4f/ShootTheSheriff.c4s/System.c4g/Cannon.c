/*-- Kanone --*/

#strict
#appendto CCAN

// Nur Pulver und Kugeln laden
public func RejectCollect(id idObj, object pObj)
{
  if(idObj != CABL && idObj != GUNP) return(1);	
}

// Eigentlich sollen nur Pferde die Kanone ziehen
public func PullCheck()
{
  // Ein Pferd
  if(FindPullingHorse()) return();
  // Bewegung und ein schiebender Clonk
  if(GetXDir() != 0)
    if(FindObject(0, 0,0,0,0, OCF_CrewMember(), "Push", this(), NoContainer()))
    {
      SetXDir(BoundBy(GetXDir(), -1,1));
    }
}
