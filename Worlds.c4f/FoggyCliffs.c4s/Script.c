#strict

func Initialize()
{
  var iTimeout,pWipf;
  SetGamma(RGB(15,15,15),RGB(118,118,118),RGB(215,215,215));
  SetSkyParallax(0,15,17,0,0,0,0); 
  //Handgemacht... ;)
  Meeresrauschen(170);
  Meeresrauschen(1100);
  Meeresrauschen(2060);
  Meeresrauschen(2900);
  Meeresrauschen(3800);
  Meeresrauschen(4800);
  Meeresrauschen(5700);
  Moewen(900);
  Moewen(2300);
  Moewen(3900);
  Moewen(5000);
  //Nebel platzieren
  for(var i;i<260;++i) 
    CreateParticle("Fog",Random(LandscapeWidth()),Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500));
  //Wipfe platzieren
  while(pWipf=FindObject(WIPF,0,0,0,0,0,0,0,0,pWipf)) {
    iTimeout=0;
    while(Stuck(pWipf)||GetMaterial(GetX(pWipf),GetY(pWipf))!=Material("Tunnel")) {
      SetPosition(Random(LandscapeWidth()),RandomX(LandscapeHeight()*2/3,LandscapeHeight()),pWipf);
      if(iTimeout++ > 50000) { RemoveObject(pWipf); break; }
    }
  }  
  ScriptGo(1);
}

func Script0()
{
  CreateParticle("Fog",0,Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500)); 
}

func Script4()
{
  return(goto(0));
}

//Sound-Objekte platzieren
func Meeresrauschen()
{
  ObjectSetAction(CreateObject(SL9Z,Par(),LandscapeHeight()/2,-1),"Meer");
}
  
func Moewen()
{
  ObjectSetAction(CreateObject(SL9Z,Par(),LandscapeHeight()/3,-1),"Moewen");
}
