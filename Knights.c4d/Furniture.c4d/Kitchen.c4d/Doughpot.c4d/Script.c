/*-- Backtrog --*/

#strict 2

local product, empty_barrel;

/* Steuerung */

public func ContextMakeDough(object pCaller)
{
  [$TxtMakedough$|Image=DOGH]
  return ProductionOpenMenu(pCaller);
}
  
public func ControlDigDouble(object pCaller)
{
  [$TxtMakedough$]
  return ProductionOpenMenu(pCaller);
}
  
/* Rührvorgang abbrechen, wenn der Clonk das Objekt verläßt */
protected func ContainedDown(pClonk)
{
  // Aktion beenden
  SetAction("Idle");
  // Clonk auswerfen
  Exit(pClonk);
  // Der Teig bleibt im Trog
  return true;
}

/* Produktion */

public func IsProducerOf(caller, def) 
{
  if (!GetPlrKnowledge(GetOwner (caller), def)) return false;
  if (!DefinitionCall(def, "IsDough")) return false;
  return true;
}

public func HowToProduce(clonk, def) 
{
  AddCommand(clonk, "Wait", 0, 0, 0, 0, 165);
  ProductionOrder(def, clonk);
  return true;
}

public func ProductionOpenMenu(object pWorker)
{
  // Menü erzeugen
  CreateMenu(DPOT, pWorker, this, 1, "$TxtNorecipes$");
  // Teigpläne suchen und Menü füllen
  var i, id;
  while(id = GetPlrKnowledge(GetOwner(pWorker), 0, i ++, C4D_All) )
    if(DefinitionCall(id, "IsDough") )
      AddMenuItem("$TxtMixs$", "ProductionOrder", id, pWorker, 0, pWorker);
  return true;
}
  
public func ProductionOrder(id defProduct, object pWorker)
{
  // Arbeit beginnen
  AddCommand(pWorker, "Call", this, defProduct, 0, 0, 0, "ProductionStart");
  // Vorher: Zum Backtrog gehen
  AddCommand(pWorker, "MoveTo", 0, GetX(), GetY() );
  // Vorher: Benötigtes Material beschaffen
  ProductionAcquireMaterialFor(defProduct, pWorker);
  if (!ContentsCount(WDWB)) ProductionAcquireMaterial(WDWB,1,pWorker);
  // Ok
  return(1);
}

private func ProductionAcquireMaterialFor(id defProduct, object pWorker)
{
  var i, id;
  while(id = GetComponent(0, i ++, 0, defProduct) )
    if(ContentsCount(id) < GetComponent(id, 0, 0, defProduct) )
      ProductionAcquireMaterial(id, GetComponent(id, 0, 0, defProduct) - ContentsCount(id), pWorker);
  // Ok
  return(1);
}

private func ProductionAcquireMaterial(id idMaterial, int iAmount, object pWorker)
{
  // Benötigte Menge ermitteln
  while(iAmount--)
  {
    AddCommand(pWorker, "Put", this, 0,0,0, 0, idMaterial);
    AddCommand(pWorker, "Acquire", 0,0,0,this, 0, idMaterial, 5, 3);
  }
}

public func ProductionStartFailed() { return true; }

public func ProductionStart(clonk, def) 
{
  var obj;
  // Wasser suchen
  if (!(obj = FindContents(WDWB))) { Message("$TxtNeedsWater$", this); return false; }
  // Objekt zusammensetzen
  product = ComposeContents(def);
  if (!product) { Sound("Error"); return false; }
  // Wasser verbrauchen
  RemoveObject(obj);
  // Leeres Fass als Ersatz fuers Wasserfass erzeugen
  empty_barrel = CreateContents(WDBR);
  // Clonk einsaugen
  Enter(this, clonk);
  // Besitzer setzen für Farbe des animierten Clonk
  SetOwner(GetOwner(clonk));
  // Rühren starten
  SetAction("Mix");
  // Weiter geht's
  return true;
}

/* Rührvorgang */

private func Mixing()
{
  // Noch nicht fertig  
  if(GetActTime() < 150) return true;
  // Aktivität beenden
  SetAction("Idle");
  // Clonk auswerfen
  var i=ContentsCount(), obj, clonk;
  while (i--) if (obj = Contents(i))
    if (GetAlive(obj))
    {
      clonk = obj;
      Exit(obj, RandomX(-10, 10), 12);
    }
  // Teig einsammeln
  if (clonk && product) clonk->Collect(product);
  // Leeres Fass auch gleich mitgeben
  if (clonk && empty_barrel) clonk->Collect(empty_barrel);
  return true;
}

