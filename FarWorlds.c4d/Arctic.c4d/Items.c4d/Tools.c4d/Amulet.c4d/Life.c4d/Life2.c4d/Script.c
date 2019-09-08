/*-- Lebensamulett --*/

#strict
#include AMD1

private func MagicEnergy() {
  // Nur Clonks
  if (~GetOCF(Contained()) & OCF_CrewMember()) return(1);
  // Der tragende Clonk wird automatisch zum Angakok
  // SetPhysical("Magic",100000,2,Contained());
  // Energiezufuhr nach Größe der Bevölkerung
  for(var i=GetCrewCount(GetOwner(Contained()))/2;i>0;--i)
    DoMagicEnergy(+1,Contained());
}

public func Activate(pClonk) {
  [$TxtMagic$]
  if(GetPhysical("Magic",2,pClonk)<100000) return(0,Message("$TxtsisnoAngakok$",this(),GetName(pClonk)));
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),pClonk);
  // Menü öffnen
  var iExtra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  CreateMenu(MCMS,pClonk,this(),iExtra,"$TxtNoMagic$",GetMagicEnergy(pClonk));
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", MWTH, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", SFHN, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", MGHL, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", ASIT, pClonk,0,pClonk );
  return(1);
}
