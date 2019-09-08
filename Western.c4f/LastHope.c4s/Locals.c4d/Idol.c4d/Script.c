/*-- Statue --*/

#strict

protected func Hit3() {
  Sound("RockHit*");
  return(1);
}

protected func Initialize() {
  SetCategory(C4D_Vehicle());
  return(1);
}

/* Kann in einen Steingolem verwandelt werden */
public func GolemID() { return(GOLM); }
public func SliverID() { return(IDLS); }

public func MaxDamage() {
  // genau so viel wie der entsprechende Golem
  var result = DefinitionCall(GolemID(), "MaxDamage");
  // ...oder hardgecodete 90 falls keine Golems geladen sind
  if (!result) result = 90;  
  return(result);
}

local iText;

public func ActivateEntrance(pClonk)
{
  ActualizeText(pClonk);
  if(iText== 0) return(Message("$Tipp_Ghost0$", this()));
  if(iText== 1) return(Message("$Tipp_Ghost1$", this()));
  if(iText== 2) return(Message("$Tipp_Ghost2$", this()));
  if(iText== 3) return(Message("$Tipp_Ghost3$", this()));
  if(iText== 4) return(Message("$Tipp_Ghost4$", this()));
  if(iText== 5) return(Message("$Tipp_Ghost5$", this()));
  if(iText== 6) return(Message("$Tipp_Ghost6$", this()));
  if(iText== 7) return(Message("$Tipp_Ghost7$", this()));
  if(iText== 8) return(Message("$Tipp_Ghost8$", this()));
  if(iText== 9) return(Message("$Tipp_Ghost9$", this()));
  if(iText==10) return(Message("$Tipp_Ghost10$", this()));
  if(iText==11) return(Message("$Tipp_Ghost11$", this()));
  if(iText==12) return(Message("$Tipp_Ghost12$", this()));
  if(iText==13) return(Message("$Tipp_Ghost13$", this()));
  if(iText==14) return(Message("$Tipp_Ghost14$", this()));
  if(iText==15) return(Message("$Tipp_Ghost15$", this()));
}

public func ActualizeText(pClonk)
{
  if(iText==2) if(FindObject(NTIP)) iText=3;
  if(iText==3) if(FindObject(AMFH)) iText=4;
  if(iText==4) if(GetMagicEnergy(pClonk)) iText=5;
  
  if(iText==7) if(FindObject(AMBR)) iText=8;
  if(iText==8) if(PathFree(5774,500,5854,500)) iText=9;
  if(iText==9) if(FindObject(AMBI)) iText=10;
  if(iText==10) if(FindObject(AMKL)) iText=11;

  if(iText==12) if(FindObject(AMKL)) iText=13;
  if(iText==13) if(FindObject(MTIP) && FindObject(TIPI)) iText=14;
  if(iText==14) if(FindObject(SCRG)->GetClanCrew()>=6) iText=15;
}

protected func Damage() {
}
