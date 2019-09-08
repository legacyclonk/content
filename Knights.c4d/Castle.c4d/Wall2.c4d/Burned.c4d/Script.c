/*-- Verbranntes Burgteil --*/

#strict

/* Entzündung */

protected func Incineration()
{
  return(FCastleChange());
}

// Burgteile auf ihre Daseinsberechtigung überprüfen (Brustwehren inmitten der Burg, etc.)
public func FCastleChange()
{
  // Alle Teile benachrichtigen
  var pPart;
  while(pPart = FindObject(0, 0, 0, 0, 0, OCF_Fullcon(), 0,0, NoContainer(), pPart))
    pPart->~CastleChange();
  return(1);
}