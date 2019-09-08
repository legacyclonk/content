/*-- Alchemiekochtopf --*/
// funktioniert ähnlich wie der Amboss

#strict

local product;
local worker;

/* Steuerung */

public func ContextProduction(pCaller)
{
  [$TxtProduce$|Image=ACLD|Condition=IsBuilt]
  return(ProductionMenu(pCaller));
}

protected func IsBuilt() {
  return(GetCon() >= 100);
}

public func ControlDigDouble(pCaller)
{
  [$TxtProduce$|Image=ACLD]
  return(ProductionMenu(pCaller));
}

/* Produktion */

public func ProductionMenu(pWorker) {
  // Nur wenn inaktiv
  if (!ActIdle()) return(0);
  // Menu erzeugen
  CreateMenu(ACLD, pWorker, this(), 1, "$TxtNoPlans$");
  // Menü mit Bauplänen füllen
  var i = 0, def;
  while (def = GetPlrKnowledge(GetOwner(pWorker), 0, i++, C4D_Object()))
    if(def->~IsPotion())
      AddMenuItem("$TxtProduceMenu$","ProductionOrder",def,pWorker,0,pWorker);
  // Fertig
  return(1);
}

public func ProductionOrder(idProduct, pWorker) {

  // Verfügbarkeit überprüfen
  var pTemp;
  if(!ObjectCount(ALCO)) pTemp = CreateObject(ALCO,10,10,-1);
  var fCheck = CheckAlchem(idProduct, pWorker);
  if(pTemp) RemoveObject(pTemp);
  if(!fCheck) {
    Message("$MsgNotEnoughIngredients$",pWorker,GetNeededAlcStr(idProduct,pWorker));
    return(0);
  }

  ProductionStart(pWorker,idProduct);
  // Los geht's
  return(1);
}

public func ProductionStart(pWorker, idProduct)
{
  // mogeln gibts nicht ;)
  var pTemp;
  if(!ObjectCount(ALCO)) pTemp = CreateObject(ALCO,10,10,-1);
  var fCheck = CheckAlchem(idProduct, pWorker);

  if(!fCheck) {
    if(pTemp) RemoveObject(pTemp);
    Message("$MsgNotEnoughIngredients$",pWorker,GetNeededAlcStr(idProduct,pWorker));
    return(0);
  }

  // Zutaten rausfischen
  ReduceAlchem(idProduct,pWorker);
  if(pTemp) RemoveObject(pTemp);
  // Zielprodukt speichern
  product = idProduct;
  // Arbeiter speichern
  worker = pWorker;
  // Besitzer setzen für Farbe
  //SetOwner(GetOwner(pWorker));
  // Aktion starten
  SetAction("Brew");
  // Weiter geht's
  return(1);    
}

private func Brewing() 
{
  // Noch nicht fertig
  if (GetActTime() < product->~BrewTime()) return(0);
  // Produkt erzeugen
  var pProduct = CreateContents(product);
  // Sound
  Sound("Magic1");
  // Aktion beenden
  ProductionCancel();
  // Arbeiter noch da
  if (worker)
    // Fasst uns noch an
    if ((worker->GetAction() S= "Push") && (worker->GetActionTarget() == this()))
      // Arbeiter bekommt den Trank in die Hand
      worker->Collect(pProduct);  
  return(1);
}

private func ProductionCancel() {
  // Arbeit abbrechen
  if (GetAction() eq "Brew") SetAction("Idle");
  // Fertig
  return(1);
}

private func Effects() {
  Sound("Alchem*");
  CreateParticle("MSpark",RandomX(-8,8),-8,RandomX(-5,5),RandomX(0,-28),RandomX(10,40),RGB(127,127,255));
  CreateParticle("MSpark",RandomX(-8,8),-8,RandomX(-5,5),RandomX(-8,-15),RandomX(20,50),RGB(255,127,127));
}
