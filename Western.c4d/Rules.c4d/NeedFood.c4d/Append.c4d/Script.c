/*-- Clonks --*/

#strict

#appendto CLNK

local iHunger, iThirst;

protected func Initialize()
{
  AddEffect("Life", this(), 1, 35, this());
  return(_inherited(...));
}

func IsEatOn() 
{
 if(!FindObject(NDFO) && !FindObject(NDWA)) return();
 return(!GetEffect(Format("IntNoGoEat%s", GetPlayerName(GetOwner())))); 
}

func IsEatOff() 
{ 
  if(!FindObject(NDFO) && !FindObject(NDWA)) return();
  return(GetEffect(Format("IntNoGoEat%s", GetPlayerName(GetOwner())))); 
}

func GetHungerString()
{
  var szText = "{{APTC}}|{{APTC}}|";
  if(FindObject(NDFO)) szText = Format("%s{{WNHN}}{{APTC}}%d%{{APTC}}{{APTC}}", szText, 100*iHunger*3/2/MaximumHunger());
  if(FindObject(NDWA)) szText = Format("%s{{WNTH}}{{APTC}}%d%", szText, 100*iThirst*3/2/MaximumThirst());
  return(szText);
}

private func MaximumThirst() { return(3000); }
private func MaximumHunger() { return(5000); }

func FxLifeStart(pTarget, iEffectNumber, iTemp, var1, var2, var3, var4)
{
  // Ein weiterer Hungereffekt kann nurnoch zu weiteren Bugs führen, lieber nicht anwenden
  if(!iTemp) if(GetEffectCount("Life", pTarget) > 1) return(-1);
  return FX_OK;
}

func FxLifeTimer()
{
  // Keine Regeln? dann gleich abbrechen    
  if(!FindObject(NDFO) && !FindObject(NDWA)) return();
    
  // Hunger steigt
  if(FindObject(NDFO)) iHunger = BoundBy(iHunger+ObjectCount(NDFO),0,MaximumHunger());

  // Durst steigt
  if(FindObject(NDWA)) {
    // Im Wasser kann man trinken (man soll ja nicht ohne Schale neben einem See verdursten)      
    if(!Contained() && GetMaterial(0, 0) == Material("Water")) {
      if(iThirst>40*15) {
        Drink(0,40,1);
        ExtractMaterialAmount(0,0,Material("Water"), 12);
      }
      else
        iThirst=0;
    }
    else
      iThirst = BoundBy(iThirst+ObjectCount(NDWA),0,MaximumThirst());
  }

  CheckWarnings();

  var fControled = GetCursor(GetOwner()) == this();
  // Sehr starker Hunger: essen gehen
  if(iHunger > MaximumHunger()-MaximumHunger()/3)
    // Nur wenn nichts anderes zu tun
    if(!GetCommand())
      GoEat(fControled); // Nur wenn nicht gerade von Spieler gesteuert
  // Extremer Hunger: Energieverlust
  if(iHunger == MaximumHunger())
    DoEnergy(-5);

  // Sehr starker Durst: trinken gehen
  if(iThirst>MaximumThirst()-MaximumThirst()/3)
    // Nur wenn nichts anderes zu tun
    if(!GetCommand())
      GoDrink(fControled); // Nur wenn nicht gerade von Spieler gesteuert
  // Extremer Durst: Energieverlust
  if(iThirst == MaximumThirst())
    DoEnergy(-5);
}

func FxLifeStop(pTarget, iEffectNumber, iReason, fTemp)
{
  pTarget->RemWarning(WNHN);
  pTarget->RemWarning(WNTH);
  // Damit man nach der Reinkarnation noch weiter Hunger erhält
  if(iReason == 4) return(-1);
}


/* Status */

private func IsHungry() {
  if(LocalN("iHunger", this())>MaximumHunger()*2/3 && ObjectCount(NDFO)) return(GetAlive());
}

private func IsThirsty() {
  if(LocalN("iThirst", this())>MaximumThirst()*2/3 && ObjectCount(NDWA)) return(GetAlive());
}

private func GetReturnAndCall(object obj, bool fDrink) {
  if(Contained()) {
    if(GetID(Contained())==DYNM)
      SetCommand(this(), "Call", Contained(),0,0,0,"StartWork");
    else
      SetCommand(this(), "Enter",Contained());
  }
  else {
    if(GetProcedure() eq "PUSH")
      SetCommand(this(), "Grab",GetActionTarget());
    else
      SetCommand(this(), "MoveTo",0,GetX(),GetY());
  }

  if(fDrink) {
    if(Contained(obj))
      AddCommand(this(), "Put",Contained(obj),0,0,obj);
    else 
      AddCommand(this(), "Call",obj,GetX(obj),GetY(obj));
    AddCommand(this(),   "Wait",0,0,0,0,0,3*8);
    AddCommand(this(),   "Call",this(),0,0,0,0,"GoDrink");
  }
  else
    AddCommand(this(),   "Call",this(),0,0,0,0,"GoEat");
  AddCommand(this(),     "Get",obj);
  if(GetProcedure() eq "PUSH")
    AddCommand(this(),   "Ungrab");
}

