/* Langzeitflamme */

#strict

protected func BurnProcess()
  {
  if (Random(4))
    if (GetActTime()>100) return(RemoveObject());
  if (!OnFire()) return(RemoveObject());
  return(1);
	}

protected func Completion()
  {
  Incinerate();
  SetAction("Burn");
  SetComDir(COMD_Down());
  return(1);
  }
