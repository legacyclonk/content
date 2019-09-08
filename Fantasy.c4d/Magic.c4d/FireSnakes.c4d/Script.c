/* Feuerschlangen */

#strict

public func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;
  
  var iResult;
  if (iResult = CheckEffect("FiresnakesNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  
  // Auswählen wenn möglich
  if (pCaster->~DoSpellSelect(this(),300)) return(1);

  var pTarget;
  // Clonk kann nicht auswählen: nächsten Gegner verfluchen
  while(pTarget=FindObject(0,-300,-300,150,150,OCF_CrewMember,0,0,NoContainer(),pTarget))
      if(SelectorTarget(pTarget))
              return(ActivateTarget(pTarget));
 
  ActivateTarget(pTarget);
  
  return(1);
}

public func ActivateTarget(object pCaller, object pTarget)
{
  SetPosition(GetX(pCaller), GetY(pCaller));
  // 3-5 Schlangen beschwören
  var i = RandomX(3,5);
  while(i--)
  {
    var obj = CreateObject(FSKE, 0, 0, GetOwner(pCaller));	  
    obj->~SetTarget(pTarget);
  }
  // Objekt löschen
  return(RemoveObject());
}

public func SelectorTarget(object pTarget)
{
  // nur außerhalb von Gebäuden
  if(Contained(pTarget)) return();
  // muss ein Mannschaftsmitglied sein
  if(!(GetOCF(pTarget)&OCF_CrewMember)) return();
  // muss feindlich sein oder NTMG muss aktiviert sein
  if(!Hostile(GetOwner(),GetOwner(pTarget)) && !ObjectCount(NTMG)) return();
  // sichtbar
  if(!Visible(GetOwner(),pTarget)) return();
  return(1);
}


/* Zaubercombo */

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("163"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
