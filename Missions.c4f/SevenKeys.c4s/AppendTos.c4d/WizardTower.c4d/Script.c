
#strict

#appendto WTWR

// Keine Hintergrund-Magieaufladung
protected func RefillMagic() { }

// Weniger Energie durch Blitzeinschläge
public func LightningStrike() 
{
  // Nur wenn fertig gebaut
  if (GetCon() < 100) return(0);
  // Energiegewinnung
  for(var i = 0; i < 5; ++ i)
    DoMagicEnergy(1);
  // Sound
  Sound("Electric");
  // Effekt-Aktion
  if (GetAction() ne "Ready") return(0);
  SetAction("Energize");
  return(1);
}
