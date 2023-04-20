/*-- Projector --*/

#strict

local Active;
local light;

func Initialize() 
{
  SetAction("Turn");
  Active = 1;
  light = AddLightCone(100,RGBa(100,220,255,20));
  light->ChangeSizeXY(1000,300);
  light->ChangeOffset(25,-16);
}

public func ConsoleControl(int i)
{
    if(i == 1)
    {
      if(Active) return("$TurnOff$");
      else
          return("$TurnOn$");
    }
}

public func ConsoleControlled(int i)
{
    if(i == 1)
    {
        if(Active)
        {
          Active = 0;
          SetAction("TurnOff");
        }
        else
        {
          Active = 1;
          SetAction("TurnOn");
        }
    }
}

public func ActionOn()
{
  Active = 1;
  if(light)
    light->TurnOn();
}

public func ActionOff()
{
  Active = 0;
  if(light)
    light->TurnOff();
}

public func IsMachine() { return(1); }
public func EMPShock() { return(ActionOff()); }

/* Serialisierung */

public func Serialize(array& extra)
{
	if (!Active)
		extra[GetLength(extra)] = "ActionOff()";
}
