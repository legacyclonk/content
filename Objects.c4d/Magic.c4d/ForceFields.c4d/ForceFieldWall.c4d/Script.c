/* Kraftfeldwand (Objekt) */

#strict 2

private func GetSegmentID() { return FCWS; }

protected func Initialize()
{
  var pLast, pNext;
  
  for (var i = 0; i < 7; i++)
  {
    var pNext = CreateObject(GetSegmentID(), 0, GetDefCoreVal("Offset", "DefCore", GetID(), 1) + i*GetDefCoreVal("Height", "DefCore", GetSegmentID()), NO_OWNER);
    pNext->SetAction("Field");
    if (pLast)
    {
      pNext->LocalN("pLast") = pLast;
      pLast->LocalN("pNext") = pNext;
    }
    pLast = pNext;
  }

  RemoveObject();
  return true;
}