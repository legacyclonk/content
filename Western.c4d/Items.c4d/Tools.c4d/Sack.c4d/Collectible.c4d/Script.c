/*-- Sack (einsammelbar) --*/

#strict
#include SACK

protected func Activate(pClonk) {
  [$TxtTakeOut$]
  AddCommand(pClonk,"Get",this(),0,0,0,0,1);
  AddCommand(pClonk,"Grab",this());
  AddCommand(pClonk,"Drop");
  return(1);
}

public func ControlDigDouble() {}