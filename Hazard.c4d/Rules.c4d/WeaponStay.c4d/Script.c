/*-- Weapon Stay --*/

#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

protected func Initialize() {
  for(var pSpawn in FindObjects(Find_ID(SPNP)))
    pSpawn->Initialized();
}

protected func InitializePlayer(int iPlr)
{  
  for(var pSpawn in FindObjects(Find_ID(SPNP)))
  {
    if(GetPlayerType(iPlr))
      if(!(Local(0,pSpawn) & 1 << iPlr))
        Local(0,pSpawn) += 1 << iPlr;
  }
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }
