/*-- Geschützsteuerung --*/

#strict

local on;

public func ConsoleControl(int i)
{
  if(i == 1)
  {
    if(on) return("$TurnOff$");
    else return("$TurnOn$");
  }
}

public func ConsoleControlled()
{
  for(var gun in FindObjects(Find_ID(SEGU)))
    gun->ConsoleControlled(1);
  on = !on;
}
