/*-- Angelegte Lanze --*/

#strict

local high_target, last_x, speed_x;

/* Locals */

// 0: Zielhöhe
  
/* Ablegen */  

public func Unbuckle()
  {
  ChangeDef(UnbuckledID() );
  Sound("Connect");
  return(1);  
  }
  
public func UnbuckledID() { return(LANC); }
  
/* Aktion */
  
private func Lancing()
  {
  // Abgelegt/Verloren
  if (ActIdle() || !GetAlive(GetActionTarget())) return(ChangeDef(LANC));
  // Da bei Objekten mit Prozedur WALK oder ATTACH meistens kein
  // sinnvoller XDir-Wert ermittelt werden kann, muß ein
  // eigener berechnet werden.
  var vx = GetX()-last_x; last_x = GetX(); speed_x=vx;
  // Zielwinkel nach Bewegung ausrichten (Mit Rankabhängiger Ungenauigkeit)
  var iRnd = Random(Abs(speed_x) - Abs(speed_x/2)) / BoundBy((GetRank(GetActionTarget()) - 3) / 2, 1, 6);
  var dst_angle = BoundBy(vx * (high_target*5 + 20) / 10, -111, 111) + iRnd;
  // Eigener Winkel
  var angle = GetR(); while (angle>180) angle-=360;
  // Drehrichtung anpassen
  var iMoveSlow = 10; if (Abs(vx) > 20) iMoveSlow = 3;
  SetRDir(BoundBy( (dst_angle-angle) / iMoveSlow ,-12,12));
  return(1);
  }

private func Targeting()
  {
  // Bei hoher Geschwindigkeit Ziele beschädigen
  var obj;
  if (Abs(speed_x) > 30)
    if (obj = FindObject(0,GetVertex(1,0),GetVertex(1,1),0,0,OCF_Prey(),0,0,NoContainer()))
      if (GetAction(obj) ne "Ride" || !Random(3))
        {
        Punch(obj, Max(15 - ObjectCall(obj,"IsShielded")*14/100, 1));
        Sound("LanceHit");
        speed_x = 0;
        }
  return(1);
  }

/* Steuerung */

public func ControlTargetHeight()
  {
  // Wird durch den Ritter aufgerufen: wechselt zwischen hohem und tiefem Ziel
  high_target = !high_target;
  return(1);
  }

/* Verkauf */

protected func SellTo() { return(LANC); }
