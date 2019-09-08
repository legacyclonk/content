/*-- Orangene Bäume --*/

#strict

#appendto TREE

func Construction() 
{
  SetGraphics(0,0,GetID(),1,6); // Overlay mit selber Grafik drüberlegen
  SetObjectBlitMode(1,0,1); // und additiv machen
  SetClrModulation(RGB(255,80,0)); // Basisgrafik modulieren
  SetClrModulation(RGB(255,130,0),0,1); // Overlay modulieren  
  return(_inherited());
}
