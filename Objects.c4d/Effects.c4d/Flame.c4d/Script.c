/* Flamme */

#strict

protected func BurnProcess()
  {
  if (!OnFire()) return(RemoveObject());
  var i;
  for (var i = -5; i <= 10; ++i) if (FlameConsumeMaterial (0, i)) break;
  // Damit die Flamme nicht unten festhängt
  if (i == -5)
    {
    SetPosition (GetX(), GetY() - 1);
    }
  // Kein Öl mehr
  if (i > 10)
    {
    SetPosition (GetX(), GetY() - 1);
    return (DoCon (-5));
    }
  FlameConsumeMaterial (0,8);
  // zu viele FLAMs
  if (FindObject (FLAM, 0, -5))
    {
    SetPosition (GetX(), GetY() - 1);
    return (DoCon (-5));
    }
  // Noch mehr FLAMs erzeugen
  if (!FindObject (FLAM, -4, -9, 8, 18))
    {
    i = CreateObject(FLAM, Random(7)-3, 8 * GetCon () / 100, -1);
    SetYDir (GetYDir (0, 1000), i, 1000);
    }
  DoCon (5);
  SetXDir(Random(31)-15);
  return(1);
  }

protected func Completion()
  {
  Incinerate();
  SetAction("Burn");
  return(1);
  }
