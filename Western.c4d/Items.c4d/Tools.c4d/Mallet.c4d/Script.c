/*--- Knochenhammer --*/

#strict

protected func Hit() { Sound("WoodHit*"); }

protected func Activate(pClonk) 
{
  [$TxtCraft$]
  if(pClonk->~IsIndian()) return(ActivateIndian(pClonk));
  if(pClonk->~IsTrapper()) return(ActivateTrapper(pClonk));
  if(pClonk->~IsJungleClonk()) return(ActivateJungleClonk(pClonk));
  return(0);
}

protected func ActivateIndian(pClonk)
{
  var i, idKnowledge;
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
  CreateMenu(MLLT,pClonk,pClonk,1,"$TxtNoconstructionplans$");
  // Gebäude herstellen
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Structure); i++)
    if(idKnowledge->~IsIndianHandcraft())
      AddMenuItem("$TxtWorks$","Construct",idKnowledge,pClonk);
  // Fahrzeuge herstellen (aber nur solche, die sich als Baustelle bauen lassen)
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Vehicle); i++)
    if(idKnowledge->~IsIndianHandcraft())
      if(GetDefCoreVal("Construction", "DefCore", idKnowledge))  
        AddMenuItem("$TxtConstructions$","Construct",idKnowledge,pClonk);
  // Gegenstände herstellen
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Object); i++)
    if(idKnowledge->~IsIndianHandcraft())
      AddMenuItem("$TxtCrafts$","StartHandcraft",idKnowledge,pClonk);
  return(1);
}

protected func ActivateTrapper(pClonk)
{
  var i, idKnowledge;
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
  CreateMenu(MLLT,pClonk,pClonk,1,"$TxtNoconstructionplans$");
  // Gebäude herstellen
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Structure); i++)
    if(idKnowledge->~IsTrapperHandcraft())
      AddMenuItem("$TxtWorks$","Construct",idKnowledge,pClonk);
  // Gegenstände herstellen
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Object); i++)
    if(idKnowledge->~IsTrapperHandcraft())
      AddMenuItem("$TxtCrafts$","StartHandcraft",idKnowledge,pClonk);
  return(1);
}

protected func ActivateJungleClonk(pClonk)
{
  var i, idKnowledge;
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
  CreateMenu(MLLT,pClonk,pClonk,1,"$TxtNoconstructionplans$");
  // Gebäude herstellen
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Structure); i++)
    if(idKnowledge->~IsJungleResearch())
      AddMenuItem("$TxtWorks$","Construct",idKnowledge,pClonk);
  // Gegenstände herstellen
  for(i = 0; idKnowledge = GetPlrKnowledge(GetOwner(pClonk),0,i,C4D_Object); i++)
    if(idKnowledge->~IsJungleResearch())
      AddMenuItem("$TxtCrafts$","StartHandcraft",idKnowledge,pClonk);
  return(1);
}

// kann durch das Amulett des Gelehrten erforscht werden
public func IsJungleResearch() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann vom Trapper in Handarbeit hergestellt werden */
public func IsTrapperHandcraft() { return(1); }
