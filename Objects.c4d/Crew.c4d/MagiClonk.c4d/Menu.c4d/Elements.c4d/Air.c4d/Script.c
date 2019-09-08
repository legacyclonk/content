#strict

public func GetElementColor() { return(RGBa(187,187,221)); }
public func GetClassCombo() { return("5"); }
public func IsSpellClass() { return(1); }
protected func Initialize() { return(SetObjectLayer(this())); }
protected func AttachTargetLost() { return(RemoveObject()); }
protected func MouseSelection(iByPlr) { return(GetActionTarget()->MouseSelection(this())); }
