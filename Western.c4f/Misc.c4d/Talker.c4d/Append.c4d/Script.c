/*-- ClonkAppend --*/

#strict

#appendto CLNK

local bTalking;
local pTalker;

public func SetTalking(i) { bTalking = i; }
public func GetTalking( ) { return(bTalking); }

public func SetTalker(i) { pTalker = i; }
public func GetTalker( ) { return(pTalker); }

public func ControlThrow(a,b,c,d,e,f,g,h,i,j)
{
  if(pTalker) LocalN("iWait", pTalker)=0;	
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlRight(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlLeft(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlDown(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlDig(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlDigSingle(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlDigDouble(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlUp(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlUpDouble(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlDown(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlDownSingle(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlDownDouble(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlSpecial(a,b,c,d,e,f,g,h,i,j)
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlSpecialDouble(a,b,c,d,e,f,g,h,i,j) // dies produziert einen unschönen, unnützen zusätzlichen kommando-button im viewport... :/
{
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

public func ControlSpecial2(a,b,c,d,e,f,g,h,i,j)
{
  [$CtrlMenuDesc$|Image=CXTX]
  if(bTalking) return(1);
  else return(_inherited(a,b,c,d,e,f,g,h,i,j));
}
