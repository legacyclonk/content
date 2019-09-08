/*--- Knochenhammer --*/

#strict

protected func Hit() { Sound("WoodHit*"); }

protected func Activate(pClonk) 
{
  [$TxtCraft$]
  var idKnowledge;
  // Clonk anhalten
  SetComDir(COMD_Stop(), pClonk);
  CreateMenu(MLLT,pClonk,pClonk,1);
  // Gebäude herstellen
  for(var i=0;idKnowledge=GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Structure());++i)
    AddMenuItem("$TxtConstructions$","Construct",idKnowledge,pClonk);
  // Gegenstände herstellen
  for(var i=0;idKnowledge=GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Object());++i)
    AddMenuItem("$TxtCrafts$","StartHandcraft",idKnowledge,pClonk);
  return(1);
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }