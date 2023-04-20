/*-- Use Jetpack --*/

#strict 2

/* Der Clonk kann das Jetpack mit dieser Funktionalität benutzen
     Dazu in ControlUpDouble, ControlRightDouble und ControlLeftDouble
     jeweils die Funktion
	 if(ControlJetpack("ControlUpDouble")) return(1);
     aufrufen (hier am Beispiel von ControlUpDouble).
	 
     Damit während des JetpackFlight geschossen werden kann muss in ReadyToFire()
     und AimOverride() am Ende return(_inherited()); aufgerufen werden.
*/

public func HasJetpack()
{
  return this->HasGear(0,JTPK);
}

protected func ControlJetpack(string strControl) {

	var jetpack = this->HasJetpack();
	if(!jetpack) return false;
	
	var ret = false;
	
	// up double
	if(strControl == "ControlUpDouble") {
		ret = jetpack->ControlUpDouble(this());
	}
	
	// right double
	if(strControl == "ControlRightDouble") {
		ret = jetpack->ControlRightDouble(this());
	}
	
	// left double
	if(strControl == "ControlLeftDouble") {
		ret = jetpack->ControlLeftDouble(this());
	}
	
	return ret;
}

/* Overloads */

private func IsJumping() {
  if(WildcardMatch(GetAction(),"JetpackFlight*"))
		return true;
	return _inherited();
}

public func ReadyToFire() {
 
	if(GetAction() == "JetpackFlight") return true;
 
	return _inherited();
}

public func AimOverride() {
	if(GetAction() == "JetpackFlight") return true;
	
	return _inherited();
}