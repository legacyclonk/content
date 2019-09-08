#strict

local explosion_base;

public func CastMeteor(id idObj,int iX,int iDir,int iRange,int iExplo, int iOwner) { 
  // Standardhagel besteht aus Meteoriten
  if(!idObj) idObj=METO;
  // Log("%d is Inside %d,%d",AbsX(iX+RandomX(-iRange/2,+iRange/2)),iX-iRange/2,iX+iRange/2);
  // Einen Meteoriten gen Erdreich schleudern
  var iY=AbsX(0);
  if (!GetScenTopOpen()) iY += GetDefHeight(idObj);
  var pMeteor = CreateObject(idObj,AbsX(iX+RandomX(-iRange/2,+iRange/2)),iY, -1);
  SetSpeed(iDir*RandomX(5,30)+RandomX(-20,+20),15,pMeteor);
  // Controller korrigieren
  SetController(iOwner,pMeteor);
  // Explosionsradius anpassen
  if(iExplo && idObj==METO) pMeteor->METO::SetExplosionBase(iExplo);
}

public func Activate(object pCaster, object pRealcaster) {
  // Entfernung des Wirkungsgebietes
  var iDist=RandomX(800,1000);
  // Streuung standard (400 in jede Richtung)
  var iRange=800;

  // Wartezeit für die Zielanpeilung in Frames
  var iDelay=200;

  // Zaubernden Clonk ermitteln
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  var iDir=GetDir(pCaster)*2-1;
  var idC=GetID(Contents(0,pClonk));
  // wenn gültige Objektkomponente dabei...
  if( idC==SFLN || idC==EFLN || idC==MEGG || idC==FMEG || idC==ZAPN || idC==CNCR || idC==LIQG || idC==FRBL)
    AddEffect("MeteorDelayNSpell",Contents(0,pClonk),1,iDelay,0,GetID(),GetOwner(pClonk),pClonk,iDir);

  // ansonsten "normal" casten
  else
    {
     var iEffect=AddEffect("MeteorNSpell",0,140,15,0,GetID(),BoundBy(GetX(pCaster)+iDir*iDist,iRange/2,LandscapeWidth()-iRange/2),iDir,iRange,0);
     if(iEffect > 0)
       EffectVar(4,0,iEffect)=GetOwner(pClonk);
    }
  Sound("Magic1");
  RemoveObject();
  return(1);
}

func FxMeteorDelayNSpellStart(pTarget,iEffectNumber,iTemp,iOwner,pCaster,iDir) { 
 EffectVar(0,pTarget,iEffectNumber)=iOwner; //Besitzer für später speichern
 EffectVar(1,pTarget,iEffectNumber)=pCaster; //Caster für später speichern
 EffectVar(2,pTarget,iEffectNumber)=iDir; //Richtung des Casters für später speichern
 return(+1);
}

func FxMeteorDelayNSpellTimer() { return(-1); }

func FxMeteorDelayNSpellStop(pObject,iEffectNumber,iReason,fTemp) {
  // Entfernung des Wirkungsgebietes
  var iDist=RandomX(800,1000);
  var iRange=500;  // geringere Streuung bei der Zielanpeilung

  // nur temporäre Entfernung: nicht beachten
  if(fTemp) return();

  var idC=GetID(pObject);
  // Wenn der Caster das Objekt noch in der Hand hält, dann Richtung beachten
  var pCaster=EffectVar(1, pObject, iEffectNumber);
  var iDir=EffectVar(2, pObject, iEffectNumber);
  if(!pCaster || pObject->Contained() != pCaster) iDir = 0;
  // pObject auf Objektkomponenten untersuchen
  var iEffect=AddEffect("MeteorNSpell",0,140,15,0,GetID(),BoundBy(GetX(pObject)+iDir*iDist,iRange/2,LandscapeWidth()-iRange/2),iDir,iRange,idC);
  // Da nicht genug Parameter übergeben werden können
  if(iEffect > 0)
    EffectVar(4,0,iEffect)=EffectVar(0,pObject,iEffectNumber);
  // Objekt verbrauchen
  RemoveObject(pObject);
}

func FxMeteorNSpellStart(pTarget,iEffectNumber,iTemp,iX,iDir,iRange,idObj,iOwner) {
  // temporäre Aufrufe ignorieren
  if(iTemp) return();

  // X-Position des Hagels
  EffectVar(0,pTarget,iEffectNumber)=iX;
  // Richtung des Hagels (-1 links, 1 rechts, 0 keine)
  EffectVar(1,pTarget,iEffectNumber)=iDir;
  // Streuung des Hagels
  EffectVar(2,pTarget,iEffectNumber)=iRange; 
  // ID der herunterzufallenden Objekte
  EffectVar(3,pTarget,iEffectNumber)=idObj; 
  //EffectVar 4 ist der Controller des Objektes (die Variable muss ausserhalb dieser Funktion gesetzt werden)
  return(+1);
}

func FxMeteorNSpellTimer(pTarget,iEffectNumber,iEffectTime) {
 // Nach 200 Frames das weitere Meteoriten casten unterlassen
 if(iEffectTime>200) return(-1);

 var iOwner = EffectVar(4,pTarget,iEffectNumber);
 var idObj =  EffectVar(3,pTarget,iEffectNumber);
 var iRange = EffectVar(2,pTarget,iEffectNumber);
 var iDir =   EffectVar(1,pTarget,iEffectNumber);
 var iX =     EffectVar(0,pTarget,iEffectNumber);
 var iExplo;

 // nur weniger casten bei:
 if(idObj==ZAPN) if(Random(3))  return();
 if(idObj==FMEG) if(!Random(2)) return();
 if(idObj==MEGG) if(!Random(2)) return();
 if(idObj==FBRL) if(Random(3)) return();

 // unterschiedliche Verhaltensweisen der Komponenten
 if(idObj==SFLN)      { idObj=METO; iExplo=14; }
 else if(idObj==EFLN) { idObj=METO; iExplo=18; }
 else if(idObj==MEGG) if(!Random(10)) idObj=FMEG;
 else if(idObj==FMEG) if(!Random(4))  idObj=MEGG;
 //LIQG und CNCR bleiben so
  
 CastMeteor(idObj,iX,iDir,iRange,iExplo,iOwner);
}

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("522"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
