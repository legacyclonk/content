#strict

protected func SeaSound() {
	if(GetAction()S="Seagulls") if(!Random(50))
		return(SetAction("NoAction"));
	if(GetAction()ne"Sea"|| GetCommand()) return();
	if(!Random(50)) return(SetCommand(this(),"Wait",0,0,0,0,RandomX(40,80),1));
	if(!Random(2)) return(SetCommand(this(),"Wait",0,0,0,0,RandomX(2,4),1));
 	Sound(Format("Wave%d",Random(4)+2));
	}