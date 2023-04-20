/*-- Blitz --*/

#strict

/* Locals */

local iAdvX, iVarX, // Bewegung / Abweichung X
      iAdvY, iVarY; // Bewegung / Abweichung Y

/* Aktivierung */

public func Activate(iStartX, iStartY, iAdvanceX, iRandomX, iAdvanceY, iRandomY)
{
  // Startposition
  AddVertex(iStartX,iStartY);
  // Bewegungsverhalten
  iAdvX=iAdvanceX; iVarX=iRandomX;
  iAdvY=iAdvanceY; iVarY=iRandomY;
  // Aktion setzen
  SetAction("Advance");
  // Geräusch
  if (!Random(5)) Sound("Thunder*");
  return(1);
}

public func Launch(iX, iY, iAngle)
{
  SetVisibility(VIS_None);
  var XDir;
  XDir = Cos(iAngle, 10, 10);
//  if (iDir == DIR_Left()) XDir = -Cos(iAngle, 10, 10);
//  else (XDir = +Cos(iAngle, 10, 10));
  Activate(iX, iY, XDir, 0, -Sin(iAngle, 10, 10));
  return(1);
}

private func Timer()
{
  var i=0;
  var r;
  // Partikel-Effekt
  while(i < GetVertexNum()-1)
  {
  	r = (90*i)/GetVertexNum();
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60, RGB(255,255,128+Sin(r,255)));
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(255,255,250));
    i++;
  }
}

/* Bewegung */

private func Advance()
{
  // Einschlag
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  if (GBackSolid(iVtxX-GetX(), iVtxY-GetY() ))
    return(Remove());

  // Neuen Eckpunkt
  if (!AddVertex( iVtxX+iAdvX+RandomX(-5,5), iVtxY+iAdvY+RandomX(-5,5)))
    return(Remove());

  SetPosition(GetVertex(GetVertexNum()), GetVertex(GetVertexNum(), 1));
  
  // Im Wasser?
  if (GBackLiquid(iVtxX-GetX(), iVtxY-GetY() ))
  {
	  var obj;
	  // Alle Lebewesen in der Nähe bekommen was ab
	  while (obj = FindObject( 0, iVtxX-GetX()-400, iVtxY-GetY()-400, 800, 800, OCF_Alive(), 0,0, NoContainer(),obj  ) )
    	  {
      	    if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
	    if(!GBackLiquid(GetX(obj)-GetX(), GetY(obj)-GetY())) continue;
  	    obj->~LightningStrike();
  	    DoDmg(12, DMG_Energy, obj);
            DrawParticleLine ("LightningSpark2", iVtxX-GetX(), iVtxY-GetY(), 
		    GetX(obj)-GetX(), GetY(obj)-GetY(), 6, 60, RGB(0,100,255));
            DrawParticleLine ("LightningSpark2", iVtxX-GetX(), iVtxY-GetY(), 
		    GetX(obj)-GetX(), GetY(obj)-GetY(),  3, 25, RGB(250,250,255));
          }
	  return(Remove());
  }
  
  // Objektanziehung
  iVtx = GetVertexNum()-1; iVtxX = GetVertex(iVtx, 0); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if (iVtx>7)
  {
    while (obj = FindObject( 0, iVtxX-GetX()-25, iVtxY-GetY()-25, 50, 50, OCF_Alive(), 0,0, NoContainer(),obj  ) )
    {
      if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
      Attraction(obj);
      break;
    }
  }
  for(obj in FindObjects(Find_ID(GetID()), Find_Distance(35,iVtxX-GetX(), iVtxY-GetY()), Find_Exclude(this())))
  {
    AttractionLightning(obj);
    break;
  }
  
//  Timer();
  // Weiter	
  return();
}

/* Effekte */

private func Sparkle()
{
//  Timer();
  var iVtx = GetVertexNum()-1, iVtxX = GetVertex(iVtx, 0), iVtxY = GetVertex(iVtx, 1);
  CastObjects(SPRK,1,20,iVtxX-GetX(), iVtxY-GetY());
  return(1);
}

/* Anziehung */

private func Attraction(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex(0,0,obj)+GetX(obj),
             GetVertex(0,1,obj)+GetY(obj) );
  obj->~LightningStrike(this());
  SetAction("Connect");
  DoDmg(20, DMG_Energy, obj);
}

public func LightningStrike(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex(GetVertexNum(obj)-1,0,obj),
             GetVertex(GetVertexNum(obj)-1,1,obj) );
  SetAction("Connect");
}

private func AttractionLightning(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex(GetVertexNum(obj)-1,0,obj),
             GetVertex(GetVertexNum(obj)-1,1,obj) );
  obj->~LightningStrike(this());
  SetAction("Connect");
}

/* Ende */

private func Remove()
{
  var i=0;
  var r;
  // Leuchten
  while(i<GetVertexNum()-1)
  {
   r = (180*i)/GetVertexNum();
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60-r/6, RGBa(255,255,128+Sin(r,128),20));
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  2, 25-r/10, RGB(255,255,250));
		    
    DrawParticleLine ("LightningSpark2", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60-r/6, RGBa(255,255,128+Sin(r,128),20));
    DrawParticleLine ("LightningSpark2", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  2, 25-r/10, RGB(255,255,250));
    i++;
  }
  
  RemoveObject();
}
