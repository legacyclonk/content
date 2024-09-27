/*-- Hangartor --*/

#strict

local movement; // -1 = Schlieﬂen
                //  0 = Nichts tun
                //  1 = ÷ffnen

protected func Initialize()
{
  SetAction("Gate");
  SetDir(0);
}

public func Open() // Aufmachen
{ movement = 1; StartSound(); }

public func Close() // Zumachen
{ movement = -1; StartSound(); }

public func Stop() // Sofort stoppen
{ movement = 0; StopSound(); }

public func ConsoleControl(int i)
{
  if(i == 1) return("$Open$");
  if(i == 2) return("$Close$");
  if(i == 3) return("$Stop$");
}

public func ConsoleControlled(int i)
{
  if(i == 1 && Inside(GetDir(), 0, 14)) Open();
  if(i == 2 && Inside(GetDir(), 1, 15)) Close();
  if(i == 3 && Inside(GetDir(), 1, 14)) Stop();
}

protected func CheckMovement()
{
  CheckSolidMask();

  if(!movement) return();

  if(movement == -1)
    {
    if(!GetDir())
      { Stop(); }
    else
      SetDir(GetDir()-1);
    }
  if(movement ==  1)
    {
    if(GetDir() == 15)
      { Stop(); }
    else
      SetDir(GetDir()+1);
    }
}

private func StartSound() { Sound("Elevator", 0,0,0,0, 1); }
private func StopSound() { Sound("Elevator", 0,0,0,0, -1); Sound("Discharge"); }

private func CheckSolidMask()
{
  SetSolidMask(0, 8*GetDir(), 200, 8, 0, 0);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (GetDir()) {
		extra[GetLength(extra)] = Format("SetDir(%d)", GetDir());
		extra[GetLength(extra)] = "CheckSolidMask()";
	}
}
