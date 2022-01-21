/*-- Schutt --*/

#strict

// Überladen
public func SetDir(int iDir)
{
  iDir = BoundBy(iDir, 0, 1);
  SetAction("Exist");
  SetPhase(iDir);
  SetSolidMask(50 * iDir, 0, 50, 30, 0, 0);
  return(1);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetDir(%d)", GetDir());

}
