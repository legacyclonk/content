#strict

func Activate(pClonk) {
  [$TxtApply$]
  pClonk->~Feed(80);
  pClonk->~MagicWarm(1800);
  Sound("Inflame");
  RemoveObject();
  return(1);
}

func Hit() { Sound("WoodHit*"); }  
