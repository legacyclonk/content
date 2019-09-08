/*-- Last Hope --*/

#strict

protected func Initialize()
{
  var pObj;
  if(!ObjectCount(CP4V)) CastObjects(CP4V,3,0,50,50); // Begrentzte Tränke
  if(!ObjectCount(NDWA)) CastObjects(NDWA,5,0,50,50); // Durst
  if(!ObjectCount(NDFO)) CastObjects(NDFO,5,0,50,50); // Hunger
  if(!ObjectCount(TKNW)) CreateObject(TKNW,50,50,-1); // Teambaupläne
  if(!ObjectCount(CNMT)) CreateObject(CNMT,50,50,-1); // Baumaterial

  if(!ObjectCount(SCRG)) CreateObject(SCRG,50,50,-1); // Spielziel
  if(!ObjectCount(REPR)) CreateObject(REPR,50,50,-1); // Nachwuchs

  // Baumbestand
  pObj = FindObject(TREP);
  if(!pObj) pObj = CreateObject(TREP,50,50, -1);
  pObj->~Set(TRB1,0);
  pObj->~Set(TRB2,0);
  pObj->~Set(TRB3,0);
  pObj->~Set(BUSB,0);

  // Ein paar Zaps :-)
  if(!GetEffect("Zaps")) AddEffect("Zaps", 0, 1, 600);

  // Sicher stellen, dass man in die Starthöhle kriechen kann
  FreeRect(960,359,40,9);

  SetSkyParallax(0, 20, 10);

  ScriptGo(1);
  return(1);
}

global func SetDTRotation (int r, int xoff, int yoff, object obj) {
  var fsin=Sin(r, 1000), fcos=Cos(r, 1000);
  // set matrix values
  SetObjDrawTransform (
    +fcos, +fsin, (1000-fcos)*xoff - fsin*yoff,
    -fsin, +fcos, (1000-fcos)*yoff + fsin*xoff,
    obj
  );
}

protected func InitializePlayer(int iPlr, int tx, int ty, object pBase, int iTeam)
  {
  // Immer FoW, damit FoW-Generatoren funzen
  Schedule(Format("SetFoW(true, %d)", iPlr), 1);

  // Von links starten
  var pCrew = GetCrew(iPlr);
  pCrew->SetPosition(97,238);
  
  }

global func FxZapsTimer()
{
  var obj;
  while(obj = FindObject(CAC2, 0, 0, 0, 0, 0, 0, 0, 0, obj))
    CreateObject(ZAP1, GetX(obj), GetY(obj), -1);
}

protected func Script1()
{
  var obj = FindObject(IDOL, 0, 0, -1, -1);
  LocalN("iText", obj) = 0;
  obj->ActivateEntrance(obj);
}

protected func Script18()
{
  var obj = FindObject(IDOL, 0, 0, -1, -1);
  LocalN("iText", obj) = 1;
  obj->ActivateEntrance(obj);
}

protected func Script15()
{
  var obj = FindObject(IDOL, 0, 0, -1, -1);
  LocalN("iText", obj) = 2;
  ScriptGo(0);
}

protected func OnGoalsFulfilled()
{
  GainMissionAccess("WestLH");
  return(0);    
}
