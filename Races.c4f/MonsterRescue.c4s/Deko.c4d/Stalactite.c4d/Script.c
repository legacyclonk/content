/*-- Stalaktit --*/

#strict

local phase, y;

local pGrow;

func Initialize() {
  y = GetY()+38;
  SetAction("Hanging");
  SetPhase(phase=Random(4));
}

func Grow()
{
  SetObjDrawTransform(1000, 0, 0, 0, 0);
  pGrow=100;
}

func Check() {
  var clnk;
  if(pGrow)
  {
    pGrow-=5;
    SetObjDrawTransform(1000, 0, 0, 0, 10*(100-pGrow), -pGrow*190);
    return(1);
  }
  if(GetAction()S="Falling") return(Falling());
    
  y = GetY();
  
  while(clnk=FindObject(0,-25,0,50,100,OCF_CrewMember(),0,0,0,clnk)) 
    if(GetAlive(clnk) && !Random(2)) { 
      CreateObject(GetID(), 0, 19)->Grow();
      Sound("Earthquake"); 
      SetAction("Falling");
      SetPhase(phase);
      RemoveVertex(0);
      RemoveVertex(0);
    }
}

func Falling() {
  var clnk;
  while(clnk=FindObject(0,-7,-19,14,50,OCF_CrewMember(),0,0,0,clnk)) 
    DoEnergy(RandomX(-5,0),clnk);
}

func Hit() {
  var obj;
//  if(GetAction()ne"Falling") return(1);
  Sound("RockBreak*");
  Sound("Blast2",0,0,50);
  // links oben
  ObjectSetAction(obj=CreateObject(_STP,-4,-4,-1),"Exist");
  SetPhase(phase*2,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // rechts oben
  ObjectSetAction(obj=CreateObject(_STP,3,-4,-1),"Exist");
  SetPhase(phase*2+1,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // unten
  ObjectSetAction(obj=CreateObject(_STP,0,12,-1),"LameExist");
  SetPhase(phase,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(10)-15,-Random(35)+10,obj);
	
  RemoveObject();
}
