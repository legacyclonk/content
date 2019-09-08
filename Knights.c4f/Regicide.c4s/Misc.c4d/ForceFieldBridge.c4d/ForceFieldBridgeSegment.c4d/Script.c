#strict
#include _FWS

private func CheckStuck()
{
  var pClonk,iYChange,iX,iY;

  // Alle feststeckenden Clonks in der Nähe suchen
  while(pClonk=FindObject(0,-20,-20,40,40,OCF_CrewMember(),0,0,NoContainer(),pClonk))
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
