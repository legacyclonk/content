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

public func MenuExtraMode()
{
  var noMagicEnergy = ObjectCount(NMGE) > 0;
  if(ObjectCount(ALCO))
  {
    if(noMagicEnergy)
    {
      return C4MN_Extra_Components;
    }
    return C4MN_Extra_ComponentsLiveMagic;
  }
  if(!noMagicEnergy)
  {
    return C4MN_Extra_LiveMagicValue;
  }
  return C4MN_Extra_None;
}

public func Activate(pClonk) {
  [$TxtMagic$]
  if(GetPhysical("Magic",2,pClonk)<100000) return(0,Message("$TxtsisnoAngakok$",this(),GetName(pClonk)));
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),pClonk);
  // Menü öffnen
  CreateMenu(MCMS,pClonk,this(),MenuExtraMode(),"$TxtNoMagic$",ObjectNumber(pClonk));
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", LGT2, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", MGSW, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", DTFH, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", NXGD, pClonk,0,pClonk );
  return(1);
}
