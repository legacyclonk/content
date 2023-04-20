/*-- Granatenwerfer --*/


#strict
#appendto GLWP

public func IsUpgradeable(id uid) { return(false); }

func FMData1(int data) {
  if(data == FM_Condition)
    return(process <= 8);
  return(_inherited(data));
}

func FMData2(int data) {
  if(data == FM_Condition)
    return(process > 8);
  return(_inherited(data));
}

func FMData5(int data) {
	if(data == FM_Condition)
    return(process >= 14);
  
  return(_inherited(data));
}

func FMData3(int data) { if(data == FM_Condition) return(false); return(_inherited(data)); }
func FMData4(int data) { if(data == FM_Condition) return(false); return(_inherited(data)); }
func FMData6(int data) { if(data == FM_Condition) return(false); return(_inherited(data)); }
