/*--- Zünddraht ---*/

/* Locals */

// 0,1: Drahtfarben
// 2: Meldung ausgegeben

#strict

protected func Initialize ()
{
  // Local 0 und 1 sind die Lininenfarben
  Local(0) = 22;
  Local(1) = 22;
  // Die ersten beiden Vertices auf aktuelle Position legen
  SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
  SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
}

public func Connect (pTarget1, pTarget2)
{
  SetAction("Connect", pTarget1, pTarget2);  
}

private func Connecting ()
{
  var iDist = GetLineLength();	
  // Draht zu lang
  if (iDist > 350 )
    return(LineTooLong());
  // Draht zu lang: warnen
  if (iDist > 300 )
    return(WarnLineTooLong());
  // Meldungs-Flag zurücksetzen
  Local(2) = 0;
  // Kann nur Dynamit und Zünder verbinden
  if ( ! (GetID(GetActionTarget(0)) == DYNB || GetID(GetActionTarget(0)) == IGNB) ||
          ! (GetID(GetActionTarget(1)) == DYNB || GetID(GetActionTarget(1)) == IGNB) )
    return( Sound("LineBreak"), RemoveObject() );
}

private func GetLineLength()
{
  var i = GetVertexNum(this())-1;
  var iDist;
  while(i--)
  {
    // Die Länger zwischen den einzelnen Vertexen berechnen	  
    iDist += Distance(GetVertex(i,0),GetVertex(i,1),GetVertex(i+1,0),GetVertex(i+1,1));
  }
  return(iDist);
}
  
protected func LineBreak ()
{
  Sound("LineBreak");
}
  
private func LineTooLong ()
{
  Sound("LineBreak");
  RemoveObject();
}

private func WarnLineTooLong ()
{
  // Meldung wurde bereits ausgegeben
  if (Local(2)) return(1);
  // Meldung bei Drahtrolle am einen oder anderen Ende ausgeben
  var obj;
  if ( GetID(GetActionTarget(0)) == IGNB) obj = GetActionTarget(0);
  if ( GetID(GetActionTarget(1)) == IGNB) obj = GetActionTarget(1);
  if (obj) Message("$Zulang$", obj);  
  // Meldung ausgegeben: merken
  Local(2) = 1;
}
