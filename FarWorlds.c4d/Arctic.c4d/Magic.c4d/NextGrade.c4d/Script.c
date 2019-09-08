/* Nächste Stufe */

#strict

func Activate(pCaster,none,pAmulet) {
  if(GetID(pAmulet)==AML1)      return(Life(pAmulet,pCaster));
  else if(GetID(pAmulet)==AMD1) return(Death(pAmulet,pCaster));
  else if(GetID(pAmulet)==AMM1) return(Mystic(pAmulet,pCaster));
  else  {
    Message("$TxtNoAmulet$",pCaster);
    Sound("Error");
    RemoveObject();
    return(0);
  }
}

func Life(pAmulet,pCaster) {
  // Nur wenn genug Bevölkerung!
  if(GetCrewCount(GetOwner(pCaster))<5) {
    Message("$TxtPopulation$",pCaster);
    Sound("Error");
    RemoveObject();
    return(0);
  }
  ChangeDef(AML2,pAmulet);
  return(1);
}

func Death(pAmulet,pCaster) {
  // Nur bei Nacht!
  if (!Inside(GetSeason(),60,90)) {
    Message("$TxtNight$",pCaster);
    Sound("Error");
    RemoveObject();
    return(0);
  }
  ChangeDef(AMD2,pAmulet);
  return(1);
}

func Mystic(pAmulet,pCaster) {
  ChangeDef(AMM2,pAmulet);
  return(1);
}