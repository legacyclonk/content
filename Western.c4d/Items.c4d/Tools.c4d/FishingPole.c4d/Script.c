/*-- Angel --*/

#strict

private func FindBaitInContents(pObject) {
  // Inhalt von pObject überprüfen
  var pContents;
  for(var i;pContents=Contents(i,pObject);++i)
    if(pContents->~IsBait())
      return(pContents);
  // Kein Köder gefunden
  return(0);
}

public func CreateBait() {
  // Hat der Clonk einen Köder dabei?
  var pBait;
  if (!(pBait=FindBaitInContents(Contained()))) 
    return(Message("$TxtNoBait$",Contained()));
  // Köder benachrichtigen, dass er Köder werden soll
  pBait->~StartBaiting();
  // Köder auslegen
  Exit(pBait,-15+31*GetDir(Contained()),+18);
  if(GBackLiquid(-15+31*GetDir(Contained()),+18)) Sound("Splash1");
  ObjectSetAction(pBait,"Bait",Contained());
}

public func Activate(pClonk) {
  [$TxtStartFishing$]
  // Clonk ist noch zu klein
  if (GetCon(pClonk)<100)
    return(Message("$TxtCantFish$",pClonk,GetName(pClonk)));  
  // Kann der Clonk derzeit angeln?
  if (!(pClonk->~ReadyToFish())) return(0);
  // Hat der Clonk einen Köder dabei?
  if (!FindBaitInContents(pClonk)) 
    return(Message("$TxtNoBait$",pClonk));
  // Clonk fängt an zu angeln
  ObjectSetAction(pClonk,"ThrowFishingPole");
  // Fertig
  return(1);
}

/* Kann vom Trapper in Handarbeit hergestellt werden */
public func IsTrapperHandcraft() { return(1); }

