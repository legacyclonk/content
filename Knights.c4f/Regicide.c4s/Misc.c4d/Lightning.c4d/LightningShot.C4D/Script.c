/*-- Blitz --*/

#strict

static lgt_enlightment; // Globale Erhellung durch Blitze
static lgt_last_lgt_frame; // Letztes Frame, in dem die Helligkeit angepasst wurde

/* Locals */

local iAdvX, iVarX, // Bewegung / Abweichung X
      iAdvY, iVarY; // Bewegung / Abweichung Y
local fDoGamma;
local IsSideBranch;

local launcher;
/* Aktivierung */

public func Activate(iStartX, iStartY, iAdvanceX, iRandomX, iAdvanceY, iRandomY, SideBranch)
{
  // Startposition
  AddVertex(iStartX,iStartY);
  // Bewegungsverhalten
  iAdvX=iAdvanceX; iVarX=iRandomX;
  iAdvY=iAdvanceY; iVarY=iRandomY;
  fDoGamma=0;
  IsSideBranch=SideBranch;
  // Aktion setzen
  SetAction("Advance");
//  Message("X:%d Y:%d", 0, iAdvanceX, iAdvanceY);
  // Geräusch
  if (!Random(5)) Sound("Thunder*");
  // Leuchteffekt
  LightningEffect(10);
  return(1);
}

public func Launch(pLauncher, iDir, iX, iY, iAngle)
{
  var XDir;
  if (iDir == DIR_Left()) XDir = -Cos(iAngle, 10, 10);
  else (XDir = +Cos(iAngle, 10, 10));
  /*YDir = 0;
  SetXDir(XDir);
  SetYDir(YDir);
  XLast = GetX();
  YLast = GetY();
  SetAction("Flying");*/
CreateParticle("Blast", iX-GetX(),iY-GetY(), 0,0, 100, RGBa(128,128,255,0));
launcher=pLauncher;
  Activate(iX, iY, XDir, 0, -Sin(iAngle, 10, 10));
return(1);
}

public func Reflect()
{
	iAdvX = -iAdvX;
}

private func Timer()
{
  SetVertex(0, 0, GetX(launcher)); SetVertex(0, 1, GetY(launcher));
  if(GetID(launcher)==DTWR)  SetVertex(0, 1, GetY(launcher)-45);
  
  var i=0;
  while(i<GetVertexNum()-1)//GetVertex(i+1))
  {
//    SetVertex(i+1, 0, BoundBy(GetVertex(i+1,0)+RandomX(1,-1), GetVertex(i,0)+iAdvX-5, GetVertex(i,0)+iAdvX+5)); 
//    SetVertex(i+1, 1, BoundBy(GetVertex(i+1,1)+RandomX(1,-1), GetVertex(i,1)+iAdvY-5, GetVertex(i,1)+iAdvY+5));
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60, RGB(0,100,255));
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(250,250,255));
//    CreateParticle ("LightningSpark", GetVertex(i+1)-GetX(), GetVertex(i+1, 1)-GetY(), 0, 0, 100, RGB(0,100,255)); 
//    CreateParticle ("LightningSpark", GetVertex(i+1)-GetX(), GetVertex(i+1, 1)-GetY(), 0, 0, 50, RGB(250,250,255));
//    var x = ( GetVertex(i)-GetVertex(i+1) )/2;
//    var y = ( GetVertex(i,1)-GetVertex(i+1,1) )/2;
//    CreateParticle ("LightningSpark", GetVertex(i+1)-GetX()+x, GetVertex(i+1, 1)-GetY()+y, 0, 0, 50, RGB(250,250,255));
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

  // Objektschaden
  //if (iVtx>5) BlastObjects(iVtxX, iVtxY, 5);

  // Neuen Eckpunkt
  if (!AddVertex( iVtxX+iAdvX+RandomX(-5,5), iVtxY+iAdvY+RandomX(-5,5)))
    return(Remove());

  SetPosition(GetVertex(GetVertexNum()), GetVertex(GetVertexNum(), 1));
  
  // Im Wasser?
  if (GBackLiquid(iVtxX-GetX(), iVtxY-GetY() ) && !IsSideBranch)
  {
	  if(!Random(5))
	  {
		  var obj = CreateObject(S_01, iVtxX-GetX(), iVtxY-GetY());
		  var rnd1 = RandomX(0, 1);
		  if(!rnd1) rnd1=-1;
		  var rnd2 = RandomX(0, 1);
		  if(!rnd2) rnd1=-1;
		  obj->Activate(iVtxX, iVtxY, iAdvX*rnd1+RandomX(-5,5), iVarX, iAdvY*rnd2+RandomX(-5,5), iVarY, 1);
	  }
  }
  
  // Objektanziehung
  iVtx = GetVertexNum()-1; iVtxX = GetVertex(iVtx, 0); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if (iVtx>7)
    while (obj = FindObject( 0, iVtxX-GetX()-25, iVtxY-GetY()-25, 50, 50, OCF_Alive(), 0,0, NoContainer(),obj  ) )
    {
      if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
      Attraction(obj);
      break;
    }

  LightningEffect(4);
  
  Timer();
  // Weiter	
  return(ExecLgt());
}

/* Effekte */

private func Sparkle()
{
  Timer();
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
  Punch(obj, 12);
//  BlastObjects(GetX(obj), GetY(obj), 10);

//AddEffect("Shock", obj, 200, 3, 0, GetID());
  // Leuchteffekt
  LightningEffect(40);
}

