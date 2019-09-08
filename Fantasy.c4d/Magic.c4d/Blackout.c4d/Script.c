#strict

// EffectVar 0: Effect size

public func Activate(object pCaster, object pRealcaster) {
  // Effekte!
  Sound("Magic*", 0, this(), 70);
  var iRange=100;
  var iDuration = 37;
  var iAng, iX, iY, iRng;
  // Kombo: Stein verlängert Dauer
  var pComboObj;
  if (pComboObj = FindContents(ROCK, pCaster))
    {
    iDuration *= 2;
    RemoveObject(pComboObj);
    }
  // Kombo: Fisch und Kohle vergrößert Radius
  pComboObj = FindContents(FISH, pCaster);
  if (!pComboObj) pComboObj = FindContents(DFSH, pCaster);
  if (!pComboObj) pComboObj = FindContents(COAL, pCaster);
  if (pComboObj)
    {
    if (GetAlive(pComboObj))
      iRange *= 3;
    else
      iRange *= 2;
    RemoveObject(pComboObj);
    }
  for(var i=iRange/2; i>0; --i)
  {
    iAng=Random(360);
    iRng=Random(iRange/2);
    iX=Sin(iAng, iRng);
    iY=Cos(iAng, iRng);
    CreateParticle("Feather", iX, iY, RandomX(-5,5), -3, 50+Random(25));
  }  
  var pClonk,pVictim;
  if(!(pClonk=pRealcaster)) pClonk=pCaster;
  // alle Clonks im näheren Umkreis prüfen
  while(pVictim=FindObject(0,-iRange/2,-iRange/2,iRange,iRange,OCF_CrewMember(),0,0,NoContainer(),pVictim))
    // die Opfer sind feindliche Clonks oder alle Clonks außer dem Zauberer selbst wenn NoTeamMagic aktiviert ist
    if((Hostile(GetOwner(pVictim),GetOwner(pClonk)) || ObjectCount(NTMG)) && pClonk!=pVictim)
      // wirkt länger bei heruntergekämpften Clonks
      AddEffect("SleepNSpell",pVictim,123,25,0,GetID(),iDuration*((115-GetEnergy(pVictim)*100000/GetPhysical("Energy",0, pVictim))/5));
  return(1);
}

protected func FxSleepNSpellStart(object pTarget, int iEffectNumber, int iTemp, int iTime) {
  // temporäre aufrufe
  if(iTemp) return();
  // Scheintoter Zustand
  ObjectSetAction(pTarget,"Dead");
  // mit schnarchen anfangen
  Sound("Snore",0,pTarget,0,0,+1);
  // Zeit bis zum Aufwachen speichern
  EffectVar(0,pTarget,iEffectNumber)=iTime;
  // Fertig
  return(1);
}

protected func FxSleepNSpellEffect(szNewEffect, pTarget, iEffectNumber)
  {
  // Wenn schon ein Schlafzauber wirkt, darf kein neuer dazukommen
  if (szNewEffect eq "SleepNSpell") return(-1);
  // Sonst keine Behandlung
  return();
  }

protected func FxSleepNSpellStop(object pTarget, int iEffectNumber) {
  // clonk hat den Schlaf nicht überstanden ;)
  if(!GetAlive(pTarget)) return(1);
  // aufstehen
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  // mit schnarchen aufhören
  Sound("Snore",0,pTarget,0,0,-1);
  // stehenbleiben
  SetComDir(COMD_Stop(),pTarget);
  // Fertig
  return(1);
}

protected func FxSleepNSpellTimer(object pTarget, int iEffectNumber, int iTime) {
  var iMaxTime=EffectVar(0,pTarget,iEffectNumber);
  // nach einer bestimmten Zeit aufhören
  if(iTime>=iMaxTime) return(-1);
  // Zzz-Partikel casten
  CreateParticle("Zzz",GetX(pTarget),GetY(pTarget),2+(iTime%50)*3/25+RandomX(-1,+1),-5,60,
                 RGBa(255,255,255,(iTime-iMaxTime/2)**2 * 180*4/iMaxTime**2));
}

protected func FxSleepNSpellDamage(object pTarget, int iEffectNumber, int iDmgEngy) {
  // Bei Damage aufwachen
  RemoveEffect(0,pTarget,iEffectNumber);
  return(iDmgEngy);
}

protected func FxSleepNSpellInfo() { return("{{MBOT}} $SpellInfo$"); }

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("222"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
