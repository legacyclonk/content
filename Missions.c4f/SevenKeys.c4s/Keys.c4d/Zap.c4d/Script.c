
#strict

#include ZAP1
#include _KEY

public func Init(object oWhere)
{
	// In der linken oder rechten oberen Ecke platzieren (es ist wohl hinreichend realistisch,
	// dass die frei sind. Und wenn nicht, hilft es dem Spieler auch nicht viel)
	if(Random(2))
		SetPosition(100 + Random(200), 100 + Random(200));
	else
		SetPosition(LandscapeWidth() - 100 - Random(200), 100 + Random(200));
	return(1);
}

public func Announce(object oWhere)
{
	Message("$TxtAnnounce$", oWhere);
}

private func Sting(obj) {
	// Steche _richtig_ hart zu.
  Punch(obj,10+Random(10));
  Sound("Sting");
}

protected func Check() { // wird alle 18Frames aufgerufen
  var obj;

  // Verschüttet oder im Wasser?: Sterben
  if(GBackSemiSolid()) return(Death());
  // einfach so Sterben
  if(!Random(160)) return(Death());
  // Summen
  if(!Random(50)) Sound("Bsss");

  // Vom aktuellen Ziel ablassen und jemand anders verfolgen (falls jemand da ist)
  if(!Random(10)) target=FindTarget(200);
  // Das Ziel hat sich in ein Gebäude gerettet
  if(Contained(target)) target=0; 
  // Vom Ziel ablassen weil er nicht direkt erreichbar ist
  if(!Random(5)) if(!PathFree(GetX(),GetY(),GetX(target),GetY(target))) target=0; 

  // stechen
  if(!Random(2))
    if(obj=FindObject(0,1,0,0,0,OCF_Prey(),0,0,NoContainer()))
      Sting(obj);
}

protected func Death() { /* Nö :) */ }

public func FindTarget(maxdist) {  // maximale Distanz zum "Opfer"
  var obj;
  // neues Ziel bestimmen falls es nicht zu weit entfernt ist
  if(obj=FindObject(0,0,0,-1,-1,OCF_Prey(),0,0,NoContainer()))
    if(ObjectDistance(obj)<maxdist)
			return(obj);
}

private func Flying() { // wird jedes Frame aufgerufen
  var xdir,ydir;

  // nichts tun
  if(Random(4)) return(1);
  // Geschwindigkeit zufällig bestimmen
  xdir=Random(13)-6;
  ydir=Random(9)-4;
  // Verfolgt der Zap jemanden? Geschwindigkeit abstimmen
  xdir+=BoundBy(2*(GetX(target)-GetX()),-50,50);
  ydir+=BoundBy(2*(GetY(target)-GetY())/3,-40,40);
  if(GBackLiquid(xdir,ydir)) return(SetSpeed());
  SetSpeed(xdir,ydir);
}

public func GetKeyDesc() { return "$TxtAnnounce$"; }
