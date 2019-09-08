/*-- Seehund --*/

#strict
#include ANIM

local pPossession,iXPos,iYPos,strPortrait,idPortrait,iHunger;

/* Initialisierung */

protected func Construction() {
  SetAction("Walk");
  SetComDir(COMD_Left());
  if(Random(2)) SetComDir(COMD_Right());
  iHunger=Random(100);
}

/* TimerCall */

private func Activity() {
  var pEnemy;

  // Nicht wenn besessen
  if (IsPossessed()) return(CheckPossession());
  // Hunger
  iHunger=Min(iHunger+1,350);

  // Kommando zurücksetzen/aktualisieren
  if(!Random(7)) SetCommand(this(),"None");  

  // Hängt fest: Robben können sich selbst freigraben
  if(Stuck()) DigFree(GetX(),GetY()-5,10);

  // Feind in Sicht: abhauen
  if(pEnemy=EnemyNearby())
    return(RunAway(pEnemy));

  // Liegen: mal aufstehen
  if(!Random(10))
    if(GetAction()eq"Sleep") 
      return(SetAction("Walk"));

  // Gehen: hinlegen (und wieder normales Verhalten)  
  if(!Random(3))
    if(!GetCommand())
      if(GetAction()eq"Walk") 
        return(SetAction("Sleep"),ResetPhysical());
   
  // Gehen oder Schwimmen: Richtung ändern
  if(!Random(5))
    if(!GetCommand())
      if(GetAction()eq"Walk" || GetAction()eq"Swim") 
        return(ChangeDirection());

  // Hunger: Nahrungssuche
  if(iHunger>300)
    if(FoodSearch())
      return(1);

  // Im Wasser und nichts zu tun: zurück an Land
  if(InLiquid())
    if(!GetCommand())
      return(ReturnToLand());

  // Fortpflanzung (durch Umweltsteuerungsobjekt)
  //if (!Random(800)) Reproduction();
}

/* Kontakt */

protected func ContactLeft() {
  // Nicht wenn besessen
  if(IsPossessed()) return(1);
  // Kein Kommando: wenden
  if(!GetCommand())
    if(GetDir()==DIR_Left())
      if(GetAction()eq"Walk" || GetAction()eq"Swim") 
        return(TurnRight());
  // Bewegungskommando: an Land springen
  if (GetAction()eq"Swim")
    if (!GBackSemiSolid(0,-15))
      return(JumpToCoast());
}

protected func ContactRight() {
  // Nicht wenn besessen
  if (IsPossessed()) return(1);
  // Kein Kommando: wenden
  if (!GetCommand())
    if (GetDir()==DIR_Right())
      if(GetAction()eq"Walk" || GetAction()eq"Swim") 
        return(TurnLeft());
  // Bewegungskommando: an Land springen
  if (GetAction()eq"Swim")
    if (!GBackSemiSolid(0,-15))
      return(JumpToCoast());
}

protected func ContactTop() {
  // Nicht wenn besessen
  if (IsPossessed()) return(1);
  // Abtauchen
  if (GetAction()eq"Swim") SetYDir(+15);
  return(1);
}

protected func ContactBottom() {
  // Nicht wenn besessen
  if (IsPossessed()) return(1);
  // Auftauchen
  if (GetAction()eq"Swim") SetComDir(COMD_Up());
  return(1);
}

/* Aufnahme */

protected func RejectCollect(idType,pObj) {
  // Nicht besessen: keine Aufnahme
  if(!IsPossessed()) return(1);
  // Nur im Kopfbereich
  // if(!Inside(GetX(pObj)-GetX()-12+24*GetDir(),-5,+5)) return(1);
  // Nur Perlen
  if(idType!=QPRL) return(1);
  // Aufname erlauben
  return(0);
}

protected func Collection(pObj) {
  Exit(pObj);
  Enter(pPossession,pObj);
}

/* Eingang */

protected func ActivateEntrance(pObj) {
  // Nichts von außen
  if(Contained(pObj)!=this()) return(0);
  // Von innen nur Gegenstände
  if(GetOCF(pObj) & OCF_CrewMember()) return(0);
  // Objekt auswerfen
  Exit(pObj);
}
  
/* Bewegung */

