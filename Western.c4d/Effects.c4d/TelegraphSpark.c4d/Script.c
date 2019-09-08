/*-- Leitungstestfunke --*/

#strict

local pLine,pTarget,pStart,iLineVertex,iDir;

protected func Initialize()
{
  SetVisibility(VIS_Owner());
  SetPlrView(GetOwner(), this());
  SetAction("Fly");
  Sound("Morse", 0,this(),100,GetOwner(), 1);
}

// Ausrichten
public func Set(line, start)
{
  pLine = line;
  pStart = start;
  // Zielpunkt
  pTarget = FindTarget();
  // Zielvertex auf der Leitung
  iLineVertex = FirstVertex();
  // Richtung bestimmen
  iDir = CheckDir();
}

/* Belegungsfunktionen */

private func FindTarget()
{
  // Keine Leitung?
  if(!pLine) return();
  // Schauen, welches ActionTarget pStart ist
  var i=-1;
  if(GetActionTarget(0, pLine) == pStart) i=1;
  if(GetActionTarget(1, pLine) == pStart) i=0;
  if(i == -1) return(); // <- seltsamer Fehler geschehen x(
  // Anderes ActionTarget suchen und in pTarget speichern
  return(GetActionTarget(i, pLine));
}

private func FirstVertex()
{
  // nächstes Vertex belegen und an die Startposition versetzen

  // Ich bin bei Vertex 0
  if(pStart == GetActionTarget(0, pLine))
    {
    SetPosition(GetVertex(0,0, pLine), GetVertex(0,1,pLine));
    return(1);
    }
  // Ich bei Vertex letzter
  if(pStart == GetActionTarget(1, pLine))
    {
    SetPosition(GetVertex(GetVertexNum(pLine)-1,0, pLine), GetVertex(GetVertexNum(pLine)-1,1,pLine));
    return(GetVertexNum(pLine)-2);
    }
}

private func CheckDir()
{
  // Stellt fest, ob die Leitungsvertices in ab- und aufsteigender Reihenfolge befahren werden
  if(iLineVertex == 1) return(1);
  else return(-1);
}

/* automagisches Leitungsentlangfließen */

protected func Trace()
{
  // Verschwindibus
  if(Reason2Die()) return(RemoveObject());
  // Ziel erreicht?
  if(ObjectDistance(pTarget, this()) <= 5)
    return(NewTarget());
  // Blar, Variablen
  var vx, vy;
  vx = GetVertex(iLineVertex,0,pLine);
  vy = GetVertex(iLineVertex,1,pLine);
  // Zielvertex erreicht?
  if(GetX() == vx && GetY() == vy)
    return(NewVertex());
  // Auf den Zielvertex versetzen
  if(Inside(GetX(), vx-5, vx+5) && Inside(GetY(), vy-5, vy+5))
    {
    SetPosition(vx, vy);
    return();
    }
  // Na ja, gut langweiliges weiterfliegen
  var nx, ny, angle = Angle(GetX(), GetY(), vx, vy)-90;
  nx =  Cos(angle*10, 3, 10);
  ny =  Sin(angle*10, 3, 10);
  SetPosition(GetX()+nx, GetY()+ny);
}

// Sterbegrund
private func Reason2Die()
{
  if(!pLine || !pStart || !pTarget || !iDir) return(1);
}

// Neues Ziel
private func NewTarget()
{
  // real Verbundenes Objekt
  var pConnect = GetActionTarget(1, pTarget), pCP, pNStart, pNLine;
  // Einfach anderen Verbindungspunkt suchen
  while(pCP = FindObject(CNNC, 0,0,0,0,0, "Connect", pConnect, 0, pCP))
    if(pCP != pTarget && !pNStart)
      pNStart = pCP;
  // Nix gefunden?
  if(!pNStart) { pStart=0; return(); }
  // Keine neue Leitung
  if(!(pNLine=GetActionTarget(0, pNStart))) { pStart=0; return(); }
  // Gefundener Punkt ist neuer Startpunkt (!)
  Set(pNLine, pNStart);
}

// Neuer Zielvertex
private func NewVertex()
{
  // Einfach iDir draufaddieren
  iLineVertex += iDir;
  // Testen, ob nicht alles kaputt ist, dann verschwinden wir nämlich lieber
  if(iLineVertex < 0 || iLineVertex == GetVertexNum(pLine))
    iDir=0;
}
