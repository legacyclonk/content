/*-- Bett --*/

#strict

/* Initialisierung */
protected func Initialize()
{
  UpdateState();
}

private func IsOccupied()
{
  if (FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, this()))
    return(1);
  return(0);
}

private func UpdateState()
{
  if (IsOccupied())
  {
    if(GetAction() ne "Occupied")
      return(SetAction("Occupied"));
  }
  else   
  {
    if(GetAction() S= "Occupied")
      return(SetAction("Empty") );
  }
}

protected func ActivateEntrance(object pClonk)
{
  // Eingangsanfrage
  if(!Contained(pClonk) )
    if(GetOCF(pClonk) & OCF_CrewMember())
      return(OnEntrance(pClonk));
  // Ausgangsanfrage
  if(Contained(pClonk) == this() )
    return(OnExit(pClonk));
  return(0); 
}

private func OnEntrance(object pClonk)
{
  // Schon besetzt
  if (IsOccupied()) return(CheckChallenge(pClonk));
  // Betreten
  Enter(this(),pClonk);
  UpdateState();
  return(1);
}

private func OnExit(object pClonk)
{
  Exit(pClonk,0,+4);
  UpdateState();
}
  
private func CheckChallenge(object pClonk)
{
  // Ein feindlicher Clonk von auﬂen fordert den Insassen
  var pEnemy = FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this());
  if(pEnemy)
    if(Hostile(GetOwner(pClonk), GetOwner(pEnemy)) )
      OnExit(pEnemy);
  return(0);
}

/* Bett l‰dt Energie auf */  
  
private func RefillEnergy()
{
  var clonk = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, this());
  if(clonk) DoEnergy(1, clonk);
}

