/* Rennen */

#strict

// WICHTIG: damit man das Rennen auf Liga spielen kann
// muss Wettstreit(RVLR) aktiviert sein

func Initialize()
{
  DrawIslands(0,353,415,369);
  DrawIslands(0,722,559,651);
  for(var i=0; i < 60; i++)
    PlaceVegetation(TRE1, 0, 0, LandscapeWidth(), LandscapeHeight(), -1);
  for(var i=0; i < 60; i++)
    PlaceVegetation(TRE2, 0, 0, LandscapeWidth(), LandscapeHeight(), -1);
  for(var i=0; i < 60; i++)
    PlaceVegetation(TRE3, 0, 0, LandscapeWidth(), LandscapeHeight(), -1);
  DoPlaceObjects(LOAM,60,"Earth");
  DoPlaceObjects(CNKT,20,"Earth");
  DoPlaceObjects(WOOD,50,"Earth");
  DoPlaceObjects(ROCK,60,"Earth");
  DoPlaceObjects(FLNT,60,"Earth");
  DoPlaceObjects(METL,50,"Earth");
  DoPlaceAnimal(BIRD, 14);
  return();
}

global func DoPlaceObjects(idID, iAmount, szMat)
{
  var obj;
  for(var i=0; i<iAmount; i++)
  {
    obj = PlaceInMaterial(idID, Material(szMat));
    if(obj) CreateObject(idID, LandscapeWidth()-GetX(obj), GetY(obj));
  }
}

global func DoPlaceAnimal(idID, iAmount)
{
  var obj;
  for(var i=0; i<iAmount; i++)
  {
    obj = PlaceAnimal(idID);
    if(obj) CreateObject(idID, LandscapeWidth()-GetX(obj), GetY(obj));
  }
}

global func DrawIslands(iX, iY, iWidth, iHeight)
{
 DrawMap(iX, iY, iWidth, iHeight, "map new { overlay { algo=bozo; turbulence=100; loosebounds=1; a=10; } & overlay { algo=rndchecker; turbulence=1000; a=1; mat=Earth; tex=Rough; overlay { algo=random; mat=Earth; tex=Smooth; }; overlay InEarth { grp=1; invert=1;overlay { algo=bozo; a=6; turbulence=1000; } & overlay { algo=rndchecker; a=2;turbulence=1000; mat=Rock; tex=Rough; };}; InEarth & overlay { mat=Gold; tex=Rough; }; InEarth & overlay { mat=Coal; tex=Swirl; };};overlay { algo=poly;point { x=0px; y=0px; };point { x=1px; y=0px; };point { x=1px; y=100%; };point { x=0px; y=100%; };}; overlay { algo=poly;point { x=99%; y=0px; };point { x=100%; y=0px; };point { x=100%; y=100%; };point { x=99%; y=100%; };}; overlay { algo=poly;point { x=0%; y=0px; };point { x=100%; y=0px; };point { x=100%; y=1%; };point { x=0%; y=1%; };};overlay { algo=poly;point { x=0%; y=99%; };point { x=100%; y=99%; };point { x=100%; y=100%; };point { x=0%; y=100%; };}; };");
 for(var x=0; x < iWidth; x++)
   for(var y=0; y < iHeight; y++)
     DrawMaterialQuad (MaterialName(GetMaterial(x+iX, y+iY)), LandscapeWidth()-x-iX, y+iY, LandscapeWidth()-x-iX, y+iY+1, LandscapeWidth()-x-iX+1, y+iY+1, LandscapeWidth()-x-iX+1, y+iY);
}

// -- Callbacks des Rennen-Spielziels --
// wenn diese Funktionen nicht im Szenarioscript sind
// oder 0 zurück geben, wird der Default-Wert verwendet

// Richtung:
// 1: links -> rechts
// 2: rechts -> links
// 3: untern -> oben
// 4: oben -> unten
// default: links -> rechts
func GetRACEDirection() { return(3); }

// Start: Anzahl an Pixeln, ab dem Rand, von dort beginnt die Zählung
// default: 50 px
func GetRACEStartOffset() { return(190); }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Zählung
// default: 50 px
func GetRACEEndOffset() { return(170); }

// Gewinnbedingung
//  1: Spieler hat gewonnen
//  0: gewinnen erlaubt wenn am Rand
// -1: kein gewinnen mögliche
func CheckRACEGoal(iPlr)
{
  // In diesem Fall müssen die Spieler das Nest erreichen
  if(FindObject2(Find_ID(DLAR), Find_Distance(50, GetX(GetCursor(iPlr)), GetY(GetCursor(iPlr)))))
      return(1); // Dann hat der Spieler gewonnen
  // Sonst muss er noch weiterspielen
  return(-1);
}

// Spielerbeitritt
func InitializePlayer(iPlr)
{
  // Anwählen und in Position bringen
  SetCursor(iPlr,GetHiRank(iPlr));
  SelectCrew(iPlr, GetHiRank(iPlr), 1);
  if(GetPlayerTeam(iPlr)==1)
    SetPosition(10,1609, GetHiRank(iPlr));
  else
    SetPosition(LandscapeWidth()-10,1609, GetHiRank(iPlr));
}

// Relaunch
protected func RelaunchPlayer(int iPlr)
{
  // Verkünden
  Log("$MsgRestart$", GetPlayerName(iPlr));

  // Ein neuer Clonk für den Spieler mit voller Energie
  var pClonk = CreateObject(KNIG, 10, 10, iPlr);
  MakeCrewMember(pClonk, iPlr);
  DoEnergy(100, pClonk);

  // Anwählen und in Position bringen
  SetCursor(iPlr,pClonk);
  SelectCrew(iPlr, pClonk, 1);
  if(GetPlayerTeam(iPlr)==1)
    SetPosition(10,1609, pClonk);
  else
    SetPosition(LandscapeWidth()-10,1609, pClonk);
}
