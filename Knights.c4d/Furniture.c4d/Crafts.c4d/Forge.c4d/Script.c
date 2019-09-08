/*-- Esse --*/

#strict

/* Steuerung */

public func ContextProduction(pCaller)
{
  [$TxtHeatmetal$|Image=ANML]
  return(ProductionOrder(ANML, pCaller));
}
 
public func ControlDigDouble(pCaller)
{
  [$TxtHeatmetal$|Image=ANML]
  return(ProductionOrder(ANML,pCaller));
}

/* Produktion */

public func IsProducerOf(clonk, def) 
{
  return (def == ANML);
}

public func HowToProduce(clonk, def) 
{
  AddCommand (clonk, "Wait", 0, 0, 0, 0, 0, 240);
  ProductionOrder (def, clonk);
  return(1);
}

public func ProductionOrder(id idProduct, object pWorker)
{
  AddCommand(pWorker, "Call", this(), 0, 0, 0, 0, "Acquisition", 0, 3);
  // Material beschaffen
  ProductionAcquireMaterial(COAL, 1, pWorker);
  ProductionAcquireMaterial(METL, 1, pWorker);
  // Brennvorgang beginnt selbsttätig
  return(1);
}

private func ProductionAcquireMaterial(id idMaterial, int iAmount, object pWorker)
{
  // Benötigte Menge ermitteln
  var amount = Max(iAmount - ContentsCount(idMaterial, this()), 0);
  while (amount --)
  {
    AddCommand(pWorker, "Put", this(), 0,0,0, 0, idMaterial);
    AddCommand(pWorker, "Acquire", 0,0,0,this(), 0, idMaterial, 5, 3);
  }
}

public func Acquisition(pWorker)
{
  // Materialbeschaffung erfolgreich
  return(1);
}

public func AcquisitionFailed(pWorker)
{
  // Materialbeschaffung fehlgeschlagen: Meldung ausgeben
  if (!FindContents(METL)) 
  	return(Message("$TxtNometalavailable$", pWorker));
  Message("$TxtNeedsfueltoburn$", pWorker);
  return(1);
}

/* Brennvorgang */

private func CheckContents()
{
  // Noch aktiv
  if (!ActIdle()) return(1);
  // Neues Feuer mit Kohle
  var coal = FindContents(COAL);
  if (coal)
  {
    RemoveObject(coal);
    SetAction("Burning");
  }
}

private func Burning()
{
  if(ContentsCount(METL))
    SetAction("Melting");
  if(GetActTime() > 1500)
    SetAction("Idle");
}

private func Melting()
{
  // Noch nicht fertig
  if(GetActTime() < 200) return(1);
  SetAction("Idle");
  var metl = FindContents(METL);
  if(!metl) return(1);
  RemoveObject(metl);
  // Glühendes Metall auswerfen
  Smoke(0, 15, 15);  
  Exit(CreateContents(ANML), 0, 15, 0, -1);
  CastParticles("PxSpark", 6, 10, 0, 20, 15, 30, RGB(255, 223, 127), RGB(255, 223, 127));
  Sound("Pshshsh");
}

private func Effects()
{
  Smoke(0, 15, 10);
  CastParticles("PxSpark", 3, 10, 0, 20, 15, 30, RGB(255, 223, 127), RGB(255, 223, 127));
}

protected func CalcValue() // Geringer Verkaufswert
{
  if (Contained()) return(25);
  return(150);
}
