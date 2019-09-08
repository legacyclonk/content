/* Grosser Stein */
// Der Stein rollt immer nach links herunter und zerfaellt im Tal

#strict 2

#appendto RCK2

protected func ContactBottom()
{
  if (GetX()<1000) return Destruct();
  _inherited(...);
  SetXDir(GetXDir() - 50);
  return true;
}
