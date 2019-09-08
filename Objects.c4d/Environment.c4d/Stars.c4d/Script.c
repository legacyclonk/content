/*-- Sternenhimmel --*/

#strict

/* Locals */

// Maximale Anzahl Sterne
local MaxCount;

protected func Initialize()
{
  SetAction("Initialize");
  SetPosition();
  return(1);
}

private func Initialized()
{
  // Plazierungsfaktor ermitteln
  var iCount = ObjectCount(GetID()) + 1;
  // Andere Objekte des gleichen Typs entfernen
  var pOther;
  while (pOther = FindObject(GetID()))
    RemoveObject(pOther);
  // Maximale Anzahl Sterne berechnen
  MaxCount = (LandscapeWidth() * LandscapeHeight() * iCount) / 20000;
  // Steuerungsaktion
  SetAction("Day");
  return(1);
}
    
private func Day()
{
  // Es wird Nacht
  if (IsNight())
    SetAction("Dusk");
  return(1);
}
    
private func Dusk()
{
  // Alle Sterne plaziert
  if (GetActTime() > MaxCount) 
    return(SetAction("Night"));
  // Sterne hinzufügen
  CreateObject(STAR, Random(LandscapeWidth()), Random(LandscapeHeight()), -1);
  return(1);
}
        
private func Night()
{
  // Es wird Tag
  if (IsDay())
    SetAction("Dawn");
  return(1);
}
    
private func Dawn()
{
  // Alle Sterne entfernt
  var pStar;
  if (!(pStar = FindObject(STAR))) 
    return(SetAction("Day"));
  // Sterne entfernen
  RemoveObject(pStar);
  return(1);
}
