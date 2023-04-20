/* Airstrike löst Szenarioscript aus */

#strict 2

#appendto AIRS

func Activate(pObject) {
	_inherited(pObject);

	if(!bombardment) bombardment = true;
}

func Destruction() {
	if(!bombardment)
		CreateObject(GetID(), 160, 250, -1);
	_inherited();
}
