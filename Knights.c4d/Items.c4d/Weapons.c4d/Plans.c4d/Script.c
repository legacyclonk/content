/*-- Waffenpläne --*/

#strict
#include CPPL

public func UnfoldID() { return(WPPU); }

public func CalcValue() { return(10); }

public func SetKnowledge(iPlayer)
{
  // Waffen
  SetPlrKnowledge(iPlayer,AXE1);
  SetPlrKnowledge(iPlayer,BOW1);
  SetPlrKnowledge(iPlayer,LANC);
  SetPlrKnowledge(iPlayer,SPER);
  SetPlrKnowledge(iPlayer,SWOR);
  SetPlrKnowledge(iPlayer,SHIE);
  SetPlrKnowledge(iPlayer,ARMR);
  SetPlrKnowledge(iPlayer,ARWP);
  SetPlrKnowledge(iPlayer,FARP);
  SetPlrKnowledge(iPlayer,XARP);
  
  // Schwere Waffen und Fahrzeuge
  SetPlrKnowledge(iPlayer,CATA);
  SetPlrKnowledge(iPlayer,XBOW);
  SetPlrKnowledge(iPlayer,CANN);
  SetPlrKnowledge(iPlayer,LORY);
  SetPlrKnowledge(iPlayer,SLBT);
  SetPlrKnowledge(iPlayer,BALN);
  SetPlrKnowledge(iPlayer,BLMP);  
  
  // Flints
  SetPlrKnowledge(iPlayer,GUNP);
  SetPlrKnowledge(iPlayer,FLNT);
  SetPlrKnowledge(iPlayer,SFLN);
  SetPlrKnowledge(iPlayer,EFLN);
  SetPlrKnowledge(iPlayer,TFLN);
  SetPlrKnowledge(iPlayer,STFN);
  SetPlrKnowledge(iPlayer,FBMP);
  
  // Einrichtung
  SetPlrKnowledge(iPlayer,SPRC);
  SetPlrKnowledge(iPlayer,SWRC);
  SetPlrKnowledge(iPlayer,SHRC);
  SetPlrKnowledge(iPlayer,ARCH);
  SetPlrKnowledge(iPlayer,TABL);
  SetPlrKnowledge(iPlayer,WODC);
  SetPlrKnowledge(iPlayer,BED1);

  // Gegenstände
  SetPlrKnowledge(iPlayer,CNKT);
  SetPlrKnowledge(iPlayer,LNKT);
  SetPlrKnowledge(iPlayer,WDBR);
  SetPlrKnowledge(iPlayer,BANP);
  SetPlrKnowledge(iPlayer,TENP);
  SetPlrKnowledge(iPlayer,KSDL);

  // Und netterweise sogar Kochrezepte
  SetPlrKnowledge(iPlayer,BRED);
  SetPlrKnowledge(iPlayer,COKI);
  SetPlrKnowledge(iPlayer,DOGH);
  SetPlrKnowledge(iPlayer,CODH);

  return(1);
}