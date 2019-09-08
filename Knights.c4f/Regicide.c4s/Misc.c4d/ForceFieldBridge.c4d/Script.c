/* Kraftfeldbrücke */
#strict
protected func GetSegmentID() { return(_FBS); }
protected func Initialize()
{ 
 var pLast, pNext;
   
 for (var i = 0; i < 4; i++) 
 {
    var pNext = CreateObject(GetSegmentID(), GetDefCoreVal("Offset", "DefCore") + i*GetDefCoreVal("Width", "DefCore", GetSegmentID()), 0, -1); 
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
