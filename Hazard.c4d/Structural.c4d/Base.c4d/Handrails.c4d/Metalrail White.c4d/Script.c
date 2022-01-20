/*-- Metalrail --*/

#strict

#include RAI1

/* Status */


public func Index(int index) {
	if(index == 0) return("End1");
	if(index == 1) return("Linkage");
	if(index == 2) return("Backing");
	if(index == 3) return("End2");
}
	
public func Left()		{ return(0); }
public func Right()	{ return(3); }
public func Linkage()	{ return(1); }
public func Backing()	{ return(2); }