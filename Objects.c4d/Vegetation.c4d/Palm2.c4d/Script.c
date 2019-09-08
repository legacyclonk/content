/*-- Palme --*/

#strict

#include TRE1

// Etwas leichter zu fällen
private func TreeStrength() { return(100); }

/* Überladungen für zusätzliche Sturm-Animation */  

private func Breeze()
{
  if (Abs(GetWind()) < 50 + MotionThreshold) SetAction("Still");
  if (Abs(GetWind()) > 74 + MotionThreshold) SetAction("Storm");
}
    
private func Storm()
{
  if (Abs(GetWind()) < 75 + MotionThreshold) SetAction("Breeze");
}