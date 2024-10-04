/* Generic Pseudo Set */

#strict

// This object is a generic set. It contains a number of basically unordered
// variables. The difference to a "real" set as known in other programming 
// languagues is:
// * it may contain several same values 
// * with GetItem(i) one has numbered access (but no guarantee for any order)

// Attention: If pointers to objects are added and these objects are later
// removed, there will be dead entries in the pseudo set. For objects (only),
// use the object pseudo set

local aItems;

protected func Initialize() {
	aItems = CreateArray();
}

// Is a set
public func IsSet() { return(true); }

// is empty
public func IsEmpty() {	return(!GetItemCount()); }

// count
public func GetItemCount() { return(GetLength(aItems)); }

// Add an item
public func AddItem(item) {
	// note that it is possible to add 0, false, "" etc.
	// so dont iterate through this with while(GetItem(i)) ...

	var length = GetLength(aItems);
	aItems[length] = item;
	return(true);
}

// Remove an item
public func RemoveItem(item) {
	// to prevent nulls in the array
	return( ReorderArray(item) );
}

// Contains the item?
public func ContainsItem(item) {
	if(GetItemPosition(item) != -1) return(true);
	return(false);
}

// Remove all
public func Clear() {
	aItems = CreateArray();
	return(true);
}

// get item
public func GetItem(int i) {
	if(i < 0) return( false );
	if(i >= GetLength(aItems)-1) return(false);
	return(aItems[i]);
}

private func GetItemPosition(item, int iPos) {
	if(iPos < 0) return(-1);

	// gets iPos position...
	// .. there may be other positions too
	var length = GetLength(aItems);
	var num = 0;
	for(var i=0; i<length; ++i) {
		if(aItems[i] == item) {
			if(iPos == num) return(i);
			num++;
		}
	}
	return(-1);
}

private func ReorderArray(deleteditem) {
	// not in array.. fuck you
	var num = GetItemPosition(deleteditem);
	if(num == -1) return(false);

	var last = GetLength(aItems)-1;
	// overwrite this entry with the last one
	aItems[num] = aItems[last];
	SetLength(aItems,last);
	// ... will keep the array small and without nulls...
	return(true);
}
