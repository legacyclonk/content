/*-- Strahl --*/

#strict

private func Timer()
{
  // Positionen
  var x0 = GetX(GetActionTarget(1)), y0 = GetY(GetActionTarget(1)), 
  x1 = GetX(GetActionTarget(0)), y1 = GetY(GetActionTarget(0));
  // Einzelne Vertexe mit Partikeln verschönern
  var i=0;
  var iVtxNum = GetVertexNum();
  while(i<iVtxNum)
  {
    SetVertex(i, 0, (x0+(x1-x0)*i/iVtxNum)+RandomX(5,-5)); 
    SetVertex(i, 1, (y0+(y1-y0)*i/iVtxNum)+RandomX(5,-5));
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60, RGB(250,0,0), RGB(250,0,0));
    DrawParticleLine ("BloodSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(250,100,100), RGB(250,100,100));
    i++;
  }
}

/* Effekte */

private func Sparkle()
{
  Timer();
  // Energie entziehen
  if(!Random(2)){ DoEnergy(-1, GetActionTarget());
  DoEnergy(1, GetActionTarget(1));}
  return(1);
}

/* Ende */

private func Remove()
{
  // Von ein letzter Effekt!!
  var i=0;
  while(i<GetVertexNum()-1)
  {
    DrawParticleLine ("LightningSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(), 6, 60, RGB(250,0,0));
    DrawParticleLine ("BloodSpark", GetVertex(i)-GetX(), GetVertex(i, 1)-GetY(), 
		    GetVertex(i+1)-GetX(), GetVertex(i+1,1)-GetY(),  3, 25, RGB(250,100,100));
    i++;
  }
  
  RemoveObject();
}
