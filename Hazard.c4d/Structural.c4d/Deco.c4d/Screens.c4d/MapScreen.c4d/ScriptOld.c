/*-- Landkartenanzeiger --*/

#strict

static const MAP_ZoomFactor = 10;

static MAP_ScreenData;

func Initialize() {
/*
  //so, also, erstmal schaun wie wir das mit dem Masterscreen machen..
  if(!masterscreen) { //evtl. so? nein.
dooof!    masterscreen = this(); //Ich bin der Master, harhar
hm, doch nicht so.    return(SheduleCall(0,"Initialize",1));  //und das ganze Initializezeugs um 1 Frame verzögern
    return(InitMasterscreen()); //aber so :D doch nicht..
  }
*/

//jetzt aber.
  if(!MAP_ScreenData)
    InitScreens();
}

private func InitScreens() {
  var data;
  
  //Auskommentieren, damit man z.b. das ganze neu machen kann. (oder so)  
//  if(MAP_ScreenData)
//    return(0);
  
  data = AddEffect("MapData",0,1);
  if(!data)
    return(Log("PANICK"));
    
  for(var z=0; z < LandscapeWidth()/MAP_ZoomFactor; z++)
    EffectCall(0,data,"AddData",AddEffect("MapData",0,1));

  for(var x=0; x < LandscapeWidth()/MAP_ZoomFactor; x++)  
    for(var y=0; y < LandscapeHeight()/MAP_ZoomFactor; y++) {
      var r,g,b;
      var mat=GetMaterial(x*MAP_ZoomFactor,y*MAP_ZoomFactor);
      r=GetMaterialVal("Color", 0, mat, 0);
      g=GetMaterialVal("Color", 0, mat, 1);
      b=GetMaterialVal("Color", 0, mat, 2);
      EffectCall(0,EffectVar(x,0,data),"AddData",RGB(r,g,b));
    }
  MAP_ScreenData = data;
  
}

func Resize(int width, int height) {
  SetShape(-width/2,-height/2,width,height);
}


/* Speichereffekt */

global func FxMapDataAddData(object pTarget, int iEffectNr, data) {
  EffectVar(++EffectVar(0,pTarget, iEffectNr),pTarget,iEffectNr) = data;
}

global func FxMapDataGetData(object pTarget, int iEffectNr, data) {
  return(EffectVar(data-1,pTarget,iEffectNr));
}

/* auch doof.
global func FxMapDataEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, data) {
  Log("%s",szNewEffectName);
  if(szNewEffectName S= "MapData")
    return(-3);
}

global func FxMaxDataAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, data) {
  Log("%s geaddet", SzNewEffectName);
  EffectVar(0, pTarget, iEffectNumber)++;
  EffectVar(EffectVar(0, pTarget, iEffectNumber), pTarget,iEffectNumber) = data;
  return(1);
}
*/
/*
local wdt, hgt, px, x, y;

func Initialize() {
  // Objekt zeichnen
  // Ja.
  px=10;
  wdt=LandscapeWidth()/px;
  hgt=LandscapeHeight()/px;
  x=GetX();
  y=GetY();
  SetShape(-wdt/2, -hgt/2, wdt, hgt);
  var i;
  for(i = wdt; i>=0; --i)
    SetLandscapePixel( -wdt/2+i-1, -hgt/2, RGB(0,0,1));
  for(i = wdt; i>=0; --i)
    SetLandscapePixel( -wdt/2+i-1, hgt/2, RGB(255,255,255));
  for(i = hgt; i>=0; --i)
    SetLandscapePixel( -wdt/2-1, -hgt/2+i, RGB(50,50,50));
  for(i = hgt; i>=0; --i)
    SetLandscapePixel( wdt/2, -hgt/2+i, RGB(205,205,205));
  AddEffect("Map", this(), 1, 1, this());
  return(1);
}

public func FxMapTimer(target, effect, time)
{
  SetPosition(x,y);
  var i=EffectVar(0, this(), effect)++;
  if(i == wdt-1) EffectVar(0, this(), effect)=0;
  var color, r, g, b, mat, npx=EffectVar(0, this(), effect);
  for(var j=hgt-1; j>0; --j)
  {
    if(GBackSemiSolid(-wdt/2+i, -hgt/2+j) && (GetMaterial(-wdt/2+i, -hgt/2+j) != Material("Vehicle"))) continue;
    mat=GetMaterial(i*px-x, j*px-y);
    r=GetMaterialVal("Color", 0, mat, 0);
    g=GetMaterialVal("Color", 0, mat, 1);
    b=GetMaterialVal("Color", 0, mat, 2);
    r+=RandomX(-5,5);
    g+=RandomX(-5,5);
    b+=RandomX(-5,5);
    r = Max(0, Min(r, 255));
    g = Max(0, Min(g, 255));
    b = Max(0, Min(b, 255));
    if(GBackSky(i*px-x, j*px-y) && MaterialName(mat) ne "Vehicle")
    {
      r=200; g=200; b=250;
    }
    if(FindObject(0, i*px-x, j*px-y), 0, 0, 0, 0, NoContainer())
    {
      r=250; g=25; b=20;
    }
//      CreateParticle("MapDot", -wdt/2+i, -hgt/2+j, 0, 0, 10, RGB(r, g, b));   
    SetLandscapePixel( -wdt/2+i, -hgt/2+j, RGB(r, g, b));
    SetLandscapePixel( -wdt/2+npx, -hgt/2+j, RGB(0, 255, 0));
  }
}

func UpdateTransferZone()
{
  SetShape(-wdt/2, -hgt/2, wdt, hgt);
}

*/
