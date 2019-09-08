#strict

public func Activate(caster, real_caster) {
  // Zaubernden Clonk ermitteln
  var clonk = caster;
  if (real_caster) clonk = real_caster;

  // Richtungsvorzeichen ermitteln
  var dir_sign = -1;
  if (GetDir(caster) == DIR_Right()) dir_sign = +1;

  // Evtl. mit Flint kombinieren
  var obj;
  if (obj = FindContents(FLNT, clonk)) {
    // Holz erzeugen, anzünden und in Guckrichtung schleudern
    var wood = CreateObject(WOOD, 5*dir_sign, 0, -1);
    Incinerate(wood);
    // Richtigen Controller setzen
    SetController(GetController(clonk),wood);
    SetXDir(40*dir_sign, wood);
    SetYDir(-10, wood);
    // Flint verbrauchen
    RemoveObject(obj);
  }
  // Evtl. mit Pfeil kombinieren
  else
  {
    obj = FindContents(ARRW, clonk);
    if(!obj) if(FindContents(ARWP, clonk))
      obj = FindContents(ARWP, clonk)->GetItem();
    if(obj) {
      // Ein paar Objekte schleudern
      var fling_cnt = RandomX(4, 10);
      var fling_obj;
      while (fling_obj = FindObject(0, 0, 0, -1, -1, OCF_InFree(), 0, 0, NoContainer(), fling_obj)) {
        // Zu weit weg vom Clonk?
        if (ObjectDistance(clonk, fling_obj) > 5*GetDefCoreVal("Width", "DefCore", GetID(caster)))
          break;
        // Der Wolf konnte keine Steinhütten wegblasen - Clonks können keine Hütten mit Fundament wegblasen
        if (!LocalN("basement", fling_obj))
          {
          var pBasement = Local(9, fling_obj);
          if (GetType(pBasement) != C4V_C4Object()) pBasement=0;
          if (pBasement) if (!PrivateCall(pBasement, "BasementID")) pBasement=0;
          if (!pBasement)
            {
            SetXDir(dir_sign*Min(80, 80*350/GetMass(fling_obj)), fling_obj);
            SetYDir(-Min(20, 20/GetMass(fling_obj)), fling_obj);
            // Bei Objekten noch den Controller anpassen
            if(GetCategory(fling_obj) & C4D_Object())
              SetController(GetController(clonk),fling_obj);
            }
          }
        // Schon genügend Objekte geschleudert?
        if (!fling_cnt--) break;
      }
      if (!Contained(clonk)) {
        Fling(clonk, 8*dir_sign, -2);
      }
      // Pfeil verbrauchen
      RemoveObject(obj);
    }
    // Sonst normale Wirkung
    else {
      AddEffect("WindUSpell", 0, 104, 50, 0, GetID(), GetDir(caster));
      Sound("Wind2");
    }
  }
  
  RemoveObject();
  return(1);
}

protected func FxWindUSpellStart(pTarget,iEffectNumber,iTemp,iDir) {
  // temporäre aufrufe
  if(iTemp) return();

  var iChange=(iDir*2-1)*100;
  // Anderen Gravitationseffekt suchen
  if (!iTemp)
  {
    // hat sich selbst gefunden
    var iOtherEffect = GetEffect("WindUSpell", pTarget);
    // den anderen finden
    if(iOtherEffect==iEffectNumber) iOtherEffect=GetEffect("WindUSpell",pTarget,1);
    if(iOtherEffect)
    {
      // Gravitationsänderung auf diesen Effekt aufrechnen
      EffectVar(0,pTarget,iOtherEffect)=BoundBy(EffectVar(0,pTarget,iOtherEffect)+iChange,-100,100);
      // Selbst entfernen
      return(-1);
    }
  }
  // Wind berechnen
  EffectVar(0,pTarget,iEffectNumber)=BoundBy(GetWind(0,0,1)+iChange,-100,100);
  SetWind(EffectVar(0,pTarget,iEffectNumber));
  return(1);
}

protected func FxWindUSpellStop(pTarget, iEffectNumber) {
  // wind wieder sich selbst überlassen, dies geschieht spätestens in 1000 Frames
  return(1);
}

protected func FxWindUSpellTimer(pTarget, iEffectNumber,iEffectTime) {
  // Log("%d",GetWind());
  SetWind(EffectVar(0,pTarget,iEffectNumber));
  // nach gut einer Minute aufhören
  if(iEffectTime>3000) return(-1);
}


public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("433"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
