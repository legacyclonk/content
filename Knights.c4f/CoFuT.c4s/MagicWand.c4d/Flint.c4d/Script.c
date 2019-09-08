/* Flint herbeizaubern */

#strict

public func Activate(pCaster,pRealcaster) {
  var pSlurp;
  var pFlint;
  
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("FlintNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  // Sound
  Sound("Magic*");
  // Effekt
  CreateParticle("MSpark", 0,0, 1000, RGBa(255,50,50,150));

  if(pRealcaster) pCaster=pRealcaster;
 
 
  var cnt=Random(5);
  if(!  cnt)pFlint=CreateObject(FLNT);
  if(!--cnt)pFlint=CreateObject(SFLN);
  if(!--cnt)pFlint=CreateObject(TFLN);
  if(!--cnt)pFlint=CreateObject(STFN);
  if(!--cnt)pFlint=CreateObject(XARW);
  // Flint ins Inventar des Clonks packen: wenn voll, gleich explodieren
  if(!Collect(pFlint,pCaster))
    {
    pFlint->~Hit();
    }
 
  RemoveObject();
  
  return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(ROK1); }
public func GetSpellCombo(pMage) { return ("66"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
