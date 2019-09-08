#strict

public func GetElementColor() { return(RGB(128,64,0)); }
public func IsSpellClass() { return(1); }
public func GetClassCombo() { return("2"); }
protected func Initialize() { return(SetObjectLayer(this())); }
protected func AttachTargetLost() { return(RemoveObject()); }
protected func MouseSelection(iByPlr) { return(GetActionTarget()->MouseSelection(this())); }
