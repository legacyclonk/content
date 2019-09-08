#strict

public func GetElementColor() { return(RGB(0,0,255)); }
public func IsSpellClass() { return(1); }
public func GetClassCombo() { return("6"); }
protected func Initialize() { return(SetObjectLayer(this())); }
protected func AttachTargetLost() { return(RemoveObject()); }
protected func MouseSelection(iByPlr) { return(GetActionTarget()->MouseSelection(this())); }
