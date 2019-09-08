/*-- Fett --*/

#strict
#include MEAT

public func Eat(pEater1,pEater2) {
  if(pEater2) pEater2->~Feed(50);
  else if(pEater1) pEater1->~Feed(50);
  RemoveObject();
}

public func Rub(pRubber1,pRubber2) { // ;)
  if(pRubber2) pRubber2->~Rub(400);
  else if(pRubber1) pRubber1->~Rub(400);
  RemoveObject();
}

/* Köderqualität */  
  
public func IsBait() { return(100); }

/* Menü */

public func Activate(pClonk) {
  [$TxtApply$]
  // Clonk anhalten
  SetComDir(COMD_Stop(),pClonk);
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained())ne"Walk") return(0);
  if (Contained(Contained())) return(0);
  // Menü erzeugen
  CreateMenu(FAT1,pClonk,this());
  AddMenuItem("$TxtEat$","Eat",EATF,pClonk,0,pClonk);
  AddMenuItem("$TxtRubIn$","Rub",RUBF,pClonk,0,pClonk);
  return(1);
}
