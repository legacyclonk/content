/* Zauberstab */

#strict

local clonk;
local spells;//Array mit toll ID Liste von Zaubern
protected func Initialize()
{
  RandSpells();
  DoMagicEnergy((GetPhysical("Magic")/1000)/2);
  
  //AddEffect("WandsColor",this(),20,6,this());
    return(1);
    
}

protected func Entrance(clonktemp)
{
    clonk=clonktemp;
    SetOwner(GetOwner(clonktemp));
    return(1);
    
}

public func Activate(pCaller)
{
  [Zaubern|Image=MCMS|Condition=ReadyToMagic]
  // Nicht bereit
  if(!ReadyToMagic(pCaller)) return(0);
  // Clonk soll anhalten
    SetComDir(COMD_Stop(),pCaller);
  // Menü öffnen
  CreateMenu(MCMS,pCaller,0,3,"Keine Zaubersprüche verfügbar",GetMagicEnergy());
 
  for(var cnt=0;cnt<GetLength(spells);cnt++)
    { 
     AddMenuItem( "Zaubern: %s", "DoMagic", spells[cnt], pCaller);
     
    }
  return(1);
}

protected func ReadyToMagic(pCaller)
{
  // Nur wenn der Clonk steht, springt oder schwimmt
  if(GetAction(pCaller) ne "Walk" && GetAction(pCaller) ne "Jump" && GetAction(pCaller) ne "Swim") return(0);
  return(1);
}

protected func DoMagic(id idSpell)
{
  // Nicht bereit
  if (!ReadyToMagic(clonk)) return();
  // Nicht genug Zauberenergie
  if (GetMagicEnergy()<Value(idSpell) )
    return( Message("Nicht genug|Zauberenergie!",this()), Sound("Error") );
  // Zauberobjekt erschaffen und aktivieren
  if (!ObjectCall(CreateObject(idSpell,0,0,GetOwner()),"Activate",clonk))
    // Zauber fehlgeschlagen
    return(0);
  // Zauberenergie abziehen
  DoMagicEnergy(-Value(idSpell));
  //wenn Zauber teuer aus array löschen
  if(GetValue(0,idSpell)>19)
  if(GetArrayItemPosition(idSpell,spells)!=-1)
  DeleteArrayItem(GetArrayItemPosition(idSpell,spells),spells);
  // Fertig
  return(1);
}

protected func ReloadMagic()
{
  if(GetMagicEnergy()<GetPhysical("Magic")/1000) DoMagicEnergy(1);
    return(1);
    
}

protected func RandSpells()//Geht btw von den Zaubern von SPielerByIndex 0 aus.
{
 
 spells=[];
 //Standard ist immer löschen
 spells[GetLength(spells)]=EXTG;
 var sID;
 for(var cnt;sID=GetPlrMagic(GetPlayerByIndex(0),0,cnt);cnt++) 
 {
  //Mijon tolle Zauber eh nicht.
  if(GetValue(0,sID)>=70)continue;
  //Zufall und so
  if(Random(GetLength(spells)))continue;
  //Gibts vielleicht schon? maggi!
  if(GetArrayItemPosition(sID,spells) != -1)continue;
  //Tjo, dann adden
  spells[GetLength(spells)]=sID;
 }

 //alles toll und so
 return(1); 
}


/*protected func FxWandsColorStart(pTarget, iEffectNumber)
{
 //r und g und b
 EffectVar(0, pTarget, iEffectNumber)=128;//<- r
 EffectVar(1, pTarget, iEffectNumber)=128;//<- g
 EffectVar(2, pTarget, iEffectNumber)=128;//<- b :O!
 
 //Hilfsteil zum verändern
 EffectVar(5, pTarget, iEffectNumber)=0;
}

protected func FxWandsColorTimer(pTarget, iEffectNumber)
{

 EffectVar(0, pTarget, iEffectNumber)=BoundBy(EffectVar(0, pTarget, iEffectNumber)+EffectVar(5, pTarget, iEffectNumber),32,227);
 EffectVar(1, pTarget, iEffectNumber)=BoundBy(EffectVar(1, pTarget, iEffectNumber)-EffectVar(5, pTarget, iEffectNumber),32,227);
 EffectVar(2, pTarget, iEffectNumber)=BoundBy(EffectVar(2, pTarget, iEffectNumber)+Cos(EffectVar(5, pTarget, iEffectNumber),1),32,227);
 
 if(!Random(20))
 if(Random(2))
 EffectVar(5, pTarget, iEffectNumber)=BoundBy(EffectVar(5, pTarget, iEffectNumber)-1,-1,1);
 else
 EffectVar(5, pTarget, iEffectNumber)=BoundBy(EffectVar(5, pTarget, iEffectNumber)+1,-1,1);
 
 SetColorDw(RGB(EffectVar(0, pTarget, iEffectNumber),EffectVar(1, pTarget, iEffectNumber),EffectVar(2, pTarget, iEffectNumber)));
 
 //Hilfsteil zum verändern
 EffectVar(5, pTarget, iEffectNumber)=0;
}*/

protected func Incineration()
{
 CastParticles("FSpark",3,8,0,0,20,60,RGB(200,200,200),RGB(255,255,255));
 
 CastParticles("Fire",3,8,0,0,20,60,RGB(200,200,200),RGB(255,255,255));
 CastParticles("Fire2",3,8,0,0,20,60,RGB(200,200,200),RGB(255,255,255));
 RemoveObject();
}