private func TurnRight() {
  // Hängt fest: nichts machen
  if (Stuck()) return(0);
  // Dreh-Aktion
  if(GetDir()!=DIR_Right()) {
    if(GetAction()eq"Walk") SetAction("Turn");
    else if(GetAction()eq"Swim") SetAction("SwimTurn");
    else return 1;
  }
  // Richtung setzen
  SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

private func TurnLeft() {
  // Hängt fest: nichts machen
  if (Stuck()) return(0);
  // Dreh-Aktion
  if(GetDir()!=DIR_Left()) {
    if(GetAction()eq"Walk") SetAction("Turn");
    else if(GetAction()eq"Swim") SetAction("SwimTurn");
    else return 1;
  }
  // Richtung setzen
  SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

private func ChangeDirection() {
  if(GetDir()==DIR_Left()) return(TurnRight());
  if(GetDir()==DIR_Right()) return(TurnLeft());
}

/* Aktionen von außerhalb */

public func Feed(iVal) {
  DoEnergy(iVal);
  iHunger-=iVal;
  Sound("SealEat");
}
            
/* Lebensweg */

protected func Death() {
  LeavePossession();
  ChangeDef(DSEL);
  SetAction("Dead");
}

/* Suchfunktionen */

private func EnemyNearby() {
  var pEnemy=FindObject(0,0,0,-1,-1, OCF_CrewMember());
  var iDist=ObjectDistance(pEnemy);
  var pContainer=Contained(pEnemy);
  if(pContainer) if(GetID(pContainer)==SEAL) return();
  if(pEnemy)
    if(iDist<350)
      if(!Contained(pEnemy) || iDist<150)
        if(PathFree(GetX(),GetY(),GetX(pEnemy),GetY(pEnemy)) || iDist<100 )
          return(pEnemy);
}
  
private func FindFood() {
  var pFood;
  // Fleisch
  if(pFood=FindObject(MEAT,0,0,-1,-1,0,0,0,NoContainer())) 
    if(Distance(GetX(),GetY(),GetX(pFood),GetY(pFood))<500)
      return(pFood);
  // Fett
  if(pFood=FindObject(FAT1,0,0,-1,-1,0,0,0,NoContainer())) 
    if(Distance(GetX(),GetY(),GetX(pFood),GetY(pFood))<500)
      return(pFood);
  // Tote Fische
  if(pFood=FindObject(DFSH,0,0,-1,-1,0,0,0,NoContainer()))
    if(Distance(GetX(),GetY(),GetX(pFood),GetY(pFood))<500)
      return(pFood);
  // Fische
  if(pFood=FindObject(FISH,0,0,-1,-1,0,0,0,NoContainer())) 
    if(Distance(GetX(),GetY(),GetX(pFood),GetY(pFood))<500)
      return(pFood);
}
  
private func FindSolidX(iX,iY) {
  for(var i;i<30;++i) {
    if(Inside(iX-15*i,0,LandscapeWidth()-1))
      if(GBackSolid(iX-15*i,iY))
        return(iX-15*i); 
    if(Inside(iX+15*i,0,LandscapeWidth()-1))
      if(GBackSolid(iX+15*i,iY))
        return(iX+15*i);
  }   
  return(iX);
}
  
/* Verhaltenssteuerung */
  
private func RunAway(pEnemy) { // C4Object *pEnemy
  // Aufstehen
  if(GetAction()eq"Sleep") SetAction("Walk");
  // Nahrungssuche einstellen
  if (GetCommand()eq"Follow") SetCommand(this(),"None");
  // In die entgegengesetzte Richtung
  if(GetX(pEnemy)>GetX()) TurnLeft();  
  if(GetX(pEnemy)<GetX()) TurnRight();
  // Bewegungskommando
  SetCommand(this(),"None");
  AddCommand(this(),"MoveTo",0,GetX()+100*(GetDir()*2-1),GetY(),0,20);
  // Höhere Fluchtgeschwindigkeit
  SetPhysical("Walk",50000+Random(20000),PHYS_Temporary);
  // Geräusch
  if(!InLiquid()) if(!Random(2)) Sound("Seal*.wav");
}
        
private func FoodSearch() {
  var pFood=FindFood();
  // Fisch suchen
  if (!pFood) return(0);
  // Schläft: aufstehen
  if (GetAction()eq"Sleep") SetAction("Walk");
  // Ziel in Reichweite: fressen
  if (Inside(GetX()-GetX(pFood),-25,+25) && Inside(GetY()-GetY(pFood),-15,+15))
    return( pFood->Eat(this()), SetCommand(this(),"None") );
  // Führt gerade ein Kommando aus
  if (GetCommand()) return(1);
  // Fisch jagen
  return(SetCommand(this(),"Follow",pFood));
}
  
private func ReturnToLand() {
  // Aufgetaucht (aber noch im Wasser): an den nächstgelegenen Strand
  if(!GBackSemiSolid(0,-15))
    return(SetCommand(this(),"MoveTo",0,GetX()+FindSolidX(0,-15),GetY()-15));  
  // Auftauchen
  SetCommand(this(),"None");
  return(AddCommand(this(),"MoveTo",0,GetX(),GetY()-1000,0,20));
}
  
private func JumpToCoast() {
  // Nicht am Kartenrand
  if (!Inside(GetX(),20,LandscapeWidth()-20)) return(0);
  // An Land springen
  SetPosition(GetX(),GetY()-5);
  SetAction("Jump");  
  SetXDir(15*(GetDir()*2-1));
  SetYDir(-30);
  return(1);
}
  
/* Übernahme durch Clonk */

public func IsPossessed() { return(pPossession); }
  
private func CheckPossession() {
  if(!(GetAlive(pPossession) && Contained(pPossession)==this()) || GetBreath()<300) 
    return(LeavePossession());
  return(1);
}

public func TakePossession(pClonk) {
  if (IsPossessed() || GetBreath()<800) return(0);
  iXPos=GetX(pClonk);
  iYPos=GetY(pClonk);
  strPortrait= GetPortrait(pClonk);
  idPortrait=GetPortrait(pClonk,1);
  SetPortrait("random",pClonk,SEAL);
  Enter(this(),pClonk);
  Sound("Magic1");
  pPossession=pClonk;
  return(1);
}

public func LeavePossession() {
  var pContents;
  if(!IsPossessed()) return(0);
  Exit(pPossession,iXPos-GetX(),iYPos-GetY()+10);
  SetPortrait(strPortrait,pPossession,idPortrait);
  //while(pContents=Contents()) Exit(pContents,iXPos-GetX(),iYPos-GetY()+10);
  Sound("Magic1");
  pPossession=0;
  strPortrait=0;
  idPortrait=0;
}

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(Max(GetXDir() - 2, 0));
    else SetXDir(Min(GetXDir() + 2, 0));
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(Max(GetYDir() - 2, 0));
    else SetYDir(Min(GetYDir() + 2, 0));
  }
}

