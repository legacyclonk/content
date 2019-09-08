/*--Amulett--*/

#strict

protected func Hit() { Sound("Crystal*"); }

public func LightningStrike() {
  // Sound
  Sound("Electric");
  // magische Energie
  if(GetOCF(Contained()) & OCF_CrewMember())
    DoMagicEnergy(+10,Contained());
  return(1);
}

/* Zaubern */

protected func DoMagic(idMagic,pClonk) {
  // nur beim Gehen
  if(GetAction(pClonk)ne"Walk") return(0);
  // Energie prüfen
  if(!CheckMana(idMagic,pClonk)) return(Message("$TxtErrMagic$",this(),GetName(pClonk)), Sound("Error") );
  // Aktion starten
  ObjectSetAction(pClonk,"Magic");
  // Zauberobjekt erschaffen und aktivieren
  if(CreateObject(idMagic,0,0,GetOwner())->Activate(pClonk,0,this()))
    DoMagicEnergy(-Value(idMagic),pClonk);
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
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", LGT2, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", MGSW, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", DTFH, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", NXGD, pClonk,0,pClonk );
  return(1);
}