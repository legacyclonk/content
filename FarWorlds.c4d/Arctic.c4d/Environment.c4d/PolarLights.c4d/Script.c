/*-- Nordlichter --*/

#strict

protected func Initialize() {
  SetAction("NorthernLights");
  SetPosition(50,70);
}
  
private func NorthernLights() {    
  // Nordlichter
  if (IsNight()) 
    if (!Random(10)) 
      PlaceNorthernLight();
}
 
private func PlaceNorthernLight() {
  var iRand;
  CreateObject(NTLT,iRand=Random(LandscapeWidth()),Random(GetHorizonHeight(iRand)-40),-1);
}
  
private func GetHorizonHeight(iX) {
  var i=0;
  while(i<LandscapeHeight() && !GBackSemiSolid(iX,i)) i+=10;
  return(i);
}