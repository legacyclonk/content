/*-- Light Effect --*/

#strict
#appendto CLGH

/*-- Deckenlicht --*/

func Initialize() {
  var disabled=0;
  if(!Random(10)) disabled=1;
  if(!disabled) {
  var light;
  light = AddLightAmbience(55);
  light->ChangeGraphics("Half");
  light->SetR(180);}
  if(disabled) AddEffect("RegainLight",this(),1,1,this());
  return(1);
}

global func Influence() {

}

func FxLooseLightTimer() {
    if(!Random(10)) {  //Influence(); 
                        while(FindObject(LIGH,-10,-10,20,20)) RemoveObject(FindObject(LIGH,-10,-10,20,20));
                        RemoveEffect("LooseLight",this());
                        
                        if(!Random(6)) {
                        Sound("Rip");
                        CastObjects(SPRK, 5, 30, RandomX(-3,3),4);
                        }
                        
                        AddEffect("RegainLight",this(),1,1,this());}
}

func FxRegainLightTimer() {
  if(Random(35)) return(0);
  var light;
  light = AddLightAmbience(55);
  light->ChangeGraphics("Half");
  light->SetR(180);
  AddEffect("LooseLight",this(),1,1,this());
  RemoveEffect("RegainLight",this());
  
}

public func IsInfluenced() { return(true); }
public func IsLamp() { return(true); }
