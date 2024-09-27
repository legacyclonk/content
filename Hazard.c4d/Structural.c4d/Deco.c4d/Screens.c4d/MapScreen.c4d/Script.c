/*-- Landkartenanzeiger --*/

#strict

static const MAP_ZoomFactor = 10;

static MAP_ScreenWidth, MAP_ScreenHeight;
static MAP_MasterScreen;

local MapX, MapY, lClonk;

protected func Initialize()
{
  // Erstmal warten
  ScheduleCall(this(), "Initialized", 1);
}

protected func Initialized()
{
  // Noch kein MasterScreen gesetzt?
  if(!MAP_MasterScreen)
    {
    // Ich werde zum MasterScreen
    MAP_MasterScreen = this();
    // Landschaftsmaße ermitteln
    LandscapeWH();
    }
  // MasterScreen belegt, diesen Bildschirm initialisieren
  InitScreen();
  // Braucht eigentlich nur der MasterScreen
  MapX = MAP_ZoomFactor / 2;
  MapY = MAP_ZoomFactor / 2;
}

/* Slave-Funktionen */

// Alles was der Bildschirm nur für sich macht

private func InitScreen()
{
  // Größe anpassen
  SetShape(-MAP_ScreenWidth/2, -MAP_ScreenHeight/2, MAP_ScreenWidth, MAP_ScreenHeight);
  // Rahmen setzen
  var pFrame = AddFrame();
  pFrame->PanelColor(RGBa(1,1,1,200));
  pFrame->FrameColor(RGBa(255,255,255));
  // Clonkanzeige setzen
  ScheduleCall(this(), "ShowClonks", 1);
}

private func ScreenLandscape()
{
  // MasterScreen weg?
  if(!MAP_MasterScreen)
    MAP_MasterScreen = this();
  // Bin ich der MasterScreen?
  if(MAP_MasterScreen != this())
    return();
  // Alle vorhandenen Bildscbirme speichern
  var screens = CreateArray();
  screens = FindObjects(Find_ID(GetID()));
  // Drei Landschaftspixel scannen
  for(var j=0,mat,matC ; j<3 ; j++)
    {
    mat = GetMaterial(AbsX(MapX), AbsY(MapY));
    matC = RGB(GetMaterialColor(mat, 1, 0), GetMaterialColor(mat, 1, 1), GetMaterialColor(mat, 1, 2));
    // An alle Bildschirme verteilen
    for(var i=0 ; i < GetLength(screens) ; i++)
      screens[i]->DrawPixel((MapX-MAP_ZoomFactor/2) / MAP_ZoomFactor, (MapY-MAP_ZoomFactor/2) / MAP_ZoomFactor, matC);
    // MapX und MapY erhöhen
    MapX += MAP_ZoomFactor;
    if((MapX-MAP_ZoomFactor/2) / MAP_ZoomFactor > MAP_ScreenWidth)
      {
      MapX = MAP_ZoomFactor / 2;
      MapY += MAP_ZoomFactor;
      if((MapY-MAP_ZoomFactor/2) / MAP_ZoomFactor > MAP_ScreenHeight)
        MapY = MAP_ZoomFactor / 2;
      }
    }
}

public func DrawPixel(int iX, int iY, int Color)
{
  // Zeichnen
  SetLandscapePixel(-MAP_ScreenWidth/2 + iX, -MAP_ScreenHeight/2 + iY, Color);
}

protected func ShowClonks()
{
  // Neuen Clonk suchen
  lClonk = FindObject(0,0,0,0,0, OCF_CrewMember(),0,0,0, lClonk);
  if(lClonk)
    {
    // Clonkposition anzeigen
    var iX = GetX(lClonk) / MAP_ZoomFactor;
    var iY = GetY(lClonk) / MAP_ZoomFactor;
    CreateParticle("MapDot", -MAP_ScreenWidth/2 + iX, -MAP_ScreenHeight/2 + iY, 0, 0, 10, GetColorDw(lClonk));
    }
  // Nächsten anzeigen
  ScheduleCall(this(), "ShowClonks", 2);
}

/* Master-Funktionen */

// Führt nur der MasterScreen aus

private func LandscapeWH()
{
  MAP_ScreenWidth = LandscapeWidth() / MAP_ZoomFactor;
  MAP_ScreenHeight = LandscapeHeight() / MAP_ZoomFactor;
}
