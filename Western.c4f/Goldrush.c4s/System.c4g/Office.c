/* Büro des Sheriffs */

#strict
#appendto SHOF

// Kann nicht benutzt werden
protected func ContainedUp() { return(false); }
protected func ContainedDig() { return(false); }
protected func ContextShowPrisoners() { [|Condition=Never] return(false); }
protected func ContextRecruitSheriff() { [|Condition=Never] return(false); }
protected func Never() { return(false); }