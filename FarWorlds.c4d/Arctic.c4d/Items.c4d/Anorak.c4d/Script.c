/*-- Anorak --*/

#strict

public func Activate(pInuk) {
  [$TxtPuton$]
  if (GetID(pInuk)!=INUK) return();
  if(!(pInuk->INUK::IsUndressed())) return(Message("$TxtAlreadywearingananor$",pInuk));
  pInuk->AnorakOn();
  return(1);
}