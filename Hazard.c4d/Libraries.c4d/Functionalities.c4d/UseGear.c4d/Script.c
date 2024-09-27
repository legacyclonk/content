/* Context Use equipment functionality */
/* Ausr�stungssteuerung */

/*
   Objekte die die volle Funktionalit�t benutzen wollen m�ssen folgende Dinge
   sicherstellen... Bis auf das erste sind alle optional. Falls definiert:
   
   * In "Construction" _inherited() aufrufen 
   
   * In "OnDmg" _inherited() aufrufen und den R�ckgabewert zum eigenen
     R�ckgabewert hinzuaddieren
     [falls die Ausr�stung genommenen Schaden modifizieren k�nnen soll
      z.B. die R�stung]
     
   * In "OnHit" _inherited() aufrufen
     [falls Ausr�stung Effekte nach Schaden machen k�nnen soll
      (z.B. Aufblinken des Schildgenerators)]

   * In "Death" _inherited() aufrufen
     [falls der Clonk bei Tod die Ausr�stung fallen lassen soll]

*/

#strict

local aGear;

/* An Objekt ranh�ngen... */

// Array erstellen
protected func Construction()
{
  _inherited();
  aGear = CreateArray();
}

// Ausr�stung fallenlassen
protected func Death()
{
  if(HasGear()) TakeOffGear();
  _inherited();
}

// Erm�glicht die Modifikation des Schadens f�r das Objekt durch die Ausr�stung
public func OnDmg(int iDamage, int iType) {
  var before = _inherited(iDamage,iType);
  
  var add = before;
  var gear;
  for(gear in aGear) {
    // not null...
    if(gear) {
      add += (gear->~OnClonkDmg(iDamage, iType, this()));
    } 
  }
  return(add);
}

// Effekte o.�. anzeigen
public func OnHit(int iDmg, int iType, object pFrom)
{
  _inherited(iDmg, iType, pFrom);
  
  // Ausr�stung will vielleicht mitreden...
  for(var gear in aGear)
    // not null...
    if(gear)
      gear->~OnClonkHit(iDmg, iType, this());
}

/* Kontext men� */

// Ausr�stung benutzen
protected func ContextUseEquipment(object pCaller) {
  [$CtxUseEquipmentDesc$|Image=HARM|Condition=HasUsableGear]
  if(!HasUsableGear()) return();
  CreateMenu(HARM,0,0,0,0,0,1);
  // nutzbare Ausr�stung anzeigen
  for(var i=0; i<GetLength(aGear); ++i) {
    var gear = aGear[i];
    if(gear) 
      if(gear->~ConUseable())
        AddMenuItem(gear->ConDesc(), "UseEquipment", GetID(gear), 0, 0, i);
  }

  return(1);
}

// Ausr�stung ablegen
protected func ContextUnbuckle(object pCaller)
{
  [$CtxUnbuckleDesc$|Image=HARM|Condition=FunnyBug]
  if(!HasGear()) return();
  CreateMenu(HARM,0,0,0,0,0,1);
  // alle Ausr�stung anzeigen
  for(var i; i<GetLength(aGear); ++i) {
    var gear = aGear[i];
    if(gear)
      AddMenuItem("$CtxUnbuckleItem$", Format("TakeOffGear(0,%d)",i), GetID(gear));
  }

  return(1);
}


/* Statusfunktionen */

// Objekt hat eine Ausr�stung angelegt die man benutzen kann
protected func HasUsableGear()
{
  // gibt true zur�ck wenn mindestens eine Ausr�stung
  // vorhanden ist, die per Kontextmen� nutzbar ist
  var gear;
  for(gear in aGear) {
    if(gear) 
      if(gear->~ConUseable())
        return(1);
  }
}

// Objekt hat eine Ausr�stung des Typs iGearType angelegt
//   alternativ kann man per idGear auch nach einer bestimmten
//   Ausr�stung suchen. Ohne Parameter = irgendeine ausr�stung angelegt
public func HasGear(int iGearType, id idGear)
{
  // nach bestimmter ID suchen
  var gear;
  if(idGear) {
    for(gear in aGear)
	  if(gear)
	    if(GetID(gear) == idGear)
	      return(gear);
    return(false);
  }

  // nach bestimmtem Typ suchen
  if(iGearType) {
    if(aGear[iGearType])
      return(true);
    return(false);
  }
  
  // keine Parameter: allgemein ob Ausr�stung da ist
  for(gear in aGear)
    if(gear)
      return(true);
  
  return(false);
}

// Kann Ausr�stung benutzen
public func CanUse(id idObj)
{
  // Das Objekt kann nat�rlich Ausnahmen vornehmen....
  if(DefinitionCall(idObj, "IsHazardGear")) return(1);
  return(_inherited(idObj));
}



/* Benutzen */

// Ausr�stung benutzen
protected func UseEquipment(id bla, int i)
{
  if(aGear[i])
    aGear[i]->ConUse(this());
}

// Ausr�stung ablegen
public func TakeOffGear(object pGear, int iGearType)
{
  var geartype = iGearType;

  // man kann auch statt iGearType das konkrete Objekt angeben
  if(!geartype) {

    // ohne Parameter = alles runter
    if(!pGear) {
      var i;
      for(var i=0; i<GetLength(aGear); ++i)
        if(aGear[i])
          TakeOffGear(0,i);
      return(1);
    }

    geartype = pGear->GetGearType();
    // das Objekt ist nicht angelegt
    if(aGear[geartype] != pGear) return(0);
  }
  else {
    // angegebener geartype garnicht angelegt 
    if(!HasGear(geartype)) return(0);
  }
  
  // er ist angelegt: ok, ablegen
  aGear[geartype]->GearUnbuckle(this());

  // aufsammeln wenn m�glich
  if(aGear[geartype])
    Collect(aGear[geartype]);

  // und l�schen
  aGear[geartype] = 0;
}

// Ausr�stung anlegen
public func EquipGear(object pGear)
{
  // �berhaupt benutzbar
  if(!pGear) return(0);
  if(!(pGear->~IsHazardGear())) return(0);
  // schon eine Ausr�stung dieses Typs am Clonk
  var geartype = pGear->GetGearType();
  if(HasGear(geartype)) return(0);

  // ok:
  // speichern
  aGear[geartype] = pGear;
  // Ausl�sen
  return(1);
}

protected func FunnyBug() { return(HasGear()); }
