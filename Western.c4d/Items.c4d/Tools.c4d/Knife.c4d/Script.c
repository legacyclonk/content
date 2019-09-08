#strict

protected func Hit() { Sound("WoodHit*"); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann vom Trapper in Handarbeit hergestellt werden */
public func IsTrapperHandcraft() { return(1); }