public func ContainedUpdate(object self, int comdir)
{
  SetComDir(comdir);
  ClearScheduleCall(this(), "ClearDir");
  if(comdir == COMD_Down || comdir == COMD_Up) ScheduleCall(this(), "ClearDir", 1, Abs(GetXDir()), true);
  if(comdir == COMD_Left || comdir == COMD_Right) ScheduleCall(this(), "ClearDir", 1, Abs(GetYDir()), false);
  return(1);
}

public func ContainedLeft() {
  [$TxtMovement$]
  if(GetAction()eq"Sleep") return(SetAction("Walk"));
  SetCommand(this(),"None");
  if (GetDir()==DIR_Right()) return(TurnLeft());
  SetComDir(COMD_Left());    
  return(1);
}

public func ContainedRight() {
  [$TxtMovement$]
  if(GetAction()eq"Sleep") return(SetAction("Walk"));
  SetCommand(this(),"None");
  if(GetDir()==DIR_Left()) return(TurnRight());
  SetComDir(COMD_Right());    
  return(1);
}
  
public func ContainedUp() {
  [$TxtMovement$]
  if (GetAction()eq"Sleep") return(SetAction("Walk"));
  SetCommand(this(),"None");
  if(!GBackSemiSolid(0,-5)) {
    if(GetAction()eq"Swim") return(JumpToCoast());
    return(Jump());
  }
  SetComDir(COMD_Up());    
  return(1);
}
  
public func ContainedDown() {
  [$TxtMovement$]
  SetCommand(this(),"None");
  SetComDir(COMD_Stop());    
  if(GetAction()eq"Swim") SetComDir(COMD_Down());
  return(1);  
}

public func ContainedDigDouble() {
  [$TxtLeave$]
  LeavePossession();
  return(1);
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
  // Bewegungskommando
  if(szCommand S= "MoveTo")
    return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
  return(0);
}

/* Wärme */

public func GetWarmth() { return(20); }

/* Status */

public func IsOccupied() { return(1); }
