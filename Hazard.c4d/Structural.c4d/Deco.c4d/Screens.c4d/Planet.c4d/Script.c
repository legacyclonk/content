/*-- Radarschirm --*/

#strict

local bc, cc;

func Initialize() {
  Start();
  AddFrame();
}

func Start() {
  SetAction("LoadPlanet");
  bc = 0;
  cc = 0;
}

func BlinkPlanet() {
  if(++bc > 2) {
    bc = 0;
    SetAction("LoadEurope");
  }
}

func BlinkEurope() {
  if(++bc > 2) {
    bc = 0;
    SetAction("LoadMidEurope");
  }
}

func Credits() {
  if(cc == 0) SetAction("Matthi");
  if(cc == 1) SetAction("Newton");
  if(cc == 2) SetAction("Boni");
  if(cc == 3) SetAction("Clonko");
  if(cc == 4) SetAction("Raven");
  
  if(cc == 5) Start();
  else        ++cc;
}
