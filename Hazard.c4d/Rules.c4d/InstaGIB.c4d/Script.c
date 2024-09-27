/*-- Insta GIB --*/

#strict

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

protected func Initialize()
{
  ScheduleCall(this(), "Initialized", 1);
}

protected func Initialized()
{
  // Keine-Munition-Regel muss da sein
  if(!FindObject(NOAM)) CreateObject(NOAM, 0,0, -1);
}
/*
public func InitializePlayer(int iPlr)
{
  for(var i=0 ; GetCrew(iPlr, i) ; i++)
    CreateContents(IGWP, GetCrew(iPlr, i));
}

public func RelaunchPlayer(int iPlr, object pClonk)
{
if(!GetAlive(pClonk)) return(ScheduleCall(0,"RelaunchPlayer",1,0,iPlr,pClonk));
//  if(!pClonk || !GetAlive(pClonk))
    if(!(pClonk = GetCursor(iPlr)))
      return(ScheduleCall(0,"RelaunchPlayer",1,0,iPlr,pClonk));

  CreateContents(IGWP, pClonk);
}*/

public func OnClonkRecruitment(object pClonk, int iPlr)
{
  CreateContents(IGWP, pClonk)->DoAmmo(ENAM, 1);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }
