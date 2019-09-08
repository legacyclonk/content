/*--Amulett--*/

#strict

protected func Hit() { Sound("Crystal*"); }

public func LightningStrike() {
  // Sound
  Sound("Electric");
  return(1);
}

protected func Life(idBla,pClonk) {
  // Nur wenn die Bevölkerung groß genug ist
  if(GetCrewCount(GetOwner(pClonk))<3)  
    return(Message("$TxtErrLife$",this()),Sound("Error"));
  ChangeDef(AML1);
  Sound("Magic1");
  SetPhysical("Magic",100000,PHYS_Temporary,Contained());
  // Aktion setzen
  ObjectSetAction(pClonk,"Magic");
  SetAction("DoEnergy");
}

protected func Death(idBla,pClonk) {
  // Nur bei Nacht oder Dämmerung möglich 
  if (!Inside(GetSeason(),40,90)) 
     return(Message("$TxtErrDeath$",this()),Sound("Error"));
  ChangeDef(AMD1);
  Sound("Magic1");
  SetPhysical("Magic",100000,PHYS_Temporary,Contained());
  ObjectSetAction(pClonk,"Magic");
}

protected func Mystic(idBla,pClonk) {
  ChangeDef(AMM1);
  Sound("Magic1");
  SetPhysical("Magic",100000,PHYS_Temporary,Contained());
  ObjectSetAction(pClonk,"Magic");
}

public func Activate(pClonk) {
  [$TxtSelectCircle$]
  // Menü öffnen
  CreateMenu(AMUL,pClonk,this(),1,"$TxtSelectCircle$");
  AddMenuItem("$TxtSelectCircle$: $TxtLife$",  "Life",  AML1,pClonk,0,pClonk);
  AddMenuItem("$TxtSelectCircle$: $TxtDeath$", "Death", AMD1,pClonk,0,pClonk);
  AddMenuItem("$TxtSelectCircle$: $TxtMystic$","Mystic",AMM1,pClonk,0,pClonk);
  return(1);
}