private func CheckNoEat(bool fNoWalk) {
  // Keine Brücke abbrechen
  if(GetProcedure() eq "BRIDGE") return(1);
  if(fNoWalk == WNHN || fNoWalk == WNTH) return(0);
  // Nicht laufen
  if(fNoWalk || IsEatOff()) return(1);
}

private func GoEat(fNoWalk) {
  var pFood;
  if(GetType(fNoWalk) == C4V_C4Object) fNoWalk = 0;
  if (CheckNoEat(fNoWalk)) return();
  // Keine verfügbare Nahrung in Reichweite
  if (!(pFood=FindFood())) return(0);
  // Nahrung eingesammelt: essen
  if (Contained(pFood)==this())
    return(pFood->Eat(this()));
  // Nahrung einsammeln
  GetReturnAndCall(pFood, false);
}
// Keine Meldung ausgeben
func GoEatFailed () { return (1); }

private func GoDrink(fNoWalk) {
  var pFood;
  if(GetType(fNoWalk) == C4V_C4Object) fNoWalk = 0;
  if (CheckNoEat(fNoWalk)) return();
  // Keine verfügbare Nahrung in Reichweite
  if (!(pFood=FindDrink())) return(0);
  // Nahrung eingesammelt: essen
  if (Contained(pFood)==this())
    return(pFood->Drink(this()));
  // Nahrung einsammeln
  GetReturnAndCall(pFood, true);
}
// Keine Meldung ausgeben
func GoDrinkFailed () { return (1); }

private func PutBottleBack(pThis, iX, iY, pFood)
{
  if(!pFood) return();	
  AddCommand (this(),"Drop",pFood, iX, iY); 
}
// Keine Meldung ausgeben
func PutBottleBackFailed () { return (1); }
private func PutBottleBackContainer(pThis, iContainer, iFood)
{
  var pContainer = Object(iContainer);
  var pFood = Object(iFood);
  if(!pFood) return();	
  if(pContainer) AddCommand (this(),"Put",pContainer, 0, 0, pFood); 	
}
// Keine Meldung ausgeben
func PutBottleBackContainerFailed () { return (1); }


/* Essen und Trinken */  
  
public func Feed(iValue) {
  // Energie auffüllen
  DoEnergy(iValue);
  // Hunger stillen
  if(ObjectCount(NDFO))
// Damit kleine Clonks nicht durch das Essen möglichst kleiner Objekte satt werden
    LocalN("iHunger",this()) = BoundBy(LocalN("iHunger",this())-iValue*3*(200-GetCon())/5,0,MaximumHunger());
//    LocalN("iHunger",this()) = BoundBy(LocalN("iHunger", this())-iValue*60-iValue*60*(GetCon()-100)/100,0,MaximumHunger());
//    LocalN("iHunger", this())=BoundBy(LocalN("iHunger", this())-iValue*60-500+5*GetCon(),0,MaximumHunger());
  // Geräusch
  Sound("ClonkMunch");
}

public func Drink(object pDrink, int iValue, int fNoDrinkAction) {
  // Durst stillen
  if(ObjectCount(NDWA))
    LocalN("iThirst",this()) = BoundBy(LocalN("iThirst", this())-iValue*15-500+5*GetCon(),0,MaximumThirst());
  // sonst normale Aktion machen
  if(!fNoDrinkAction) return(_inherited(...));
  return(1);
}


/* Suchfunktionen */

private func FindFood() {
  var pFood;
  // Sucht zuerst nach Essen im Inventar oder in der Nähe. 
  // Da es sich nicht lohnt, Ungekochte Nahrung zu essen, wird es zuletzt genommen. 
  if(pFood = FindObject2(Sort_Reverse(Sort_Func("GetFeedValue")),Find_Func("GetFeedValue"),Find_Container(this()))) return(pFood);
  if(Contained(this()))
    if(pFood = FindObject2(Sort_Reverse(Sort_Func("GetFeedValue")),Find_Func("GetFeedValue"),Find_Container(Contained(this())))) return(pFood);
  if(pFood = FindObject2(Sort_Reverse(Sort_Func("GetFeedValue")),Find_Func("GetFeedValue"),Find_OCF(OCF_Available()),Find_InRect(-150,-60,300,120))) return(pFood);

  // Keine verfügbare Nahrung gefunden
  return(0);
}

