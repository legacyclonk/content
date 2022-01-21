/* Generic ID bag */

#strict
#include L_AI

// This object contains a number of IDs.

// The IDs have no properties. For example the Hazard-Munition-System and the
// Alchemy-Ingredient-System uses these kind of ID bags.
// It is faster than storing a number of (icon)-objects in it but doesn't
// allow non-definition properties.

// Attention: The bag should only contain Items. All contents are considered
// as items.

// Is a bag
public func IsBag() { return(true); }

// Add one item
public func AddItem(id idItem) { return(AddItems(idItem,1)); }

// Delete on item
public func RemoveItem(id idItem) { return(RemoveItems(idItem,1)); }

// Add items
public func AddItems(id idItem, int iNum) {
	if(iNum < 0) return( false );

	var obj;
	// if no object yet...
	if(!(obj=FindContents(idItem))) {
		obj = CreateContents(idItem);
	}
	// still no object (id not existant)
	if(!obj) return(false);

	// increase..
	var newinfo = GetInfo(obj,0)+iNum;
	SetInfo(obj,0,newinfo);
	return (true);
}

// Delete items
public func RemoveItems(id idItem, int iNum) {
	if(iNum < 0) return(false);

	var obj;
	if(!(obj=FindContents(idItem)))
		return( false );

	// decrease..
	var newinfo = GetInfo(obj,0)-iNum;
	SetInfo(obj,0,newinfo);

	// delete ID object if empty
	if(GetInfo(obj,0) <= 0) {
		obj->RemoveObject();
	}

	return (true);
}

// Get item (like Contents only that each ID only comes one time)
public func GetItem(int i) {
	var c = Contents(i);
	if(!c) return(false);
	return(c->GetID());
}

// bag is empty
public func IsEmpty() {
	return(!Contents(0));
}

// Remove all of idItem. If idItem = 0, remove all
public func RemoveAllItems(id idItem) {
	// none to remove: return false
	if(IsEmpty()) return(false);

	// no id: remove all
	if(!idItem) {
		var obj;
		while(obj=Contents(0))
			obj->RemoveObject();
	}
	// id: remove id
	else {
		var obj;
		if(obj=FindContents(idItem))
			obj->RemoveObject();
		else
			return(false);
	}
	return (true);
}

// Get item count. If idItem = 0, get all
public func GetItemCount(id idItem) {
	// recursive add...
	if(!idItem) {
		var sum = 0;
		for(var i=0; Contents(i); ++i) {
			sum += GetItemCount(GetID(Contents(i)));
		}
		return (sum);
	}
	// number of the contents
	else {
		var obj = FindContents(idItem);
		if(!obj) return(0);
		
		return(GetInfo(obj,0));
	}
}

// Transfer items to another bag
// if iCount = 0, transfer all
// if bStrict, only transfer if iCount is not more than actually in bag
public func TransferItems(object pBag, id idItem, int iCount, bool bStrict) {
	// All parameters must be given 
	if(!pBag) return(false);
	if(!idItem) return(false);
	if(iCount < 0) return(false);

	var top = GetItemCount(idItem);
	// no sense in transfer if it is 0
	if(top == 0) return( true );

	// transfer all
	if(!iCount) {
		// just displace the object (no calls)
		//obj->Exit();
		//obj->Enter(pBag);
		// could produce problems with RejectEntrance etc...

		iCount = top;
	}
	// iCount is too big?
	if(iCount > top) {
		if(bStrict) {
			return(false);
		}
		else {
			iCount = top;
		}
	}

	// transfer...
	pBag->AddItems(idItem,iCount);
	RemoveItems(idItem,iCount);

	return(true);
}

// Transfer all items into another bag
public func TransferAllItems(object pBag) {
	if(!pBag) return(false);

	var obj;
	while(obj = Contents(0))
		TransferItems(obj, GetID(obj), 0);

	return(true);
}

// Destruction
// protected func Destruction() {
//	RemoveAllItems();
//}
// Contents are removed automatically