/* Ende */

private func Remove()
{
  if (ObjectCount(FXL1)<=1) ResetGamma(5);

var i=0;
  while(i<GetVertexNum()-1)//GetVertex(i+1))
  {
//    SetVertex(i+1, 0, BoundBy(GetVertex(i+1,0)+RandomX(1,-1), GetVertex(i,0)+iAdvX-5, GetVertex(i,0)+iAdvX+5)); 
//    SetVertex(i+1, 1, BoundBy(GetVertex(i+1,1)+RandomX(1,-1), GetVertex(i,1)+iAdvY-5, GetVertex(i,1)+iAdvY+5));
    DrawParticleLine ("LightningSpark2", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60, RGB(0,100,255));
    DrawParticleLine ("LightningSpark2", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(250,250,255));
//    CreateParticle ("LightningSpark", GetVertex(i+1)-GetX(), GetVertex(i+1, 1)-GetY(), 0, 0, 100, RGB(0,100,255)); 
//    CreateParticle ("LightningSpark", GetVertex(i+1)-GetX(), GetVertex(i+1, 1)-GetY(), 0, 0, 50, RGB(250,250,255));
//    var x = ( GetVertex(i)-GetVertex(i+1) )/2;
//    var y = ( GetVertex(i,1)-GetVertex(i+1,1) )/2;
//    CreateParticle ("LightningSpark", GetVertex(i+1)-GetX()+x, GetVertex(i+1, 1)-GetY()+y, 0, 0, 50, RGB(250,250,255));
    i++;
  }
  
  RemoveObject();
}

protected func FxShockStart(pTarget, iEffectNumber)
  {
  // Vorherige Physicals speichern
  EffectVar(0, pTarget, iEffectNumber) = 10;
  EffectVar(1, pTarget, iEffectNumber) = GetPhysical("Walk", 0, pTarget);
  EffectVar(2, pTarget, iEffectNumber) = GetPhysical("Swim", 0, pTarget);
  EffectVar(3, pTarget, iEffectNumber) = GetPhysical("Float", 0, pTarget);
  EffectVar(4, pTarget, iEffectNumber) = GetPhysical("Scale", 0, pTarget);
  EffectVar(5, pTarget, iEffectNumber) = GetPhysical("Hangle", 0, pTarget);
  EffectVar(6, pTarget, iEffectNumber) = GetPhysical("Jump", 0, pTarget);
  EffectVar(7, pTarget, iEffectNumber) = GetPhysical("Dig", 0, pTarget);
  EffectVar(8, pTarget, iEffectNumber) = GetClrModulation(pTarget);
  if(!EffectVar(8, pTarget, iEffectNumber)) EffectVar(8, pTarget, iEffectNumber) = RGBa(255,255,255,0);
  SetPhysical("Walk",   0, 2, pTarget);
  SetPhysical("Swim",   0, 2, pTarget);
  SetPhysical("Float",  0, 2, pTarget);
  SetPhysical("Scale",  0, 2, pTarget);
  SetPhysical("Hangle", 0, 2, pTarget);
  SetPhysical("Jump",   0, 2, pTarget);
  SetPhysical("Dig",   0, 2, pTarget);
  pTarget->~Disabeled();
  // Fertig
  return(1);
  }

protected func FxShockTimer(pTarget, iEffectNumber)
  {
  if((--EffectVar(0, pTarget, iEffectNumber))<=0)
  {
	  return(-1);
  }
  // Visueller Effekt
  if(Mod(EffectVar(0, pTarget, iEffectNumber), 2)) SetClrModulation(EffectVar(8, pTarget, iEffectNumber), pTarget);
  else SetClrModulation(RGBa(GetRGBaValue(EffectVar(8, pTarget, iEffectNumber), 1),
			     GetRGBaValue(EffectVar(8, pTarget, iEffectNumber), 2), 
			     GetRGBaValue(EffectVar(8, pTarget, iEffectNumber), 3), 150), pTarget);
  // Fertig
  return(1);
  }

protected func FxShockStop(pTarget, iEffectNumber)
{
// Status wiederherstellen
	  SetPhysical("Walk",   EffectVar(1, pTarget, iEffectNumber), 2, pTarget);
	  SetPhysical("Swim",   EffectVar(2, pTarget, iEffectNumber), 2, pTarget);
	  SetPhysical("Float",  EffectVar(3, pTarget, iEffectNumber), 2, pTarget);
	  SetPhysical("Scale",  EffectVar(4, pTarget, iEffectNumber), 2, pTarget);
	  SetPhysical("Hangle", EffectVar(5, pTarget, iEffectNumber), 2, pTarget);
	  SetPhysical("Jump",   EffectVar(6, pTarget, iEffectNumber), 2, pTarget);
	  SetPhysical("Dig",    EffectVar(7, pTarget, iEffectNumber), 2, pTarget);
	  SetClrModulation(EffectVar(8, pTarget, iEffectNumber), pTarget);
	  pTarget->~NotDisabeled();
	  if(Random(2)) pTarget->~ContactLeft();
	  else pTarget->~ContactRight();
}

func FxShockEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  // weitere Shocks integrieren
  if(szNewEffectName eq "Shock")
  {	
	EffectVar(0, pTarget, iEffectNumber) == 10;
	return(-1);
  }
}
