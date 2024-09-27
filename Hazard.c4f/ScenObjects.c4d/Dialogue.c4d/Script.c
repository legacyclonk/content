/*-- NPC-Gespr�ch --*/

#strict

static const MENU_DecID = _MD1;

local ChatCount, pPartner;

/* Initialisierung */

protected func Initialize()
{
  ChatCount = CreateArray(GetChatCount());
  Initialized(); // Falls man Gespr�che anfangs zur�ckhalten will
}

/* Gespr�ch */

public func ActivateEntrance(object pBy)
{
  SetCommand(pBy, "None");
  if(!(GetOCF(pBy) & OCF_CrewMember)) return();
  if(pPartner) return();
  // Gespr�ch mit Spieler
  pPartner = pBy;
  // Men�
  if(!CreateMenu(GetID(), pPartner, this(), 0, "", 0, 3, false)) return();
  // Eintr�ge
  var szPortrait = GetMenuPortrait(this());
  // Portrait
  AddMenuItem(szPortrait, "", NONE, pPartner, 0, 0, "", 5, 0, 0);
  // Begr��ungsgelaber
  var greeting = Format("<%s>|%s", GetName(this()), Greeting());
  AddMenuItem(greeting, "", NONE, pPartner, 0, 0, "", 512, 0, 0);
  // m�gliche Gespr�che
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
    // Gegenst�nde haben, so vermute ich mal, nur 1 Portrait (ansonsten halt �berladen)
    szPortrait = "1";
  }

  szText = Format("Portrait:%i::%x::%s", GetID(), GetColorDw(pBy), szPortrait);
  return(szText);
}

/* Weiterer Gespr�chsverlauf */

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
  // Men�
  if(!CreateMenu(GetID(), pPartner, this(), 0, "", 0, 3, false)) return();
  // Eintr�ge
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
  // Eintrag anf�gen
  if(AddMenuItem(szText, szFunc, idItem, pTarget, 0, vParam, "", 512))
    return(true);
}

public func EndChat(id dummy, int i)
{
  pPartner = 0;
  // Kann noch f�r besondere Dinge benutzt werden
  ChatFinished(i);
}

/* Kontrolle von Gespr�chen */

public func CloseChat(int i) // Schlie�t ein Gespr�ch ab (kann dann nicht mehr gef�hrt werden)
{
  ChatCount[i] = true;
}

public func OpenChat(int i) // Er�ffnet ein Gespr�ch (kann dann gef�hrt werden)
{
  ChatCount[i] = false;
}

/* Status */

public func Initialized() {} // Um Gespr�che zur�ck zu halten
public func GetChatCount() {} // Anzahl Gespr�che, die man f�hren kann
public func Greeting() {} // Begr��ung
public func ChatHeader(int i) {} // Gespr�chsbeginne (0 - x)

public func StartChat(int i) {} // Gespr�chsbeginn (0 - x)

public func MenuQueryCancel() { return(true); }

public func ChatFinished(int i) {} // Wenn Gespr�che beendet werden
