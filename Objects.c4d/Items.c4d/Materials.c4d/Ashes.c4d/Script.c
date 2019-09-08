/*--- Asche ---*/

#strict

func Hit() {
	CastPXS("Ashes",100,20);
	RemoveObject();
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(60); }