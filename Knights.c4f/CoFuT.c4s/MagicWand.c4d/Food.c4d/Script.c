/* Nahrung herbeizaubern */

#strict

public func Activate(pCaster,pRealcaster) {
  
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("FoodNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  // Sound
  Sound("Magic*");
  // Effekt
  CreateParticle("MSpark", 0,0, 1000, RGBa(255,50,50,150));

  if(pRealcaster) pCaster=pRealcaster;

  var i, id; var j;
  // Liste mit Essen machen
  while(id = GetDefinition(i++, C4D_Object))
    if(GetDefEdible(id) || id==CBRD || id==CSNK || id==WMPF) if(!id->~IsPotion()) { Local(j++) = id; }	  
  
  // Ein Zufälliges Essen zaubern
//  CreateObject(Local(Random(j))); jetzt erst verzögert erschaffen! (muss ja erst zubereitet werden)
  
  // Erscheineffekt machen
  AddEffect("SpawnFoodPSpell", this(), 200, 4, 0, GetID(), Local(Random(j)));
  
  //RemoveObject();
  SetCategory(16);
  
  return(1);
}

func FxSpawnFoodPSpellStart(pClonk, iNumber, iTemp, idFood)
{
  if(!iTemp) EffectVar(0, pClonk, iNumber) = idFood;	
}

func FxSpawnFoodPSpellTimer(pClonk, iNumber, iTime)
{
  var idFood = EffectVar(0, pClonk, iNumber);
  Smoke(GetX(pClonk), GetY(pClonk), RandomX(10, 30));
  CastParticles("PSpark", 2, 8, GetX(pClonk), GetY(pClonk), 20, 180, RGB(250, 0, 0), RGB(200, 100, 0));
  CastParticles("FSpark", 1, 50, GetX(pClonk), GetY(pClonk), 20, 80);
  if(iTime>100+GetValue(0, idFood)*2) return(-1, CreateObject(idFood, GetX(pClonk), GetY(pClonk)), RemoveObject(pClonk));
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("46"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
