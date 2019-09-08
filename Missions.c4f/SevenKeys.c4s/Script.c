/*-- Seven Keys --*/

#strict

static iStartPos, iStartKeysDone;

private func GetHeight(int iX) {
	for(var iY = 0; iY < LandscapeHeight(); iY++)
		if(GetMaterial(iX, iY) != -1)
			return(iY);
	return(iY);
}

protected func InitializePlayer(int iPlr) {
  // Erster Spieler?
	var fFirst = !iStartPos;
  if(fFirst)
		// Startposition suchen
		while(true)
		{
			iStartPos = 100 + Random(LandscapeWidth() - 200);
			// Bestimme Höhe und Art des Untergrunds. Sollte fest und nicht zu hoch sein.
			var iHeight = GetHeight(iStartPos);
			if(iHeight < LandscapeHeight() / 3) continue;
			if(!GBackSolid(iStartPos, iHeight)) continue;
			// Gefunden
			break;
		}
	// Clonks suchen und positionieren
	var pClonk1 = GetCrew(iPlr), pClonk2 = GetCrew(iPlr, 1);
	var iPos = iStartPos + Random(30) - 15;
	pClonk1->SetPosition(iPos, GetHeight(iPos)-5);
	iPos = iStartPos + Random(30) - 15;
	pClonk2->SetPosition(iPos, GetHeight(iPos)-5);
	// Erster Spieler: Startmaterialien (die übliche genug-für-eine-Hütte-Nummer).
	if(fFirst)
	{
		CreateObject(_FLG, iPos = iStartPos + Random(30) - 15, GetHeight(iPos), iPlr);
		CreateObject(CNKT, iPos = iStartPos + Random(30) - 15, GetHeight(iPos), -1);
		CreateObject(WOOD, iPos = iStartPos + Random(30) - 15, GetHeight(iPos), -1);
		CreateObject(WOOD, iPos = iStartPos + Random(30) - 15, GetHeight(iPos), -1);
		CreateObject(WOOD, iPos = iStartPos + Random(30) - 15, GetHeight(iPos), -1);
		CreateObject(WOOD, iPos = iStartPos + Random(30) - 15, GetHeight(iPos), -1);
  }
	// Möglichkeit, bei einem späteren Schlüssel einzusteigen? Aber nicht in der Siedelliga!
	if(FrameCounter() < 10) if (!GetLeague())
		if(GetPlrExtraData(iPlr, "SevenKeys_KeysDone") > iStartKeysDone)
		{
			iStartKeysDone = GetPlrExtraData(iPlr, "SevenKeys_KeysDone");
			// Öffne Menü für ersten Spieler
			var oClonk = GetHiRank(GetPlayerByIndex()), oWarp = FindObject(_WRP);
			CreateMenu(_WRP, oClonk, oWarp);
			for(var i = 0; i < iStartKeysDone; i++)
				AddMenuItem("$MenuSkipKey$ %s", Format("SkipKey(%d+1)", i), Local(i, oWarp), oClonk);
		}
}

protected func Initialize()
{
	// Portal platzieren
	CreateObject(_WRP, 1520, GetHeight(1520) - 50, -1);
	// Genau zwei Kristalle platzieren
	var i = 100;
    while(ObjectCount(CRYS) < 2)
	{
		var x, y;
		if(_KEY->PositionInMaterial(Material("Earth"), 0, LandscapeHeight() - i, LandscapeWidth(), i, x, y))
		    CreateObject(CRYS, x, y, NO_OWNER);
        i += 10;
	}
}
