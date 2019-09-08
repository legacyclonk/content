/*-- Blitz --*/

#strict

static lgt_enlightment; // Globale Erhellung durch Blitze
static lgt_last_lgt_frame; // Letztes Frame, in dem die Helligkeit angepasst wurde

/* Locals */

local iAdvX, iVarX, // Bewegung / Abweichung X
      iAdvY, iVarY; // Bewegung / Abweichung Y
local fDoGamma;

/* Aktivierung */

public func Activate(iStartX, iStartY, iAdvanceX, iRandomX, iAdvanceY, iRandomY, fApplyGamma)
{
  // Startposition
  AddVertex(iStartX,iStartY);
  // Bewegungsverhalten
  iAdvX=iAdvanceX; iVarX=iRandomX;
  iAdvY=iAdvanceY; iVarY=iRandomY;
  fDoGamma=fApplyGamma;
  // Aktion setzen
  SetAction("Advance");
  // Geräusch
//  if (!Random(5)) Sound("Thunder*");
  Sound("Thunder",1);
  // Leuchteffekt
  LightningEffect(10);
  return(1);
}

/* Bewegung */

private func Advance()
{
  // Einschlag
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  if (GBackSemiSolid(iVtxX-GetX(), iVtxY-GetY() ))
    return(SetAction("Connect2"));//Remove());

  // Objektschaden
  if (iVtx>5) BlastObjects(iVtxX, iVtxY, 5);

  // Verzweigung
//  if (!Random(45))
//    LaunchLightning(iVtxX, iVtxY, iAdvX, iVarX, iAdvY, iVarY);

  // Neuen Eckpunkt
  var iXRand = Random(iVarX);
  var iYRand = Random(iVarY);
  if (!AddVertex( iVtxX+iAdvX+iXRand, iVtxY+iAdvY+iYRand))
  {
    if (!Random(2));
      Schedule(Format("LaunchLightning(%d, %d, %d, %d, %d, %d)",iVtxX, iVtxY, iAdvX, iVarX, iAdvY, iVarY),1);
    return(SetAction("Connect2"));//Remove());
  }
  if(!PathFree(iVtxX, iVtxY, iVtxX+iAdvX+iXRand, iVtxY+iAdvY+iYRand))
  {
    return(SetAction("Connect2"));
  }

  // Objektanziehung
  iVtx = GetVertexNum()-1; iVtxX = GetVertex(iVtx, 0); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if (iVtx>7)
    if (obj = FindObject( 0, iVtxX-GetX()-50, iVtxY-GetY()-50, 100, 100, OCF_AttractLightning(), 0,0, NoContainer()  ) )
      Attraction(obj);

  LightningEffect(4);

  // ExecLgt()

  Advance();
  
  // Weiter	
  return();
}

/* Effekte */

private func Sparkle()
{
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  CastObjects(SPRK,1,20,iVtxX-GetX(), iVtxY-GetY());
  ExecLgt();
  return(1);
}

private func LightningEffect(strength)
  {
  lgt_enlightment+=Random(strength)+strength/2;
  return(1);
  }

private func ExecLgt()
  {
  if (!fDoGamma) return(1);
  // Lichteffekt ausführen (Rampe 5)
  if (lgt_enlightment>0 && lgt_last_lgt_frame != FrameCounter())
    {
    var lgt=Min(lgt_enlightment*3, 255);
    SetGamma(RGB(lgt, lgt, lgt), RGB(128+lgt/2, 128+lgt/2, 128+lgt/2), 16777215, 5);
    lgt_enlightment=Max(lgt_enlightment*4/8-1); lgt_last_lgt_frame=FrameCounter();
    }
  }


/* Anziehung */

private func Attraction(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex(0,0,obj)+GetX(obj),
             GetVertex(0,1,obj)+GetY(obj) );
  obj->~LightningStrike();
  SetAction("Connect");
  // Leuchteffekt
  LightningEffect(40);
}

/* Ende */

private func Remove()
{
  if (ObjectCount(FXL1)<=1) ResetGamma(5);
  RemoveObject();
}
