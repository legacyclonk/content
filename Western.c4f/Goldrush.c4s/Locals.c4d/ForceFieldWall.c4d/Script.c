/* Kraftfeldwand */

#strict


private func GetSegmentID() { return(_FWS); }

protected func Initialize()
{ 
 var pLast, pNext;
   
 for (var i = 0; i < 9; i++) 
 {
    var pNext = CreateObject(GetSegmentID(), 0, GetDefCoreVal("Offset", "DefCore", GetID(), 1) + i*GetDefCoreVal("Height", "DefCore", GetSegmentID()), -1); 
   pNext->SetAction("Field"); 
   if (pLast) 
   { 
     pNext->LocalN("pLast") = pLast; 
     pLast->LocalN("pNext") = pNext; 
   } 
   pLast = pNext; 
 }
   
 return(RemoveObject());
}
