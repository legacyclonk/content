/*--- Signal ---*/

#strict

/* Initialisierung */

protected func Initialize()
{
  SetAction("Signal");
  SetDir();
}

/* Steuerung */

public func ControlDownSingle()
{
  [$TxtThrough$|Image=SG01:0]
  Sound("SignalClick");
  SetPhase(0);
}

public func ControlUp()
{
  [$TxtHaltandgo$|Image=SG01:1]
  Sound("SignalClick");
  SetPhase(3);
}

public func ControlLeft()
{
  [$TxtHaltandturnleft$|Image=SG01:2]
  Sound("SignalClick");
  SetPhase(2);
}

public func ControlRight()
{
  [$TxtHaltandturnright$|Image=SG01:3]
  Sound("SignalClick");
  SetPhase(1);
}

public func ControlThrow()
{
  [$TxtLoadunload$|Image=SG01:4]
  Sound("SignalClick");
  var dir = GetDir() + 1;
  SetDir(dir * (dir != 3) );
}

public func ControlDigDouble(object pClonk)
{
  [$TxtDeconstruct$|Image=SG01:5]
  Sound("SignalClick");
  CreateObject(CNKT,0,20,GetOwner(pClonk) );
  RemoveObject();
}

/* Kontext */

public func ContextDeconstruct(object pClonk)
{
  [$TxtDeconstruct$|Image=SG01:5|Condition=IsBuilt]
  AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlDigDouble");
  AddCommand(pClonk,"Grab",this());
}

public func ContextLoad(object pClonk)
{
  [$TxtLoadunload$|Image=SG01:4|Condition=IsBuilt]
  AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlThrow");
  AddCommand(pClonk,"Grab",this());
}

public func ContextStopGo(object pClonk)
{
  [$TxtHaltandgo$|Image=SG01:3|Condition=IsBuilt]
  AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlUp");  
  AddCommand(pClonk,"Grab",this());
}

public func ContextStopRight(object pClonk)
{
  [$TxtHaltandturnright$|Image=SG01:2|Condition=IsBuilt]
  AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlRight");  
  AddCommand(pClonk,"Grab",this());
}

public func ContextStopLeft(object pClonk)
{
  [$TxtHaltandturnleft$|Image=SG01:1|Condition=IsBuilt]
  AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlLeft");  
  AddCommand(pClonk,"Grab",this());
}

public func ContextThrough(object pClonk)
{
  [$TxtThrough$|Image=SG01:0|Condition=IsBuilt]
  AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlDownSingle");  
  AddCommand(pClonk,"Grab",this());
}

protected func IsBuilt() 
{
  return(GetCon() >= 100);
}

/* Materialtransport */

public func TrainArrive(object pTrain)
{
  var dir = GetDir();
  if(!dir) return(0);
  return(pTrain->~SignalCall(this(), --dir) );
}

public func TrainDepart(object pTrain)
{
  return(pTrain->~SignalDepart(this()) );
}
  
/* Forschung */  

public func GetResearchBase() { return(STMG); }
