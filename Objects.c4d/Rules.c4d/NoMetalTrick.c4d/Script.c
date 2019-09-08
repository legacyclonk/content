/*-- Kein Metall vor Gebäuden --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}
