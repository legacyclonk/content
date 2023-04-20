/*-- TorAutomatik --*/

#strict
#appendto _HN2

static iCount;
static console1_busy;

func Initialize() {
    iCount=55;
    SetAction("GateOpen");
    return();
}

protected func Close()
{
  console1_busy=1;
  var pGate2=FindObject(HNG2,-300,-50,100,100);
  if(pGate2) pGate2->Close();
  light1->TurnOn();
  light2->TurnOn();
  light3->TurnOn();
  vent1->StartWater();
  vent2->StartWater();
  Sound("Alarm");
  AddEffect("CloseAutomatic",0,1,35,this());

  return(_inherited());
}

func FxCloseAutomaticTimer() {
    var pGate2=FindObject(HNG2,-300,-50,100,100);
    //Message("%d",0,iCount);
    iCount--;
    if( iCount==20)     {
                        CreateObject(_DRA,-120,50,-1); 
                        vent1->StopWater(); 
                        vent2->StopWater();
                        light1->TurnOff();
                        light2->TurnOff();
                        light3->TurnOff();
                        }
    if(!iCount) {iCount=55; console1_busy=0; pGate2->Open(); SetAction("GateOpen"); return(RemoveEffect("CloseAutomatic"));}
}
