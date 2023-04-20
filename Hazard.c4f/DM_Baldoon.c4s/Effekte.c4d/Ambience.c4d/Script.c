/*-- Ambience --*/

#strict

local iSky;
local iReset;
local bBusy;
static a;

func Initialize() {
  AddEffect("Raining",0,1,1,this());
  return(1);
}

func Sounding() {
    if(iSky<6) bBusy=0;
    if(iSky>5) {bBusy=1; iSky=iSky-iReset; if(!Random(256-iSky)) iSky=iSky=RandomX(192,224);}
    
    SetSkyAdjust(RGBa(255,255,255,iSky),RGB(255,255,255));
    
    if(!Random(350)) {
    
                    /*a=FindObjects(Find_Func("IsInfluenced"));
                    var count=0;
                    while(count<22) {a[count]->LooseLight(); count++;}*/
                    
                    if(bBusy) return(0);
                    iReset=RandomX(3,5);
                    Sound("Thunders*",1);
                    iSky=RandomX(208,240);
                    
                    }
}

func FxRainingTimer() {
    Sound("Rain",1);
}
