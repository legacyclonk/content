/*--- Bausatz ---*/

#strict

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  // Clonk anhalten
  SetComDir(COMD_Stop(), pClonk);
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoconstructionplansa$");
  var idType; var i = 0;
  while (idType = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_Structure()))
    AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);
	return(1);
}

protected func CreateConstructionSite(idType)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained()) ne "Walk") return(0);
  if (Contained(Contained())) return(0);
  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(0, 10, Contained()) ) return(0);
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  // Baustelle erschaffen
  var pSite;
  if (!(pSite = CreateConstruction(idType, 0, 10, GetOwner(Contained()), 1, 1,1))) return(0);
  // Meldung
  Message("$TxtConstructions$", Contained(), GetName(pSite));
  // Bausatz verbrauchen
  RemoveObject();
  return(1);
}

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
