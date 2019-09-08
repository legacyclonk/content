/*-- Backofen --*/

#strict

/* Steuerung */

public func ContextBake(object pCaller)
{
  [$TxtBake$|Image=BRED]
  return(ProductionOrder(pCaller));
}

public func ControlDigDouble(object pCaller)
{
  [$TxtBake$|Image=BRED]
  return(ProductionOrder(pCaller));
}
 
/* Produktion */

public func ProductionOrder(object pWorker)
{
  // Teig im Ofen
  var dough = FindDoughContents();
  if(!dough) dough = FindDoughContents(pWorker);
  if(!dough) return(Message("$TxtNodoughathand$", pWorker));  

  // Arbeit beginnen
  AddCommand(pWorker, "Call", this(), 0, 0, 0, 0, "ProductionStart");  
  // Vorher: Zum Ofen gehen
  AddCommand(pWorker, "MoveTo", 0, GetX(), GetY() );
  // Vorher: Brennholz beschaffen
  ProductionAcquireMaterial(WOOD, 2, pWorker);
  // Vorher: Teig in den Ofen legen
  if(Contained(dough) != this() )
    AddCommand(pWorker, "Put", this(), 0, 0, dough);
  // Los geht's
  return(1);
}
  
private func ProductionAcquireMaterial(id idMaterial, int iAmount, object pWorker)
{
  // Benötigte Menge ermitteln
  var amount = Max(iAmount - ContentsCount(idMaterial), 0);
  while(amount --)
  {
    AddCommand(pWorker, "Put", this(), 0,0,0, 0, idMaterial);
    AddCommand(pWorker, "Acquire", 0,0,0,this(), 0, idMaterial, 5, 3);
  }
}

public func ProductionStartFailed()
{
  // TODO: Irgendwelche Fehlermeldungen oder sowas...?
  return(1);
}

public func ProductionStart()
{
  if(ContentsCount(WOOD) < 2) return(Error("$TxtNotenoughwood$"));
  RemoveObject(FindContents(WOOD));
  RemoveObject(FindContents(WOOD));
  SetAction("Burning");
}

private func Baking()
{
  // Noch nicht fertig
  if(GetActTime() < 250) return(1);
  SetAction("Idle");
  // Alle Teige backen. Bei Holz bringt Bake einfach nix.
  var obj, i;
  var baked = false;
  while(obj = Contents(i ++) )
    if(obj->~Bake() )
    {
      Exit(obj);
      baked = true;
      -- i;
    }
    
  if(baked) SetAction("OpenDoor");
  Smoke(0, 0, 12);
  Sound("Pshshsh");
}

private func FindDoughContents(object pContainer)
{
  var i, obj;
  while(obj = Contents(i++, pContainer) )
    if(obj->~IsDough() )
      return(obj);
  return(0);
}

/* Aufnahme */

protected func RejectCollect(id def, object obj)
{
  // Holzaufnahme
  if(def == WOOD) return(CollectWood());
  // Teigaufnahme
  if(obj->~IsDough() ) return(CollectDough());
  // Objekt ablehnen
  Error("$TxtCantbakes$", GetName(obj));
  return (1);
}

private func CollectWood()
{
  // Kann bis zu 5 Stücke Holz aufnehmen
  if(ContentsCount(WOOD) < 5) return(0);
  // Ablehnen
  Error("$TxtMorewoodwontfit$");
  return(1);
}

private func CollectDough()
{
  // Kann bis zu 3 Stücke Teig aufnehmen
  if(ContentsCount() - ContentsCount(WOOD) < 3) return(0);
  // Ablehnen
  Error("$TxtMoredoughwontfit$");
  return(1);
}

private func Error(string szMsg, param)
{
  Message(szMsg, this(), param);
  Sound("Error");
}

protected func CalcValue() // Geringer Verkaufswert
{
  if(Contained()) return(25);
  return(100);
}

