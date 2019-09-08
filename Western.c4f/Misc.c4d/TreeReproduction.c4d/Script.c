/*-- Baumbestand --*/

#strict

#appendto TREE

private func Seed()
{
 // Nicht automatisch fortpflanzen
}

protected func Initialize()
{
  if(GetID()!=TREP) return(_inherited());
  // Baumbestand speichern
  var pTree;
  while (pTree = FindObject(0, 0, 0, 0, 0, OCF_Chop(), 0, 0, 0, pTree))
    if (pTree->~IsTree())
      if (pTree->~IsStanding())
        Add(GetID(pTree));  
  return(1);    
}

protected func ReproductionControl()
{
  // Bestand kontrollieren
  var i = -1;
  var idTree;
  while (idTree = Get(0, ++i))
    if (ObjectCount(idTree) < Get(idTree) )
      Reproduce(idTree);
}

private func Reproduce(idTree)
{
  if(GetID()!=TREP) return(_inherited(idTree));
  SetPosition(Random(LandscapeWidth()), Random(LandscapeHeight()));
  var pTree;
  while (pTree = FindObject(idTree, 0, 0, -1, -1, 0, 0, 0, 0, pTree))
    if (ObjectCall(pTree, "Reproduction") || ObjectCount(idTree) >= Get(idTree)) 
      return(1);
  return(0);
}

/* ID-Listenverwaltung (Components) */       

public func Set(idType, iCount)
{
  return(SetComponent(idType, iCount));
}
  
public func Get(idType, iIndex)
{
  return(GetComponent(idType, iIndex));
}
  
public func Add(idType, iCount)
{
  return(Set(idType, Get(idType) + Max(iCount, 1)));
}
  
public func Sub(idType, iCount)
{
  return(Set(idType, Get(idType) - Max(iCount, 1)));
}
