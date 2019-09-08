
#strict

private func PositionInMaterial(int mat, int x, int y, int wdt, int hgt, &ax, &ay) {
  // Standardparameter
  if(!x && !wdt) { x = 0; wdt = LandscapeWidth(); }
  if(!y && !hgt) { y = 0; hgt = LandscapeHeight(); }
  // 1000 Platzierungsversuche
  for(var i = 0; i < 1000; i++) {
    // Zufallsposition
    ax = x + Random(wdt); ay = y + Random(hgt);
    // Richtiges Material?
    if(GetMaterial(AbsX(ax), AbsY(ay)) == mat)
      return(1);
  }
  // Nicht platzierbar
  return(0);
}

private func PositionInMaterialEx(int mat, int amt, int x, int y, int wdt, int hgt, &ax, &ay) {
  // Standardparameter
  if(!x && !wdt) { x = 0; wdt = LandscapeWidth(); }
  if(!y && !hgt) { y = 0; hgt = LandscapeHeight(); }
  // 1000 Platzierungsversuche
  for(var i = 0; i < 1000; i++) {
    // Zufallsposition
    ax = x + Random(wdt); ay = y + Random(hgt);
    // Richtiges Material?
    var aax = AbsX(ax), aay = AbsY(ay);
    if(GetMaterial(aax, aay) == mat)
    if(GetMaterial(aax + amt, aay      ) == mat)
    if(GetMaterial(aax,       aay + amt) == mat)
    if(GetMaterial(aax - amt, aay      ) == mat)
    if(GetMaterial(aax,       aay - amt) == mat)
    if(GetMaterial(aax + amt, aay - amt) == mat)
    if(GetMaterial(aax + amt, aay + amt) == mat)
    if(GetMaterial(aax - amt, aay - amt) == mat)
    if(GetMaterial(aax - amt, aay + amt) == mat)
      return(1);
  }
  // Nicht platzierbar
  return(0);
}

private func SearchPositionEarth() {
  var ax, ay;
  for(var i = 1000; i >= 0; i--)
    // Position suchen
    if(PositionInMaterial(Material("Earth"), 100, LandscapeHeight() * 2 / 3, LandscapeWidth()-200, LandscapeHeight() / 3, ax, ay)) {
      // Hinversetzen
      SetPosition(ax, ay);
			return(1);
		}
  // Nicht platzierbar
  return(0);
}

private func SearchPositionInMatEx(int mat, bool fHigh) {
  var x, y, wdt, hgt;
	if(fHigh)
		{ x = 100; y = 0; wdt = LandscapeWidth()-200; hgt = LandscapeHeight(); }
	else
		{ x = 100; y = LandscapeHeight() * 2 / 3; wdt = LandscapeWidth()-200; hgt = LandscapeHeight() / 3; }		
	var ax, ay;
  for(var i = 50; i >= 0; i--)
    // Position suchen
    if(PositionInMaterialEx(mat, i / 2, x, y, wdt, hgt, ax, ay)) {
      // Hinversetzen
      SetPosition(ax, ay);
			return(1);
    }
  // Nicht platzierbar
  return(0);		
}

protected func Timer() {
  // Umgebungsfunkeln
  if(!Random(2)) {
    var iAngle = Random(360), iDist;
    for(var i = 0; i < 10; i++) iDist += Random(200); iDist = Abs(iDist - 1000) + 10;
    CreateParticle("PSpark", Cos(iAngle, iDist), Sin(iAngle, iDist), -Cos(iAngle+30, Min(iDist/10, 50)),-Sin(iAngle+30, Min(50, iDist/10)), 5*5+Random(5*5), RGBa(255,255,255,0));     
  }
}

// Weder erforsch- noch herstellbar
public func IsChemicalProduct() { }
public func IsAnvilProduct() {  }
public func GetResearchBase() { return(GetID()); }

