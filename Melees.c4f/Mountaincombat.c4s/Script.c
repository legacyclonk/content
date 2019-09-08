/* Berggefecht */

#strict

static spellcount;
static spell1, spell2, spell3;

/* Initialisierung */

protected func Initialize()
{
  // Parallaxer Himmel
  SetSkyParallax(0,14,14, 0,0,SkyPar_Keep(),SkyPar_Keep());
  // Gamma
  SetGamma(RGB(15,15,15),RGB(128,128,128),RGB(215,215,215));
  // Objekte
  EarthMaterials(ROCK,15,Material("Rock"));
  EarthMaterials(ORE1,15,Material("Ore"));
  EarthMaterials(COAL,15,Material("Coal"));
  EarthMaterials(GOLD,15,Material("Gold"));
  EarthMaterials(ICE1,15,Material("Ice"));
  // ... und der Scriptcounter darf losrennen
  ScriptGo(1);
}

/* Spieler-Initialisierung */

protected func InitializePlayer(iPlr)
{
  // Zufaellige Position suchen
  var x = RandomX(50,LandscapeWidth()-50), obj, i, crew, j;
  while(FindObject2(Find_Category(C4D_Structure), Find_Distance(125, x, GetYHeight(x))) && j++ < 100)
    x = RandomX(50,LandscapeWidth()-50);
 
  obj = CreateConstruction(HUT2, x, GetYHeight(x), iPlr, 100, 1);
  while(crew = GetCrew(iPlr, i ++) )
    Enter(obj, crew);
  CreateContents(FLAG, obj);
  CreateContents(FLNT, obj);
  SetPlrMagic(iPlr, MagicSpell() );
}

//Ein Hilfsobjekt wär da besser, aber das braucht mijonen kilobyte
//Liefert zufällig einen Zauber, der noch nicht vergeben wurde
public func MagicSpell()
{
  if(!spell1) InitializeMagicSpells();
  var spellindex = Random(spellcount);
  
  while(!GlobalN(Format("spell%d",spellindex + 1)) )
  {
    ++ spellindex;
    if(spellindex >= spellcount) spellindex = 0;
  }
  
  var spell = GlobalN(Format("spell%d", spellindex + 1) );
  GlobalN(Format("spell%d", spellindex + 1) ) = 0;
  return(spell);
}

private func InitializeMagicSpells()
{
  //Zaubersprüche
  spellcount = 3;
  spell1 = CLFR;
  spell2 = FREZ;
  spell3 = MIWD;
}

/* Hilfsfunktionen */

global func EarthMaterials(id Id, int iLevel, int iMaterial)
{
  var cnt=0;
  while(cnt<iLevel) {
  var randWidth=Random(LandscapeWidth());
  var randHeight=Random(LandscapeHeight());
  if(iMaterial !=-1 && GetMaterial(randWidth,randHeight)==iMaterial) {
    cnt++;
    var obj=CreateObject(Id,randWidth,randHeight+GetDefCoreVal("Height","DefCore",Id)/2);
    SetR(Random(360),obj);
    }
  if(iMaterial==-1 && GBackSolid(randWidth,randHeight)) {
    cnt++;
    var obj=CreateObject(Id,iLevel,iMaterial+GetDefCoreVal("Height","DefCore",Id)/2);
    SetR(Random(360),obj);
    }
  }
return(1);
}

func GetYHeight(int ix)
{
  var cnt=0;
  while(!GBackSolid(ix,cnt) && cnt<=LandscapeWidth())
    cnt++;
 return(cnt);
}

/* Maximale Rejoinmenge */
public func MaxRejoin(int iPlayer)
{
  return(3);
}
