/*-- Doorcontrol --*/

#strict

// Lokale Variablen

local closed;       // offen?
local lock;         // T�r ist verschlie�bar (geht nicht automatisch auf)
local destroyed;    // zerst�rt
local maxdmg;

// Einstellungen und Eigenschaften:

public func GetMaxDamage()		// max. Schaden
	{ return(maxdmg); }	

public func IsDestroyable()	// zerst�rbar?
	{ return(maxdmg != -1); }

public func SetMaxDamage(int m)
	{ maxdmg = m; }

public func Lock(bool aut)	{	// T�r �ffnet sich nicht mehr automtisch
	lock = 1;
	if(aut) lock = 2;
}

public func Unlock()
	{ lock = 0; }

// Callbacks

func OnDestroyed()	{ return(1); }	// zerst�rt
func OnOpen()		{ return(1); }	// ge�ffnet
func OnClose()		{ return(1); }	// geschlossen


// Initialisieren

public func Initialize()
{
  closed = true;
  Unlock();
  SetMaxDamage(50); // default
  AddEffect("CheckOpen",this(),1,5,this());
  
  UpdateTransferZone();
}

public func UpdateTransferZone()
{
  SetTransferZone(-15,-GetObjWidth()/2,30,GetObjWidth());
}

//H�nschen klein, ging allein...
public func ControlTransfer(object obj, int x, int y) {
  //Versperrt und T�re zu!
  if(lock && closed)
    //wir k�nnen nicht durch. :(
    return(false);
  
  var dir = 1;
  if(obj->GetX() < GetX())
    dir = -1;
  
  //T�re zu?
  if(closed)
    //Nahe genug herkommen, damit sie aufgeht.
      return(AddCommand(obj,"MoveTo",0,GetX()+10*dir,GetY()));
  
  //T�re offen -> Durchgehn \o/
  
}

// Ein Schloss hinzuf�gen

public func SetKeycardLock(int iID, int dir, bool autoclose)		// per Keycard
{
  var x = (dir*2-1)*(GetObjWidth()/2+15);
  var konsole = CreateObject(LKEY,x,0,GetOwner());
  konsole -> Set(this(),iID);
  Lock(autoclose);
  return konsole;
}

public func SetPasscodeLock(int a, int dir, bool autoclose)		// per Eingabecode
{
  var x = (dir*2-1)*(GetObjWidth()/2+10);
  var konsole = CreateObject(PKEY,x,0,GetOwner());
  konsole -> Set(this(),a);
  Lock(autoclose);
  return konsole;
}

public func SetSwitchLock(int dir, bool autoclose)			// per Schalter
{
  var x = (dir*2-1)*(GetObjWidth()/2+8);
  var konsole = CreateObject(SWTH,x,0,GetOwner());
  konsole -> Set(this());
  Lock(autoclose);
  return konsole;
}

public func Access() {
  if(closed)
    Open();
  else if(lock != 2)
    Close();
}

public func SwitchOn()	{ Open(); }
public func SwitchOff()	{ Close(); }

// �ffnen & Schlie�en

public func Open()					// �ffnet die T�r
{
  if(destroyed) return();
  if(!closed) return();
  OnOpen();
  closed = false;
}

public func Close()					// schlie�t die T�r
{
  if(destroyed) return();
  if(closed) return();
  OnClose();
  closed = true;
}


// Enginecalls

protected func Damage(int iChange, int iByPlayer)	// Schaden
{
  // kapusch
  if(GetDamage() > GetMaxDamage() && IsDestroyable() && !destroyed)
  {
    SetSolidMask(0);
    RemoveEffect("CheckOpen",this(),0);
    destroyed = true;
    OnDestroyed();
  }
}

public func IsBulletTarget()				// kann getroffen werden
{
    // kann nur beschossen werdne wenn zu
    if(!closed || destroyed)
        return(false);
    return(true);
}

// T�rsteuerung - automatisches �ffnen und schlie�en

public func FxCheckOpenTimer()				// Clonks suchen und T�r �ffnen
{
  // Wenn kaputt, dann gar nix mehr
  if(destroyed) return(-1);
  // wenn verschlie�bar, T�r �ffnet und schlie�t nicht automatisch
  if(lock == 1) return();

  // wieder schlie�en
  if(!closed) {
    if(!SomeonesApproaching())
      Close();
  }
  // �ffnen
  else if(!lock) {
    if(SomeonesApproaching())
      Open();
  }
}

private func SomeonesApproaching() {			// such ankommene Clonks

  var aClonks = CreateArray();

  for(var i = 0; i < 2; ++i)
  {
     // Suchen wir mal rum.
     aClonks = FindObjects(Find_InRect(-35 * i, -GetObjHeight() / 2, 35, GetObjHeight()),
//			   Find_NoContainer(),
			   Find_OCF(OCF_Alive),
			   Find_Not(Find_Func("IsAlien")) );
     // Irgendwas gefunden?
     if(GetLength(aClonks) > 0)
     {
       return(true);
     }
  }
  return(false);
}

public func IsSideDoor() { return(true); }

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetMaxDamage(%d)", maxdmg);
	if (lock)
		extra[GetLength(extra)] = Format("Lock(%v)", (lock == 2));
	if (destroyed) {
		extra[GetLength(extra)] = "SetSolidMask()";
		extra[GetLength(extra)] = "RemoveEffect(\"CheckOpen\",this(),0)";
		extra[GetLength(extra)] = "LocalN(\"destroyed\")=true";
	}
}
