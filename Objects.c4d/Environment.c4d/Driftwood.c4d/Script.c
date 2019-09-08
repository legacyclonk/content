/*-- Treibholz --*/

#strict

// Maximale Treibholzmenge
local MaxAmount;

protected func Initialize()
{
  SetAction("Initialize");
  SetPosition(50,60);
  return(1);
}
  
/* Verzögerte Initialisierung */  
  
private func Initialized()
{
  // Maximale Treibholzmenge richtet sich nach der
  // Anzahl der platzierten Treibholzobjekte x 10 + 10
  MaxAmount = ObjectCount(GetID(this())) * 10 + 10;
  // Alle übrigen Steuerungsobjekte dieser Art entfernen
  var pOther;
  while (pOther = FindObject(GetID(this())))
    RemoveObject(pOther);
  // Steuerungsaktion
  SetAction("Driftwood");
}
  
/* Treibholz plazieren */  
  
private func PlaceDriftwood()
{
  // Zufallssteuerung
  if (Random(30)) return(0);
  // Nicht zu viel Treibholz gleichzeitig in der Landschaft (im Freien)
  if (ObjectCount(WOOD) > MaxAmount) return(0);
  // Position ermitteln
  var x,y;
  // X-Position
  x = Random(LandscapeWidth());
  // Überprüfen, ob an dieser Position Flüssigkeit ist
  while ((y < LandscapeHeight()) && !GBackSemiSolid(x, y))
    y += 20;
  // Keine Flüssigkeit
  if (!GBackLiquid(x, y)) return(0);
  // Treibholz erzeugen
  SetR(Random(360), CreateObject(WOOD, x, y, -1));
}
