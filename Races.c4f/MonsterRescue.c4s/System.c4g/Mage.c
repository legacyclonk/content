/*-- Magus --*/

#strict
#appendto MAGE

// Beim verlassen des Monsters sterben
protected func Departure() { return(Kill()); }
