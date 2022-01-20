/*-- Nadelbaum --*/

#strict

#include TREE

func Initialize() {
    SetGraphics(0,this(),TRE4);
}

/* Initialisierung */

protected func Construction()
{
  // Bewegungsschwelle
  Local(0)=Random(10);
  // Aktionszeitoffset
  Local(1)=Random(20);
  // Verzögerte Initialisierung
  SetAction("Initialize");
  // Ausrichtung
  SetDir(Random(2));
  return(0);
}
  
private func Initializing()
{
  if (GetPhase() == Local(0)) SetAction("Still");
  return(1);  
}

/* Bewegung (Wind) */  
  
private func Still()
{
  if (Abs(GetWind()) > 49 + Local(0)) SetAction("Breeze");
  return(1);
}
    
private func Breeze()
{
  if (Abs(GetWind()) < 50 + Local(0)) SetAction("Still");
  return(1);    
}
