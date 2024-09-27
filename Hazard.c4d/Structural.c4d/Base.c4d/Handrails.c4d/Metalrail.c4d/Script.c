/*-- Metalrail --*/

#strict

/* Grundobjekt f�r andere Rails */

/*
Die einzelnen Teile des Gel�nders bekommen einen Index zugewiesen und
k�nnen dann theoretisch beliebig kombiniert werden.
Dabei werden folgende Kriterien vorausgesetzt:
Index 0 entspricht dem linken Endst�ck
Index 1 ist ein beliebig Mittelst�ck (kein Pfeiler!)
Der letzte Index (= func LastIndex() ) entspricht dem rechten Endst�ck
*/

// hat in regelm��igen Abst�nden Pfeiler.
// Funktioniert nicht mit Gel�ndern die keine St�tzen haben (RAI3)
static const RAIL_Supported = 1;		

static const RAIL_LeftOpen = 2;		// hat keinen Pfeiler am Anfang (steckt wohl links in der Wand)
static const RAIL_RightOpen = 4;		// hat keinen Pfeiler am Ende (" rechts ")

static const RAIL_AltLinkage = 8;		// alternativer Gel�nderstil (wenn vorhanden)


/* Status */

public func Index(int index) {
	if(index == 0) return("End1");
	if(index == 1) return("Linkage1");
	if(index == 2) return("Linkage2");
	if(index == 3) return("Backing");
	if(index == 4) return("End2");
}

public func Left()		{ return(0); }
public func Right()	{ return(4); }
public func Linkage(i)	{ return(1+(!!i)); }
public func Backing()	{ return(3); }

/* �ffentliches */

// Alias
public func Set(a,b) { return(SetRailLength(a,b)); }

// Die einfachste Art, ein Gel�nder auszurichten. Setzt eine bestimmte L�nge Standardteile (Index 1)
public func SetRailLength(int iLength, int iMod)
{
  ClearRail();
  parttype = CreateArray();
  
  var i = 0;
  // Anfangspfosten setzen, falls kein Modifier gesetzt ist
  if(!(iMod & RAIL_LeftOpen))
    parttype[i++] = Left();
  else
    iLength--;
    
  var support = false, now = false;
  if(iMod & RAIL_Supported) 
    if(Backing() != -1) {
      support = true;
    }
    
  while(i < iLength+1) {
    if(support && now) {
	    parttype[i] = Backing();
	    now = false;
	    iLength++;
	}
    else {
      if(iMod & RAIL_AltLinkage)
        parttype[i] = Linkage(1);
      else
        parttype[i] = Linkage(0);
      now = true;
    }
    i++;
  }
    
  // Endposten setzen, falls kein Modifier gesetzt ist
  if(!(iMod & RAIL_RightOpen))
    parttype[GetLength(parttype)] = Right();

  DrawRail();
}

// Setzt eine Abfolge von Teilst�cken, Anfang und Ende werden automatisch eingesetzt
public func SetRail(array parts, bool nop)
{
  ClearRail();
  parttype = CreateArray();
  var i = 0, j = 0;
  if(!nop) parttype[i++] = Left();
  for( ; j < GetLength(parts) ; i++) {
    parttype[i] = parts[j++];
  }
  if(!nop) parttype[GetLength(parttype)] = Right();
  DrawRail();
}

// Manipuliert einzelne Teile (um z.B. Anfang oder Ende zu entfernen
public func AdjustPart(int iPart, int iNewIndex)
{
  ClearRail();
  // Sch�tz vor �berl�nge
  if(GetLength(parttype) <= iPart || iPart < 0) return();
  // Manipulieren
  parttype[iPart] = iNewIndex;
  DrawRail();
  return(1);
}

/* Internes */

local length, // L�nge
    parttype; // Array mit den Typen der einzelnen Teile

protected func Initialize()
{
  // Initialisieren
  parttype = CreateArray();
  // Erstmal Standardgr��e setzen
  parttype = [Left(), Linkage(), Right()];
  // Zeichnen
  DrawRail();
}

// Neu zeichnen
protected func DrawRail()
{
  // Gesamtl�nge in Pixeln
  var alength = 0;
  // L�nge aktualisieren
  length = GetLength(parttype);
  // Einzelne Teile ansetzen
  for(var i = 0, action, plength ; i < length ; i++)
  {
    // Aktion speichern
    action = Index(parttype[i]);
    // X-L�nge der Aktion
    plength = GetActMapVal("Facet", action, GetID(), 2);
    // Grafik einpflanzen
    SetGraphics("Rail", this(), GetID(), i+1, 2, action);
    // Transformieren
    SetObjDrawTransform(1000,0, 1000 * (plength/2 + alength), 0,1000,0,0, i+1);
    // Gr��e des Teils draufrechnen
    alength += plength;
  }
  SetShape(0,-GetObjHeight(this())/2, alength, GetObjHeight(this()));
}

// Bisheriges L�schen
protected func ClearRail()
{
  for(var i = 0 ; i < GetLength(parttype) ; i++)
    SetGraphics("", this(), 0, i+1);
}

// Schutz vor Neuladen
protected func UpdateTransferZone()
{
  ClearRail();
  DrawRail();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetRail(%v,1)", parttype);
}
