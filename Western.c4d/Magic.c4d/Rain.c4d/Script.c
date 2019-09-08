/* Regen */

#strict

func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  var mat = "Water";

  // Kombinieren?
  var obj;
  if (obj = FindContents(ABRL, pClonk)) {
    mat = "Acid"; 
  } else if (obj = FindContents(OBRL, pClonk)) {
    mat = "Oil";
  } else if (obj = FindContents(LBRL, pClonk)) {
    mat = "Lava";
  }

  if(obj) RemoveObject(obj);

  // es fängt an zu Regnen
  AddEffect("RainUSpell",0,250,1,0,GetID(), GetX(), GetY()-100, Material(mat));

  // entfernen
  RemoveObject();
  return(1);
}

func FxRainUSpellStart(pClonk, iEffectNumber, fTemp, iX, iY, iMat)
{
  if(fTemp) return();
  EffectVar(0, pClonk, iEffectNumber) = iX;
  EffectVar(1, pClonk, iEffectNumber) = iY;
  EffectVar(2, pClonk, iEffectNumber) = iMat;
}

func FxRainUSpellTimer(pClonk, iEffectNumber, iTime)
{
  var iX =   EffectVar(0, pClonk, iEffectNumber), 
      iY =   EffectVar(1, pClonk, iEffectNumber),
      iMat = EffectVar(2, pClonk, iEffectNumber);
  var iXRange = 50;
  var iYRange = 5;
  var i = 2;
  while(i--)
  {
    InsertMaterial(iMat, iX+RandomX(-iXRange,iXRange), iY+RandomX(-iYRange,iYRange));
    if(Random(2)) CreateParticle("Cloud", iX+RandomX(-iXRange, iXRange), iY+RandomX(-iYRange,iYRange), 0, 0, RandomX(400, 1000));
  }
  if(iTime>500) return(-1);
}

/* Zaubercombo */

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return ("455"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
