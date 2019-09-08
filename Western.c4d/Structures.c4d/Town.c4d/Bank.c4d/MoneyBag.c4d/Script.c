/*-- Geldsack --*/

#strict

local m_val;

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
