/* Kraftfeldbrücke */

#strict 2

protected func GetSegmentID() { return FBRS; }

protected func Initialize()
{
  var pLast, pNext;
  
  for (var i = 0; i < 4; i++)
  {
    var pNext = CreateObject(GetSegmentID(), GetDefCoreVal("Offset", "DefCore") + i*GetDefCoreVal("Width", "DefCore", GetSegmentID()), 0, NO_OWNER);
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