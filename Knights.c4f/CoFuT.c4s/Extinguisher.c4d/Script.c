/*-- Löschobjekt --*/

#strict

global func FxFireStart(object obj, int iNumber, int iTemp, int iCausedBy, bool fBlasted, object pIncineratingObject)
  {
  // Wenn ein Objekt zu brennen beginnt, gleich das Löschen planen
  var i;
  if (!iTemp) if (GetAlive(obj)) if (i=ObjectCount(_ETG))
    {
    // Lösche in Schlüsselframes, damit nahe beieinanderliegende Objekte ungefähr gleichzeitig gelöscht werden
    // Dies verringert die Chance des Neuanzündens
    var iExtgTime = 200/i + 50 - (FrameCounter() % 50);
    AddEffect("IntExtinguisher", obj, 1, iExtgTime, obj);
    }
  return(inherited(obj, iNumber, iTemp, iCausedBy, fBlasted, pIncineratingObject));
  }

global func FxIntExtinguisherStop(object obj, int iNumber, int iReason, bool fTemp)
  {
  if (!fTemp)
    {
    // Zeit zum Löschen!
    Extinguish(obj);
    }
  }

global func FxFireStop(object obj, int iNumber, int iReason, bool fTemp)
  {
  if (!fTemp)
    {
    // Das Objekt wurde gelöscht (entweder durch den Löscheffekt, oder anderswie): Löcheffekt entfernen
    RemoveEffect("IntExtinguisher", obj, 0, true);
    }
  return(inherited(obj, iNumber, iReason, fTemp));
  }
