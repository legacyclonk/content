#strict

protected func Hit() { Sound("MetalHit*"); }

protected func Activate(object pByObj) {

  // create menu
  CreateMenu(GetID(),pByObj,this(),4,"$NothingUpgradeable$",0,0,0);

  for(var pObj in GetUpgradeable()) {
    var szName = Format("$Upgrade$",GetName(pObj));
    // fill the menu with menuitems
    AddMenuItem(szName,"UpgradeObj",GetID(pObj),pByObj,0,pObj,pObj->IsUpgradeable(GetID()));
  }
  return(1);
}

public func GetUpgradeable() {
  var result = CreateArray();

  for(var i = 0; i < ContentsCount(0,Contained()); ++i) {
      var pObj = Contents(i,Contained());
      if(pObj->~IsUpgradeable(GetID()))
		result[GetLength(result)] = pObj;
  }
  for(var pObj in FindObjects(Find_Category(C4D_Vehicle),Find_AtPoint(0,-1))) {
	if(pObj->~IsUpgradeable(GetID()))
		result[GetLength(result)] = pObj;
  }
  
  return(result);
}

protected func UpgradeObj(id idObj, object pObj) {
  UpgradeObject(pObj);
}

public func UpgradeObject(object pObj) {
  if(pObj->~Upgrade(GetID())) {
    // Sound
    Sound("Connect");
    // only remove if Upgrade() of pObj returns true
    return(RemoveObject());
  }

}

// Status
public func IsUpgrade() { return(1); }

public func GetUpgradeGraphicsIDBit() { return 1; }