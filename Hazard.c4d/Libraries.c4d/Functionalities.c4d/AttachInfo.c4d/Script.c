/* Attach information functionality */

#strict

// Attaches information to any object for exclusively this object
// So it is some personal information of one object that can be
// attached to any object. There can be an infinite number of infos

public func HasAttachInfo() { return(true); }

public func InfoName() { return("IntInfo"); }

// sets an info of that object for this object
public func SetInfo(object pObj, int iNum, info) {
	if(!pObj) return(false);

	// Info not existant yet
	var num = InfoNumber(pObj);
	if(num == -1) {
		// ...create
		num = AddEffect(InfoName(),pObj,1,0,this());
	}
	// set info
	EffectVar(iNum,pObj,num) = info;
	return(true);
}

// Clears ALL info of that object for this object
public func ClearInfo(object pObj) {
	if(!pObj) return(false);

	var num = InfoNumber(pObj);
	return(RemoveEffect(0,pObj,num));
}

// Get info of that object for this object
public func GetInfo(object pObj, int iNum) {
	if(!pObj) return(false);

	var num = InfoNumber(pObj);
	// info not existant: false
	if(num == -1) return(false);
	
	// else return info
	return( EffectVar(iNum,pObj,num) );
}

// get the effect number for the object
private func InfoNumber(object pObj) {
	if(!pObj) return(false);
	
	// search all similar effects
	var num;
	for(var i=0; num = GetEffect(InfoName(),pObj,i); ++i) {
		// and look if the target is this...
		var target = GetEffect(InfoName(),pObj,i,4);
		// found it
		if(target == this()) return(num);
	}
	// couldnt find it
	return(-1);
}
