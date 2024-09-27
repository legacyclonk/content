/* Vertex */

// This is a vertex of a directed graph
// The edges may have properties and only one edge may go from
// one vertex to another

#strict
// uses attach info...
#include L_AI

local pVertices;

protected func Initialize() {
	pVertices = CreateObject(L_OS);
}

protected func Destruction() {
	pVertices->RemoveObject();
}

// A vertex of a graph
public func IsVertex() { return(true); }

// add an edge. If fReconnect = true, there'll be an edge back
public func AddEdge(object pTarget, bool fReconnect) {
	// ein Pfad ohne Target ist nix
	if(!pTarget) return(false);

	// new edge
	// addItem returns false in an object set if item already
	// existant
	var ret = pVertices->AddItem(pTarget);

	if(fReconnect) {
		// same goes for the other side
		ret = (ret && pTarget->AddEdge(this()));
	}
	
	// will return false if the connection is already there
	return(ret);
}

// remove an edge
public func RemoveEdge(object pTarget, bool fBoth) {
	if(!pTarget) return(false);
	var ret = pVertices->RemoveItem(pTarget);
	ClearEdgeData(pTarget);
	
	if(fBoth) {
		ret = (ret || pTarget->RemoveEdge(this()));
	}
	// will return true if at least one edge has been removed
	return(ret);
}

// get a connected vertex
// attention: no guarantee that the order will stay...
// so dont safe the edge by number but rather by object 
public func GetVertex(int iNum) {
	return(pVertices->GetItem(iNum));
}

// count
public func GetEdgeCount() {
	return(pVertices->GetItemCount());
}


// sets the iPos data for the edge to pTarget
public func SetEdgeData(object pTarget, int iPos, data) {
	if(!pTarget) return(false);
	if(iPos < 0) return(false);
	
	return(SetInfo(pTarget,iPos, data));
}

// gets the iPos data for the edge to pTarget
public func GetEdgeData(object pTarget, int iPos) {
	if(!pTarget) return(false);
	if(iPos < 0) return(false);
	
	return(GetInfo(pTarget,iPos));
}

public func ClearEdgeData(object pTarget) {
	if(!pTarget) return(false);
	return(ClearInfo(pTarget));
}
