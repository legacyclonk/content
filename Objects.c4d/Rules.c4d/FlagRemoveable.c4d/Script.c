/*-- Flagge abnehmbar --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}
