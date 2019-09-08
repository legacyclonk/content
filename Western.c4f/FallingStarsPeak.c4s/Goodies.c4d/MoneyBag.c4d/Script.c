/*-- Geldsack --*/

#strict

local m_val;
local iTimer;

protected func Initialize()
{
  m_val = 15;	
}

protected func Hit()
{
//  iTimer = 15;
  AddEffect("Disappear", this(), 1, 35, 0, 0, 20);
}

protected func Timer()
{
  if(iTimer)
  {
    iTimer--;
    if(!iTimer) return(RemoveObject());
    SetClrModulation(RGBa(255,255,255, 255-iTimer*255/15));
  }
  CastParticles("FSpark", 10, 30, 0, 0, 20, 30, RGB(255,255), RGB(255,255), this());
}

// Shows the value
public func Activate(object pClonk)
{
  Message("{{GOLD}} %d", pClonk, m_val);
  return(1);
}
public func ControlDigDouble(object pClonk)
{ return(Activate(pClonk)); }

// Calculates the value
protected func CalcValue()
{
    return(m_val);
}
// Set the exact value
public func SetValue(int iVal)
{
    m_val = iVal;
}
// Add to the value
public func DoValue(int iVal)
{
    m_val += iVal;
}
