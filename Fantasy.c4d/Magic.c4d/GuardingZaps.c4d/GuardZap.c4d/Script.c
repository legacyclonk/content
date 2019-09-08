/*-- Schutzzap --*/

#strict
#include ZAP1

local target,enemy;

private func Flying() { //wird alle 3 Frames aufgerufen
  var xdir,ydir;

  CreateParticle("PSpark",0,0,0,0,RandomX(20,80),RGBa(255,255,0,127));

  //nichts tun
  if (Random(6)) return();
    //Geschwindigkeit zufällig bestimmen
    xdir+=Random(13)-6;
    ydir+=Random(9)-4;
    if(enemy) {
      xdir+=BoundBy((GetX(enemy)-GetX()),-11,11);
      ydir+=BoundBy((GetY(enemy)-GetY())/3,-7,7);
      }
    else if(!Contained(target)) {
      xdir+=BoundBy((GetX(target)-GetX()),-18,18);
      ydir+=BoundBy((GetY(target)-GetY())/3,-12,12);
      }
  if(GBackLiquid(xdir,ydir)) return(SetSpeed());
  SetSpeed(xdir,ydir);
}

protected func Check() { //wird alle 18Frames aufgerufen
  var obj;

  //Verschüttet oder im Wasser?: Sterben
  if(GBackSemiSolid()) return(Death());
  //einfach so Sterben
  if(!Random(160)) return(Death());
  //Summen
  if(!Random(50)) Sound("Bsss");

  //Vom aktuellen Ziel ablassen und jemand anders verfolgen (falls jemand da ist)
  if(!Random(10)) enemy=FindEnemy(100);
  // Das Ziel hat sich in ein Gebäude gerettet
  if(Contained(target)) enemy=0; 
  //Vom Ziel ablassen wenn er sich im Wasser versteckt
  if(GBackSemiSolid(GetX(enemy)-GetX(),GetY(enemy)-GetY())) enemy=0;
  //Vom Ziel ablassen weil er nicht direkt erreichbar ist
  if(!Random(5)) if(!PathFree(GetX(),GetY(),GetX(enemy),GetY(enemy))) enemy=0; 
  //Vom Ziel ablassen weil der Zap sich zu weit vom Zauberer entfernt hat
  if(ObjectDistance(enemy,target)>250) enemy=0; 

  //stechen
  if(!Random(2))
    if(obj=FindObject(0,1,0,0,0,OCF_CrewMember(),0,0,NoContainer()))
      if(Hostile(GetOwner(target),GetOwner(obj)))
        if(obj!=target)
          Sting(obj);
}

public func FindEnemy(maxdist) {  //maximale Distanz zum "Opfer"
  var obj;
  //neues Ziel bestimmen falls es nicht zu weit vom Zauberer entfernt ist
  while(obj=FindObject(0,GetX(target)-GetX(),GetY(target)-GetY(),-1,-1,OCF_CrewMember(),0,0,NoContainer(),obj)) {
    // nicht den Zauberer und nur Feinde. Wenn NTMG aktiviert ist, auch Freunde
    if(!Hostile(GetOwner(target),GetOwner(obj)) && !ObjectCount(NTMG)) continue;
    if(obj==target) continue;

    if(ObjectDistance(obj,target)>maxdist) continue;
    if(GBackSemiSolid(GetX(obj)-GetX(),GetY(obj)-GetY())) continue;
    if(!PathFree(GetX(),GetY(),GetX(enemy),GetY(enemy))) continue;
    return(obj);
  }
}  
  

