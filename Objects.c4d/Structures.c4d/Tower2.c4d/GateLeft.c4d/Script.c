#strict

private func CheckRange()
{
  var y = GetY(GetActionTarget() );
  if(GetY() < y - 2)
    SetComDir(COMD_Down() );
  if(GetY() > y + 27)
    SetComDir(COMD_Up() );
  return(1);
}

private func DestructCheck()
{
  if(GetID(GetActionTarget()) != TWR2) RemoveObject();
}

public func Activate()
{
  Sound("Click");
  Sound("SteelGate1");
  if(GetComDir() == COMD_Up() ) return(Close() );
  return(Open() );
}

private func Close()
{
  if(GetY() > GetY(GetActionTarget()) + 20) return(0);
  SetComDir(COMD_Down() );
}

private func Open()
{
  if(GetY() < GetY(GetActionTarget()) - 2) return(0);
  SetComDir(COMD_Up() );
}

protected func Completion()
{
  SetComDir( COMD_Up() );
}

