/* Context Use equipment functionality */
/* Ausrüstungssteuerung */

/*
   Objekte die die volle Funktionalität benutzen wollen müssen folgende Dinge
   sicherstellen... Bis auf das erste sind alle optional. Falls definiert:
   
   * In "Construction" _inherited() aufrufen 
   
   * In "OnDmg" _inherited() aufrufen und den Rückgabewert zum eigenen
     Rückgabewert hinzuaddieren
     [falls die Ausrüstung genommenen Schaden modifizieren können soll
      z.B. die Rüstung]
     
   * In "OnHit" _inherited() aufrufen
     [falls Ausrüstung Effekte nach Schaden machen können soll
      (z.B. Aufblinken des Schildgenerators)]

   * In "Death" _inherited() aufrufen
     [falls der Clonk bei Tod die Ausrüstung fallen lassen soll]

*/

#strict

local aGear;

/* An Objekt ranhängen... */

// Array erstellen
protected func Construction()
{
  _inherited();
  aGear = CreateArray();
}

// Ausrüstung fallenlassen
protected func Death()
{
  if(HasGear()) TakeOffGear();
  _inherited();
}

// Ermöglicht die Modifikation des Schadens für das Objekt durch die Ausrüstung
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

// Effekte o.Ä. anzeigen
public func OnHit(int iDmg, int iType, object pFrom)
{
  _inherited(iDmg, iType, pFrom);
  
  // Ausrüstung will vielleicht mitreden...
  for(var gear in aGear)
    // not null...
    if(gear)
      gear->~OnClonkHit(iDmg, iType, this());
}

/* Kontext menü */

// Ausrüstung benutzen
protected func ContextUseEquipment(object pCaller) {
  [$CtxUseEquipmentDesc$|Image=HARM|Condition=HasUsableGear]
  if(!HasUsableGear()) return();
  CreateMenu(HARM,0,0,0,0,0,1);
  // nutzbare Ausrüstung anzeigen
  for(var i=0; i<GetLength(aGear); ++i) {
    var gear = aGear[i];
    if(gear) 
      if(gear->~ConUseable())
        AddMenuItem(gear->ConDesc(), "UseEquipment", GetID(gear), 0, 0, i);
  }

  return(1);
}

// Ausrüstung ablegen
protected func ContextUnbuckle(object pCaller)
{
  [$CtxUnbuckleDesc$|Image=HARM|Condition=FunnyBug]
  if(!HasGear()) return();
  CreateMenu(HARM,0,0,0,0,0,1);
  // alle Ausrüstung anzeigen
  for(var i; i<GetLength(aGear); ++i) {
    var gear = aGear[i];
    if(gear)
      AddMenuItem("$CtxUnbuckleItem$", Format("TakeOffGear(0,%d)",i), GetID(gear));
  }

  return(1);
}


/* Statusfunktionen */

// Objekt hat eine Ausrüstung angelegt die man benutzen kann
protected func HasUsableGear()
{
  // gibt true zurück wenn mindestens eine Ausrüstung
  // vorhanden ist, die per Kontextmenü nutzbar ist
  var gear;
  for(gear in aGear) {
    if(gear) 
      if(gear->~ConUseable())
        return(1);
  }
}

// Objekt hat eine Ausrüstung des Typs iGearType angelegt
//   alternativ kann man per idGear auch nach einer bestimmten
//   Ausrüstung suchen. Ohne Parameter = irgendeine ausrüstung angelegt
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
  
  // keine Parameter: allgemein ob Ausrüstung da ist
  for(gear in aGear)
    if(gear)
      return(true);
  
  return(false);
}

// Kann Ausrüstung benutzen
public func CanUse(id idObj)
{
  // Das Objekt kann natürlich Ausnahmen vornehmen....
  if(DefinitionCall(idObj, "IsHazardGear")) return(1);
  return(_inherited(idObj));
}



/* Benutzen */

// Ausrüstung benutzen
protected func UseEquipment(id bla, int i)
{
  if(aGear[i])
    aGear[i]->ConUse(this());
}

// Ausrüstung ablegen
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

  // aufsammeln wenn möglich
  if(aGear[geartype])
    Collect(aGear[geartype]);

  // und löschen
  aGear[geartype] = 0;
}

// Ausrüstung anlegen
public func EquipGear(object pGear)
{
  // überhaupt benutzbar
  if(!pGear) return(0);
  if(!(pGear->~IsHazardGear())) return(0);
  // schon eine Ausrüstung dieses Typs am Clonk
  var geartype = pGear->GetGearType();
  if(HasGear(geartype)) return(0);

  // ok:
  // speichern
  aGear[geartype] = pGear;
  // Auslösen
  return(1);
}

protected func FunnyBug() { return(HasGear()); }
