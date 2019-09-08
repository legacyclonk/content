/*-- Munitionsnachschub --*/

#strict

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(), iPlr);
  return(1);
}

protected func Supply()
{
  // Alle Munitionslager durchsuchen
  var pArmory;
  while(pArmory = FindObject(AMRY, 0,0,0,0,OCF_Fullcon(),0,0,0, pArmory))
    // Keine Munition vorhanden
    if(!FindContents(AMBO, pArmory))
      // Erzeugen
      CreateContents(AMBO, pArmory);
}
