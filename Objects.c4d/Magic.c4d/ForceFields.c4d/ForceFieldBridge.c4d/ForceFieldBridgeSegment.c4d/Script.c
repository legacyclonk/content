/* Kraftfeldbruecke - Segment */

#strict 2
#include FCWS

private func CheckStuck()
{
  var pClonk,iYChange,iX,iY;
  // Alle feststeckenden Clonks in der Nähe suchen
  for (pClonk in FindObjects(Find_InRect(-20,-20,40,40), Find_OCF(OCF_CrewMember),Find_NoContainer()))
  {
    iX=GetX(pClonk);
    iY=GetY(pClonk);
    while(Stuck(pClonk) && Inside(GetY(pClonk)-GetY(),-20,20))
    {
      if(!(iYChange=BoundBy(GetY(pClonk)-GetY(),-1,1))) iYChange=1;
      // Zur Sicherheit...
      if(!Inside(GetY(pClonk)+iYChange,-100,LandscapeHeight())) break;
      SetPosition(GetX(pClonk),GetY(pClonk)+iYChange,pClonk);
    }
    // verschieben fehlgeschlagen: rückgängig machen
    if(Stuck(pClonk)) SetPosition(iX,iY,pClonk);
  }
}