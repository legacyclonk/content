/* Heilung */

#strict

func Activate(pCaster,pRealcaster)
{
  if(GetActMapVal("Name","Magic",GetID(pCaster)))
    if (!DoesMagic(pCaster))
      if (GetAction(pCaster) eq "Ready") // Für den Zauberturm: Dieser setzt Magic ansonsten erst verspätet
        ObjectSetAction(pCaster, "Magic");
      else
        return(0,RemoveObject());
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;
  // schon voll geheilt
  if(GetEnergy(pClonk)==GetPhysical("Energy",0,pClonk)/1000) return(0,RemoveObject());
  AddEffect("HealPSpell",pCaster,250,2,0,GetID(),pClonk);
  return(1,RemoveObject());
}

func FxHealPSpellStart(pCaster, iEffectNumber, iTemp, pClonk)
{
  // Zielobjekt speichern: Das Zielobjekt des Zaubers ist nicht immer der
  // Zauberer selbst, Beispiel Zauberturm
  EffectVar(0,pCaster,iEffectNumber)=pClonk;
}

func FxHealPSpellTimer(pCaster, iEffectNumber, iEffectTime)
{
  var pClonk=EffectVar(0,pCaster,iEffectNumber);
  var iMLength=GetActMapVal("Length",GetAction(pCaster),GetID(pCaster));
  if(iEffectTime >= 200) return(-1);
  
  // magische Aktion vorhanden
  if(GetActMapVal("Name","Magic",GetID(pCaster)))
  {
    // z.B. während des Zauberns durch einen Stein getroffen
    if(!DoesMagic(pCaster)) return(-1);
    // Zauberaktion für das zaubernde Objekt: Es wird angenommen, dass diese
    // in einem Raum von etwa 2/6 bis 5/6 der Zauberaktion spielt
    if(GetPhase(pCaster) >= iMLength*5/6) SetPhase(iMLength*2/6,pCaster);
  }

  // Clonk heilen
  if (!Random(3))
    {
    var prev_energy = GetEnergy(pClonk);
    DoEnergy(+2,pClonk);
    var after_energy = GetEnergy(pClonk);
    // Antiheilfluch: Todesheilung bei 1HP abbrechen
    if (after_energy < prev_energy && after_energy <= 2) return(FX_Execute_Kill);
    }

  // Abbruch wenn voll geheilt
  if(GetEnergy(pClonk)==GetPhysical("Energy",0,pClonk)/1000) return(FX_Execute_Kill);

  
  // Partikeleffekte für den betroffenen
  CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8),GetY(pClonk)+RandomX(-10, 10),0,-20,RandomX(18,38),RGB(210,20,20));
  CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),0,0,180,RGB(16,8,0),pCaster);

  return(1);
}

protected func FxHealPSpellInfo() { return("{{MGHL}} $SpellInfo$"); }

func DoesMagic(pObj) { return(WildcardMatch(GetAction(pObj), "*Magic")); }

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("655"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
