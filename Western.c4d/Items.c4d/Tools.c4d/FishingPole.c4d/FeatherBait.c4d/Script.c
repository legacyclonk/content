/*-- Köderfedern --*/

#strict 2

#include METB

/* Köderqualität */  
  
//public func IsBait() { return(45); }
public func IsBait() { return(20); }
public func BaitRecyclable() { return(1); }

/* Action-Call */

public func IdleBait() { return(SetAction("Idle")); }

/* Kann vom Trapper in Handarbeit hergestellt werden */
public func IsTrapperHandcraft() { return(1); }