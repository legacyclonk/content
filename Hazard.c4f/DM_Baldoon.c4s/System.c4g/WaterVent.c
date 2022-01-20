/*-- Watervent --*/

#strict
#appendto GAT2

func StartWater() {
    AddEffect("Water",0,1,5,this());
}

func FxWaterTimer() {
    CastPXS("Acid",30,20,0,0);
}

func StopWater() {
    RemoveEffect("Water");
}
