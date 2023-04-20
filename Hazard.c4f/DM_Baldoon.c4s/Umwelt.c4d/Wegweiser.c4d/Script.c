/*-- Wegweiser --*/

#strict
//#include BAS1

local count;

protected func Initialize() {
    count=70;
return(1);
}
  
public func ControlDigDouble() {
  Message("$Restricted$",this());
  //AddEffect("RingTrick",this(),1,1,this());
return(1);
}

func FxRingTrickTimer() {
  count--;
  if(count<1) return(Explode(39,this()));
  }
