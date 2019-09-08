/* Schrumpfen */

#strict

public func Activate(pCaster,pRealcaster)
{
  if(pRealcaster) pCaster = pRealcaster;
  var iShrink, iTime, pCombo, iEffect;

  iTime = 400;
  // ein Stein verlängert die Dauer
  if(pCombo = FindContents(ROCK,pCaster)) {
    iTime *= 2;
    RemoveObject(pCombo);
  }

  // Schon geschrumpft?
  if(iEffect = GetEffect("ShrinkPSpell", pCaster)) {
    // Das war gestern
    //return(0, Message("MsgIsShrunken", pCaster), RemoveObject());
    EffectCall(pCaster,iEffect,"AddTime",iTime);
    Sound("Magic1");
    RemoveObject();
    return(1);
  }

  iShrink = 75;
  // mit Holz/Gold schrumpft man mehr
  if((pCombo = FindContents(WOOD,pCaster)) || (pCombo = FindContents(GOLD,pCaster))) {
    iShrink = 50;
    RemoveObject(pCombo);
  }

  // Clonk wird geschrumpft
  AddEffect("ShrinkPSpell",pCaster,250,2,0,GetID(),iShrink,iTime);
  Sound("Magic1");

  // entfernen
  RemoveObject();
  return(1);
}

public func FxShrinkPSpellStart(pClonk, iEffectNumber, iTemp, iShrink, iTime)
{
  if(iTemp) return();
  EffectVar(0, pClonk, iEffectNumber) = iShrink;
  EffectVar(1, pClonk, iEffectNumber) = iTime+(100-iShrink);
}

public func FxShrinkPSpellTimer(pClonk, iEffectNumber, iTime)
{
  var i, iAngle, iCon = EffectVar(0, pClonk, iEffectNumber);
  // Zeit ist vorrüber: Clonk muss wachsen
  if(iTime > EffectVar(1,pClonk, iEffectNumber)) {
    // Wächst nur bis 99 da bei 100 die Initialize-Funktion im Clonk aufgerufen wird, was zu Bugs führen kann
    DoCon(Min(1,99-GetCon(pClonk)), pClonk);
    iAngle = Random(360);
    CreateParticle("PSpark",GetX(pClonk)+Sin(iAngle,15),GetY(pClonk)-Cos(iAngle,15),-Sin(iAngle,5),Cos(iAngle,5),RandomX(50,70),RandomX(RGBa(255,255,100,50),RGBa(100,100,0,75)),pClonk);
    // Clonk ist wieder groß
    if(GetCon(pClonk) >= 99)
      return(-1);
  }
  // Ansonsten: Clonk muss noch schrumpfen
  else if(GetCon(pClonk) > iCon) {
    DoCon(-1, pClonk);
    // Sorgt dafür, dass die Schrumpfpartikel nicht auftreten wenn der Clonk schon geschrumpft ist
    if(!EffectVar(2,pClonk,iEffectNumber)) {
      i = Random(20);
      iAngle = Random(360);
      CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),Sin(iAngle,i),Cos(iAngle,i),RandomX(50,70),RGB(RandomX(100,255),RandomX(100,255),Random(101)),pClonk);
//      CastParticles("PSpark",1,20,GetX(pClonk),GetY(pClonk)+4,50,70,RGBa(255,255,100),RGBa(100,100,0),pClonk);
      if(GetCon(pClonk) == iCon) EffectVar(2,pClonk,iEffectNumber) = 1;
    }
  }
  return(1);
}

public func FxShrinkPSpellAddTime(pClonk, iEffectNumber, iTime)
{
  EffectVar(1, pClonk, iEffectNumber) += iTime;
  if(GetCon(pClonk) > EffectVar(0, pClonk, iEffectNumber)+1) EffectVar(2, pClonk, iEffectNumber) = 0;
  return(1);
}

public func FxShrinkPSpellStop(pClonk, iEffectNumber, iReason, iTemp)
{
  if(iTemp) return();
  // Da "SetCon(100)" die Größe nicht genau auf 100 setzt und die Initialize-Funktion im Clonk aufruft, wird die Größe per "ResetCon" auf genau 100 gesetzt
  ResetCon(pClonk);
  return(1);
}

/* Zaubercombo */

public func GetSpellClass() { return(EART); }
public func GetSpellCombo() { return("511"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
