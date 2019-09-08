#strict

func InitializePlayer(int iPlayer)
{
  var pHut;
  if(iPlayer==0) pHut=CreateConstruction(HUT2,30  ,285,-1,100,1);
  if(iPlayer==1) pHut=CreateConstruction(HUT2,1410,285,-1,100,1);
  Enter(pHut,GetCrew(iPlayer,0));
  Enter(pHut,GetCrew(iPlayer,1));
  Enter(pHut,GetCrew(iPlayer,2));
  Enter(pHut,CreateObject(FLAG,0,0,iPlayer));
}