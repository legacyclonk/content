/*-- Stalaktit --*/

#strict

local iPhase;
local pGrow;
local pChild;

func Initialize() {
  // Ein zufälliges Aussehen
  SetAction("Hanging");
  SetPhase(iPhase=Random(4));
}

// Ein neuer Stalaktit wird erzeugt
func Grow()
{
  SetObjDrawTransform(1000, 0, 0, 0, 0);
  pGrow=100;
}

func Check() {
  // Noch im Wachsen?	
  if(pGrow)
  {
    // In der Wachstumsphase wird er immer Größer	  
    pGrow-=5;
    SetObjDrawTransform(1000, 0, 0, 0, 10*(100-pGrow), -pGrow*190);
    return(1);
  }

  // Im Fallen?
  if(GetAction()S="Falling") return(Falling());
  
  var clnk;
  // Ausgewachsene hängende Stalaktite fallen vorzugsweise auf Clonks runter
  while(clnk=FindObject(0,-25,0,50,100,OCF_CrewMember(),0,0,0,clnk)) 
    if(GetAlive(clnk) && !Random(2)) {
      // Und wenn wir noch keinen Nachwuchs haben einen neuen Stalaktit heranwachsen lassen	    
      if(!pChild) (pChild = CreateObject(GetID(), 0, 19))->Grow();
      Sound("Earthquake");
      SetAction("Falling");
      SetPhase(iPhase);
      RemoveVertex(0);
      RemoveVertex(0);
    }
}

// Im Fallen Schaden machen
func Falling() {
  var clnk;
  while(clnk=FindObject(0,-7,-19,14,50,OCF_CrewMember(),0,0,0,clnk)) 
    DoEnergy(RandomX(-5,0),clnk);
}

// Am Boden in die Einzelteile zerfallen
func Hit() {
  var obj;
  Sound("RockBreak*");
  Sound("Blast2",0,0,50);
  // links oben
  ObjectSetAction(obj=CreateObject(_STP,-4,-4,-1),"Exist");
  SetPhase(iPhase*2,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // rechts oben
  ObjectSetAction(obj=CreateObject(_STP,3,-4,-1),"Exist");
  SetPhase(iPhase*2+1,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // unten
  ObjectSetAction(obj=CreateObject(_STP,0,12,-1),"LameExist");
  SetPhase(iPhase,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(10)-15,-Random(35)+10,obj);
	
  RemoveObject();
}
