/*-- Burgbaupl‰ne --*/

#strict

public func UnfoldID() { return(CPPU); }

public func CalcValue() { return(30); }

public func Activate(pByObject)
{               
  [$TxtViewplans$]
  // Ansichtsobjekt erzeugen
  Sound("PlanUnfold");
  var pUnfold = CreateObject(UnfoldID(), 0, -10, GetOwner(pByObject));
  pUnfold->Activate();
  SetPlrView(GetOwner(pByObject), pUnfold);
  // Pl‰ne setzen
  SetKnowledge(GetOwner(pByObject));
  return(1);
}

public func SetKnowledge(iPlayer)
{
  // Auﬂenlager
  SetPlrKnowledge(iPlayer,WGTW);
  SetPlrKnowledge(iPlayer,PALS);
  SetPlrKnowledge(iPlayer,PAL2);
  SetPlrKnowledge(iPlayer,ARCH);

  // Burgelemente  
  SetPlrKnowledge(iPlayer,BAS7);
  SetPlrKnowledge(iPlayer,CPT2);
  SetPlrKnowledge(iPlayer,CPW1);
  SetPlrKnowledge(iPlayer,CPW2);
  SetPlrKnowledge(iPlayer,CPT1);
  SetPlrKnowledge(iPlayer,CPWL);
  SetPlrKnowledge(iPlayer,CPWR);
  SetPlrKnowledge(iPlayer,CPTL);
  SetPlrKnowledge(iPlayer,CPTR);
  SetPlrKnowledge(iPlayer,CPT3);
  SetPlrKnowledge(iPlayer,CPT4);
  SetPlrKnowledge(iPlayer,CPR1);
  SetPlrKnowledge(iPlayer,CPR2);
  SetPlrKnowledge(iPlayer,CPH1);
  SetPlrKnowledge(iPlayer,CPOF);
  SetPlrKnowledge(iPlayer,CPWK);
  SetPlrKnowledge(iPlayer,CPAL);
  SetPlrKnowledge(iPlayer,CPKT);
  SetPlrKnowledge(iPlayer,CPHC);
  SetPlrKnowledge(iPlayer,CPEL);
  
  // Handwerkszeug & Einrichtung
  SetPlrKnowledge(iPlayer,ANVL);
  SetPlrKnowledge(iPlayer,FRGE);
  SetPlrKnowledge(iPlayer,FDRS);
  SetPlrKnowledge(iPlayer,SPRC);
  SetPlrKnowledge(iPlayer,SWRC);
  SetPlrKnowledge(iPlayer,SHRC);
  SetPlrKnowledge(iPlayer,BWRC);
  SetPlrKnowledge(iPlayer,TABL);
  SetPlrKnowledge(iPlayer,WODC);
  SetPlrKnowledge(iPlayer,DPOT);
  SetPlrKnowledge(iPlayer,OVEN);
  SetPlrKnowledge(iPlayer,THRN);
  SetPlrKnowledge(iPlayer,BED1);
  SetPlrKnowledge(iPlayer,CNDL);
  SetPlrKnowledge(iPlayer,DCO3);
  SetPlrKnowledge(iPlayer,DCO4);

  return(1);
}