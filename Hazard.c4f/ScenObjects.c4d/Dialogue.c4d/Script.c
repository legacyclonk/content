/*-- NPC-Gespräch --*/

#strict

static const MENU_DecID = _MD1;

local ChatCount, pPartner;

/* Initialisierung */

protected func Initialize()
{
  ChatCount = CreateArray(GetChatCount());
  Initialized(); // Falls man Gespräche anfangs zurückhalten will
}

/* Gespräch */

public func ActivateEntrance(object pBy)
{
  SetCommand(pBy, "None");
  if(!(GetOCF(pBy) & OCF_CrewMember)) return();
  if(pPartner) return();
  // Gespräch mit Spieler
  pPartner = pBy;
  // Menü
  if(!CreateMenu(GetID(), pPartner, this(), 0, "", 0, 3, false)) return();
  // Einträge
  var szPortrait = GetMenuPortrait(this());
  // Portrait
  AddMenuItem(szPortrait, "", NONE, pPartner, 0, 0, "", 5, 0, 0);
  // Begrüßungsgelaber
  var greeting = Format("<%s>|%s", GetName(this()), Greeting());
  AddMenuItem(greeting, "", NONE, pPartner, 0, 0, "", 512, 0, 0);
  // mögliche Gespräche
  var i = 0;
  for(var chat in ChatCount)
  {
    if(!chat)
    {
      AddMenuItem(ChatHeader(i), Format("StartChat(%d)", i), GetID(), pPartner, 0, 0, 0, 512);
	  }
	  i++;
	}
	// Standard
  AddAnswer(pPartner, "$Cancel$", "EndChat", NONE,-1);
	//SetMenuDecoration();
	SetMenuTextProgress(GetLength(GetName())+1, pPartner);
}

protected func GetMenuPortrait(object pBy)
{
  var szPortrait, szText;

  if(GetOwner(pBy) != NO_OWNER)
  {
  }
  else
  {
    // Gegenstände haben, so vermute ich mal, nur 1 Portrait (ansonsten halt überladen)
    szPortrait = "1";
  }

  szText = Format("Portrait:%i::%x::%s", GetID(), GetColorDw(pBy), szPortrait);
  return(szText);
}

/* Weiterer Gesprächsverlauf */

// Dialogbox erstellen
public func CreateDialogBox(object pTarget, string szText, object pWho)
{
  // Parameter
  if(!pTarget)
    if(!(pTarget = pPartner))
      return();
  if(!szText)
    return();
  if(!pWho)
    pWho = this();
  // Menü
  if(!CreateMenu(GetID(), pPartner, this(), 0, "", 0, 3, false)) return();
  // Einträge
  var szPortrait = GetMenuPortrait(pWho);
  // Portrait
  AddMenuItem(szPortrait, "", NONE, pPartner, 0, 0, "", 5, 0, 0);
  // Was man zu sagen hat
  AddMenuItem(szText, "", NONE, pPartner, 0, 0, "", 512);
  // Standard
  //SetMenuDecoration();
	SetMenuTextProgress(GetLength(GetName(pWho))+1, pPartner);
  return(true);
} 

public func AddAnswer(object pTarget, string szText, string szFunc, id idItem, vParam)
{
  // Parameter
  if(!pTarget)
    if(!(pTarget = pPartner))
      return();
  if(!szText)
    return();
  if(!szFunc)
    return();
  // Eintrag anfügen
  if(AddMenuItem(szText, szFunc, idItem, pTarget, 0, vParam, "", 512))
    return(true);
}

public func EndChat(id dummy, int i)
{
  pPartner = 0;
  // Kann noch für besondere Dinge benutzt werden
  ChatFinished(i);
}

/* Kontrolle von Gesprächen */

public func CloseChat(int i) // Schließt ein Gespräch ab (kann dann nicht mehr geführt werden)
{
  ChatCount[i] = true;
}

public func OpenChat(int i) // Eröffnet ein Gespräch (kann dann geführt werden)
{
  ChatCount[i] = false;
}

/* Status */

public func Initialized() {} // Um Gespräche zurück zu halten
public func GetChatCount() {} // Anzahl Gespräche, die man führen kann
public func Greeting() {} // Begrüßung
public func ChatHeader(int i) {} // Gesprächsbeginne (0 - x)

public func StartChat(int i) {} // Gesprächsbeginn (0 - x)

public func MenuQueryCancel() { return(true); }

public func ChatFinished(int i) {} // Wenn Gespräche beendet werden