private func FindDrink() {
  var pFood;
  // Sucht zuerst nach Wasser im Inventar oder in der Nähe. 
  if(pFood = FindObject2(Sort_Reverse(Sort_Func("GetDrinkValue")),Find_Func("GetDrinkValue"),Find_Container(this()),Find_Not(Find_Action("Potion")))) return(pFood);
  if(Contained(this()))
    if(pFood = FindObject2(Sort_Reverse(Sort_Func("GetDrinkValue")),Find_Func("GetDrinkValue"),Find_Container(Contained(this())),Find_Not(Find_Action("Potion")))) return(pFood);
  if(pFood = FindObject2(Sort_Reverse(Sort_Func("GetDrinkValue")),Find_Func("GetDrinkValue"),Find_OCF(OCF_Available()),Find_InRect(-150,-60,300,120),Find_Not(Find_Action("Potion")))) return(pFood);

  // Keine verfügbare "Nahrung" gefunden
  return(0);
}

/* Warnungen */

private func CheckWarnings(iWarmthChange) {
  var pActT, iX, iY;

  if(!GetAlive()) return();

  pActT=this();
  if(Contained())
  {
    pActT=Contained();
    iX = GetVertex(0, 0, pActT);
    iY = GetVertex(0, 1, pActT)-GetDefOffset(GetID(pActT), 1)-10;
  }

  // Hunger: Warnung
  if(IsHungry())    SetWarning(WNHN,pActT, iX, iY);
  else          RemWarning(WNHN);

  // Durst: Warnung
  if(IsThirsty())    SetWarning(WNTH,pActT,iX, iY);
  else          RemWarning(WNTH);
}

private func GetWarning(warning_id) {
  var pWarn, szWarnAct=DefinitionCall(warning_id,"WarnAction");
  while(pWarn=FindObject(warning_id,0,0,0,0,0,szWarnAct,0,0,pWarn))
    if(GetActionTarget(1,pWarn)==this())
      return(pWarn);
}

private func RemWarning(warning_id) {
  var pWarn=GetWarning(warning_id);
  if(!pWarn) return(0);
  RemoveObject(pWarn);
}

private func SetWarning(warning_id,pAct0, iX, iY) {
  var pWarn=GetWarning(warning_id), szWarnAct=DefinitionCall(warning_id,"WarnAction");
  if(!pWarn) return(ObjectSetAction(CreateObject(warning_id),szWarnAct,pAct0,this()));
  if(GetActionTarget(0,pWarn)!=pAct0)
  {  SetActionTargets(pAct0,this(),pWarn); SetVertexXY(0, iX, iY, pWarn); }
}

private func RemoveWarnings() {
  return( RemWarning(WNHN) || RemWarning(WNTH));
}

// Noch eine Extrafunktion, damit man dem Clonk gleich sagen kann, nach Nahrung usw. zu suchen.
func ContextEatMenu() {
  [$CtxEatMenu$|Image=MEAC|Condition=NeedFood]
  CreateMenu(MEAC,this(),0,0,"$CtxEatMenu$",0,1,1);
  if(FindObject(NDFO))
    AddMenuItem("$SatisfyHunger$","GoEat",  WNHN,this(),0,0,"");
  if(FindObject(NDWA))
    AddMenuItem("$SatisfyThirst$","GoDrink",WNTH,this(),0,0,"");
  if(GetEffect(Format("IntNoGoEat%s", GetPlayerName(GetOwner()))))
    AddMenuItem("$CtxGoEatSwitchOn$", "SwitchGoEat",RMET,this(),0,0,"");
  else
    AddMenuItem("$CtxGoEatSwitchOff$","SwitchGoEat",MEAC,this(),0,0,"");
}

func SwitchGoEat(iID)
{
  if(iID == RMET)
    RemoveEffect(Format("IntNoGoEat%s", GetPlayerName(GetOwner())), 0);
  if(iID == MEAC)
   if(!GetEffect(Format("IntNoGoEat%s", GetPlayerName(GetOwner())), 0))
    AddEffect(   Format("IntNoGoEat%s", GetPlayerName(GetOwner())), 0, 1, 0);
  CloseMenu(this());
  ContextEatMenu();
  SelectMenuItem(Min(1,ObjectCount(NDFO))+Min(1,ObjectCount(NDWA)),this());
}

func NeedFood() { return(ObjectCount(NDFO) || ObjectCount(NDWA)); }