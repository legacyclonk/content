#strict

local pAngakok,idSpirit;

func Activate(idMagic,pCaster) {
 pAngakok=pCaster;
 idSpirit=idMagic;
}

func Hit() {
  CreateObject(idSpirit,0,3,GetOwner())->Activate(pAngakok);
  RemoveObject();
}