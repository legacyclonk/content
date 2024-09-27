/* Object Set */

#strict
#include L_ST
#include L_AI

// This object is a object set. It contains only objects. It is faster than
// the generic set for object since it attaches info to all objects 
// that are in the set. It removes nullpointers automatically and immediately
// It is, again, a pseudo set because:
// * one has numbered access through GetItem(i) (no guarantee for the order, though)
//   (which is against the standard in other programming languages)
//
// A pointer to an object can not be in here twice - as opposed to the generic pseudo
// set


protected func Initialize() {
	// The array starts at 1 (with real values)
	// why: Because the info on each object which points to the position in the
	// array might point at the zero position. Since "info not set" == 0 in Clonk
	// this is a problem
	aItems = CreateArray(1);
	aItems[0] = -1;
}


// Is a set
public func IsSet() { return(true); }

// Add an item
public func AddItem(object item) {
	if(!item) return(false);
	// already in list? return false
	if(GetInfo(item,0)) return(false);

	var length = GetLength(aItems);
	aItems[length] = item;
	SetInfo(item,0,length);
	return(true);
}

// Remove an item
public func RemoveItem(object item, bool fRemove) {
	// to prevent nulls in the array
	var ret = ReorderArray(item);

	// delete info of the object that is out of the set
	ClearInfo(item);

	if(fRemove) item->RemoveObject();
	
	// will return false if item is not existant
	return(ret);
}

// Get iPos item of ID idObj
public func GetItemByID(id idObj, int iPos) {
	if(!idObj) return(false);
	if(iPos < 0) return(false);
	
	// search for objects of that id
	var item;
	for(var i=0; item = GetItem(i); ++i) {
		// found one of this id
		if(GetID(item) == idObj) {
			if(!iPos) return(item);
			else iPos--;
		}
	}
	// not found
	return(false);
}

// Remove all
public func Clear(bool fRemove) {
	// remove object infos
	var length = GetLength(aItems);
	for(var i=1; i<length ;++i) {
		ClearInfo(aItems[i]);
		if(fRemove) aItems[i]->RemoveObject();
	}
	
	aItems = CreateArray(1);
	aItems[0] = -1;
	return(true);
}

// get item
public func GetItem(int i) {
	if(i < 0) return( false );
	// see Initialize for reason. array starts at 1
	if(i >= GetLength(aItems)-1) return(false);
	return(aItems[i+1]);
}

// automatically remove objects out of set on removal
public func FxIntInfoStop(object pObj, int iNum, int iReason) {
	// if object is removed
	if(iReason == 3) {
		this()->ReorderArray(0,EffectVar(0,pObj,iNum));
	}
}

// count. If id is given, count objects of that ID
public func GetItemCount(id idObj) {
	if(!idObj) return(GetLength(aItems)-1);

	var sum = 0;
	// search for objects of that id
	var length = GetLength(aItems);
	for(var i=1; i<length ;++i) {
		if(GetID(aItems[i]) == idObj) sum++;
	}
	return(sum);
}

private func GetItemPosition(item) {
	// if object its faster..
	// since there can be only one pointer to the same object
	// there is no second parameter
	if(!item) return(-1);
	var info = GetInfo(item,0);
	if(!info) return(-1);
	return(info);
}

private func ReorderArray(deleteditem, int num) {
	if(!num) {
		num = GetItemPosition(deleteditem);
		if(num == -1) return(false);
	}

	var last = GetLength(aItems)-1;
	// overwrite this entry with the last one
	aItems[num] = aItems[last];
	SetLength(aItems,last);
	// ... will keep the array small and without nulls...
	
	// change info in the switched object if it was not the last one
	if(num != last) {
		SetInfo(aItems[num],0,num);
	}
	return(true);
}
