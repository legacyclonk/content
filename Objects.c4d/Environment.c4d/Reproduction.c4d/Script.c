/*-- Nachwuchs --*/

#strict

protected func Initialize()
{
  // Tierbestand verzögert speichern (für Tiere, die erst im Szenarienscript erzeugt werden)
  ScheduleCall(this(), "StoreAnimals", 5);
}

public func StoreAnimals()
{
  // Tierbestand speichern
  var pAnimal;
  while (pAnimal = FindObject(0, 0, 0, 0, 0, OCF_Living(), 0, 0, 0, pAnimal))
    if (~GetOCF(pAnimal) | OCF_CrewMember())
      if (GetAlive(pAnimal))
        Add(GetID(pAnimal));  
  return(1);
}

protected func ReproductionControl()
{
  // Bestand kontrollieren
  var i = -1;
  var idAnimal;
  while (idAnimal = Get(0, ++i))
    if (ObjectCount(idAnimal) < Get(idAnimal) )
      if(!Random( 23 )) 
        Reproduce(idAnimal);
}

private func Reproduce(idAnimal)
{
  SetPosition(Random(LandscapeWidth()), Random(LandscapeHeight()));
  var pAnimal;
  while (pAnimal = FindObject(idAnimal, 0, 0, -1, -1, 0, 0, 0, 0, pAnimal))
    if (ObjectCall(pAnimal, "Reproduction", 1) || ObjectCount(idAnimal) >= Get(idAnimal)) 
      return(1);
  return(0);
}

/* ID-Listenverwaltung (Components) */       

public func Set(idc, iCount)
{
  return(SetComponent(idc, iCount));
}
  
public func Get(idc, iIndex)
{
  return(GetComponent(idc, iIndex));
}
  
public func Add(idc, iCount)
{
  return(Set(idc, Get(idc) + Max(iCount, 1)));
}
  
public func Sub(idc, iCount)
{
  return(Set(idc, Get(idc) - Max(iCount, 1)));
}
