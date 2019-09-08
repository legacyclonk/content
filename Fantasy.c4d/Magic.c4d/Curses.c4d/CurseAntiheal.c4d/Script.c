/* Antiheilfluch */

#strict

/* Allgemeine Funktionen für Flüche */

static const CAHE_MaxLifetime = 150; // Frames*2 - Lebenszeit in Luft (oder Erde*5)

local pCasterClonk;

public func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;
  if (!pClonk) return(1);

  pCasterClonk = pClonk; // Caster speichern, damit es sich unter keinen Umständen selber wählen kann
  // Auswählen wenn möglich
  if (pClonk->~DoSpellSelect(this(),300)) return(1);

  var pTarget;
  // Clonk kann nicht auswählen: nächsten Gegner verfluchen
  while(pTarget=FindObject(0,-300,-300,150,150,OCF_CrewMember,0,0,NoContainer(),pTarget))
      if(SelectorTarget(pTarget))
              return(ActivateTarget(pTarget));

  // ansonsten geht nich
  RemoveObject();
  return(0);
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Typ verfluchen
  AddEffect(Format("Curse%s", GetCurseName()),pTarget,250,2,0,GetID(), GetX(pCaller), GetY(pCaller), pCaller);
  // Objekt löschen
  return(RemoveObject());
}

public func SelectorTarget(object pTarget)
{
  // nur außerhalb von Gebäuden - einzig erlaubter Container ist der Tarnstein
  if(Contained(pTarget)) if (!Contained(pTarget)->~IsCamouflageContainer()) return();
  // muss ein Mannschaftsmitglied sein
  if(!(GetOCF(pTarget)&OCF_CrewMember)) return();
  // darf nicht er selbst sein (auch wenn NTMG aktiviert ist)
  if(pTarget == pCasterClonk) return();
  // muss feindlich sein oder NTMG muss aktiviert sein
  if(!Hostile(GetOwner(),GetOwner(pTarget)) && !ObjectCount(NTMG)) return();
  // Noch nicht verflucht
  if (GetEffect("*Curse*",pTarget)) return();
  // sichtbar
  if(!Visible(GetOwner(),pTarget)) return();
  return(1);
}

protected func FxCurseStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller)
{
  if(iTemp) return();
  // Anfangsposition des Fluchs speichern
  EffectVar(0, pClonk, iEffectNumber) = iX;
  EffectVar(1, pClonk, iEffectNumber) = iY;
  EffectVar(2, pClonk, iEffectNumber) = true;
  EffectVar(3, pClonk, iEffectNumber) = Angle(iX, iY, GetX(pClonk), GetY(pClonk));
  EffectVar(4, pClonk, iEffectNumber) = pCaller;
}

protected func FxCurseTimer(pClonk, iEffectNumber, iEffectTime)
{
  // Immunitätstrank beendet Fluch	
  if (GetEffect("BanNegPotion",pClonk)) return FX_Execute_Kill;
  
  // Fliegt der Fluch noch?	
  if(!FxCurseIsActive(pClonk, iEffectNumber))
  {
    // Fluch noch im Flug
    var iX = EffectVar(0, pClonk, iEffectNumber);
    var iY = EffectVar(1, pClonk, iEffectNumber);
    var pContained = Contained(pClonk);
    if (pContained) if (pContained->~IsCamouflageContainer()) pContained=0;
    if(Distance(iX, iY, GetX(pClonk), GetY(pClonk))<6 && !pContained) // Kein Treffer in Gebäuden - dann "wartet" der Fluch draußen
    { 
      // Treffer! Fluch aktivieren
      EffectVar(2, pClonk, iEffectNumber) = false;
      RemoveEffect("Curse*", EffectVar(4, pClonk, iEffectNumber));
    }
    else
    {
      var iNewAngle = Angle(iX, iY, GetX(pClonk), GetY(pClonk));
      var iOldAngle = EffectVar(3, pClonk, iEffectNumber);
      while(iNewAngle-iOldAngle>180) iNewAngle-=360;
      while(iNewAngle-iOldAngle<-180) iNewAngle+=360;
      var angle = EffectVar(3, pClonk, iEffectNumber) = BoundBy(iNewAngle, iOldAngle-6, iOldAngle+6);
      EffectVar(0, pClonk, iEffectNumber) += Sin(angle, 5);
      EffectVar(1, pClonk, iEffectNumber) -= Cos(angle, 5);
      // Der Fluch verbraucht sich - in festem Material schneller
      var iTimeAdd = 1;
      if (GBackSolid(iX-GetX(), iY-GetY())) iTimeAdd = 5;
      if ((EffectVar(5, pClonk, iEffectNumber) += iTimeAdd) > CAHE_MaxLifetime)
        return FX_Execute_Kill;
    }
    CreateParticle("PSpark",iX,iY,0,0,180,RGBa(55,55,155, 50));
  }
  else
  {
    // Fluch aktiv: Effekte!
    CreateParticle("CurseSpark",GetX(pClonk),GetY(pClonk),0,0,180+Sin(iEffectTime, 15),RGBa(55,55,255, 150),pClonk, 1);
    var angle = RandomX(-180, 180);;
    CreateParticle("PSpark",GetX(pClonk)+Sin(angle, 15),GetY(pClonk)-Cos(angle, 15),-Sin(angle, 5),Cos(angle, 5),30,RGB(0,0,160),pClonk, 1);
  }
  return FX_OK;
}

protected func FxCurseIsActive(pClonk, iEffectNumber)
{
  return !EffectVar(2, pClonk, iEffectNumber);
}

public func NoTowerMagic() { return(true); }


/* Implementation Antiheilfluch */

public func GetCurseName() { return "Antiheal"; }

protected func FxCurseAntihealStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller)
{
  return FxCurseStart(pClonk, iEffectNumber, iTemp, iX, iY, pCaller);
}

protected func FxCurseAntihealTimer(pClonk, iEffectNumber, iEffectTime)
{
  return FxCurseTimer(pClonk, iEffectNumber, iEffectTime);
}

protected func FxCurseAntihealInfo(pClonk, iEffectNumber)
{
  if (FxCurseIsActive(pClonk, iEffectNumber)) return ("{{CAHE}} $SpellInfo$");
} 

protected func FxCurseAntihealDamage(pClonk, iEffectNumber, iDmgEngy, iCause)
{
  // Heilung wird umgekehrt
  if(!FxCurseIsActive(pClonk, iEffectNumber)) return(iDmgEngy);	
  if(iDmgEngy>0) return(-iDmgEngy/2);
  return(iDmgEngy);
}


/* Zaubercombo */

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return ("445"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
