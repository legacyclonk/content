/* Assault */

#strict
#include TEAM

local targets;      // Die Ziele
local targetstatus; // Status der Ziele
local targetopt;    // Ob das Ziel optional ist

local timer1;
local timer2;
local phase, phasechange;

local finished;

local attacker, defender;

global func AS_GetAttackerTeam()
{
  var goal = FindObject(GAS_);
  if(GetID() == GAS_) goal = this;
  if(!goal) return;
  return LocalN("attacker", goal);
}

global func AS_GetDefenderTeam()
{
  var goal = FindObject(GAS_);
  if(GetID() == GAS_) goal = this;
  if(!goal) return;
  return LocalN("defender", goal);
}

protected func Initialize()
{
  targets = CreateArray();
  targetstatus = CreateArray();
  targetopt = CreateArray();

  // secTime = Zeit die noch übrig ist in Sekunden
  timer1 = 0;
  timer2= 0;
  phase = 0;
  iWinScore = 1;
  finished = false;

  // Angreifer ist erstmal Team 1
  attacker = 1;
  // Verteidiger ist demnach Team 2
  defender = 2;

  return(_inherited());
}

public func AddTarget(object newtarget, int opt, int maxdmg, bool nobar)
{
  // kein Ziel? WTF?!
  if(!newtarget)
    return();

  // Ziel adden
  var index = GetLength(targets);
  targets[index] = newtarget;
  targetstatus[index] = 1;
  targetopt[index] = opt;
  
  // Effekt hinzufuegen == Zerstoerungskontrolle
  AddEffect("AS_Target",newtarget, 42, 0, this(), GAS_, maxdmg, nobar);

	newtarget->SetTeam(AS_GetDefenderTeam());

  //das wars!
  return(index+1);
}

public func SetTargetStatus(int Index, int status, int maxdmg, bool nobar)
{
  // Index muss >0 und <=GetLength() sein.
  if(GetLength(targetstatus) < Index && Index <= 0)
    return(0); //wtf? kein target? einfach 0 zurueckgeben.
  
  if(status)
  {
    //existiert das Objekt noch?
    if(!targets[Index])
      return();
    
    AddEffect("AS_Target",targets[Index], 42, 0, this(), GAS_, maxdmg, nobar);
  }
  else
  {
    RemoveEffect("AS_Target",targets[Index],GetEffect("AS_Target",targets[Index]));
  }
  targetstatus[Index]=status;
}

public func GetTargetStatus(int index)
{
  // *copy'n'paste*
  // Index muss >0 und <=GetLength() sein.
  if(GetLength(targetstatus) < index && index <= 0)
    return(0); //wtf? kein target? einfach 0 zurueckgeben.
  
  return(targetstatus[index-1]);
}

public func GetAssaultPhase() {
	return(phase);
}

public func GetHUDInfo(int player) {

	return(Format("<c %x>%d:%.2d</c> - <c %x>%d:%.2d</c>",GetTeamColor(1), timer2/60, timer2%60, GetTeamColor(2), timer1/60, timer1%60));
}

/*
public func SetDamageMask(int index, int dmgmask)
{
  // *copy'n'paste*
  // Index muss >0 und <=GetLength() sein.
  if(GetLength(targetstatus) < index && index <= 0)
    return(0); //wtf? kein target? einfach 0 zurueckgeben.
    
  var obj = targets[index-1];
  var nr = GetEffect("AS_Target", obj);
  EffectVar(1, obj, nr) = dmgmask;
}

public func GetDamageMask(int index )
{
  // *copy'n'paste*
  // Index muss >0 und <=GetLength() sein.
  if(GetLength(targetstatus) < index && index <= 0)
    return(0); //wtf? kein target? einfach 0 zurueckgeben.

  var obj = targets[index-1];
  var nr = GetEffect("AS_Target", obj);
  return(EffectVar(1, obj, nr));
} */

/* Effekte!!!! */

// Variablen:
// 0 - Maximaler Schaden
// 1 - nicht benutzt
// 2 - Farbe des Balkens

// Anfang
private func FxAS_TargetStart(object pTarget, int iEffectNumber, int iTemp, int maxdmg, bool nobar)
{
  if(iTemp)
    return();

	var index;
	for(index = 0; index < GetLength(targets); index++)
	if(targets[index] == pTarget)
		break;

  // Lebensbalken erstellen?
  if(!nobar)
  {
    var color = RGBa(230,10,10,50);
    if(targetopt[index] == 1)
      color = RGBa(180,230,10,50);
    if(targetopt[index] == 2)
      color = RGBa(10,230,180,50);

		var bar = CreateObject(EBAR,0,0,-1);
		bar->Set(pTarget, color);
    EffectVar(2, pTarget, iEffectNumber) = bar;
  }

  EffectVar(0, pTarget, iEffectNumber) = maxdmg;
  
  FxAS_TargetUpdate(pTarget, iEffectNumber, 0);
}

// Und Ende?
private func FxAS_TargetDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  var eng = pTarget->GetDamage() + iDmgEngy;
  if(GetOCF(pTarget) & OCF_Living)
    eng = pTarget->GetPhysical("Energy")/1000 - pTarget->GetEnergy() - iDmgEngy/1000;
  
  // Lebensanzeige und evtl. Status updaten!
  FxAS_TargetUpdate(pTarget, iEffectNumber, (eng*100)/EffectVar(0, pTarget, iEffectNumber));
  
  // Schaden durchgehen lassen.
  return(iDmgEngy);
}

private func FxAS_TargetUpdate(object pTarget, int iEffectNumber, int percent)
{
  // Lebensbalken updaten
  if(EffectVar(2, pTarget, iEffectNumber))
    EffectVar(2, pTarget, iEffectNumber)->Update(Max(0,100-percent));
  
  /* macht das dings selbst 
  if(percent >= 100)
  {
    pTarget->~Damage();
    RemoveEffect(0, pTarget, iEffectNumber);
    return(-1);
  }*/
  return();
}

// Das wahre Ende.
private func FxAS_TargetStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp)
    return;

  // Nichts tun wenn gerade die Phase gewechselt wird
  if(phasechange) return;

  // stirb Lebensbalken, stirb!
  RemoveObject(EffectVar(2, pTarget, iEffectNumber));

  var index, count;
  for(index = 0; index < GetLength(targets); index++)
    if(targets[index] == pTarget)
      break;
  
  var count;
  for(var i = 0; i <= index; i++)
  	if(!targetopt[i])
  		count++;
  
  // Ziel zerstört!
  targetstatus[index] = false;
  
  // Das Spiel benachrichtigen. :D
  GameCall(Format("OnTarget%dDestruction",index+1));
  // Nachricht ausgeben
  if(targetopt[index] == 0)
  	Log("$TargetDestroyed$", count, GetName(pTarget));
  if(targetopt[index] == 1)
  	Log("$OptTargetDestroyed$", GetName(pTarget));
  
  Sound("TargetDestroyed",1);
  
	// Geld!
	DoWealth(attacker,50);
	
  // alle Hauptziele put?
  var end = true;
  for(index = 0; index < GetLength(targetstatus); index++)
    // Ziel noch am leben und nicht optional?
    if(targetstatus[index] && !targetopt[index])
    {
      end = false;
      break;
    }
  
  if(end)
  {
	// Phase 0 ist zuende... Phase 1 einleiten!!!! (zweites Team greift an)
	if(phase == 0) {

	    phasechange = true;
			phase = 1;

	    targets = CreateArray();
	    targetstatus = CreateArray();
	    targetopt = CreateArray();

	    Log("$TeamChange$", GetTeamName(AS_GetAttackerTeam()), timer1/60, timer1%60, GetTeamName(AS_GetDefenderTeam()));
	    Sound("TeamChange", 1);

		// Teams austauschen
		attacker = 2;
		defender = 1;

	    // Anderer GameCall, den man für die Teamstartpositionen benutzen sollte
	    GameCall("PreResetAssault");

	    // Alles löschen --- ##### !!!!! Gefahr!
	    for(var obj in FindObjects(Find_Not(Find_ID(GOAL)),			// und hier hats geknallt
								   Find_Not(Find_Category(C4D_Goal)),
	                               Find_Not(Find_Category(C4D_Rule)),
	                               Find_Not(Find_Category(C4D_Environment)),
	                               Find_Not(Find_Func("NoAssaultRemove")))) // Damit werden Objekte nicht gelöscht!
	      if(obj) // Nullpointerschutz falls Objekte ihre Hilfsobjekte löschen
	      {
	        if(obj->GetOCF() & OCF_CrewMember) obj->~NoDeathAnnounce();
	        obj->RemoveObject(0, true);
	      }

			// Alles vom Szenario neu erzeugen lassen
			Schedule("GameCall(\"ResetAssault\")",1,0);

	    phasechange = false;
	}
	// Phase 2: Spiel ist zuende und das Angreiferteam der zweiten Runde hat gewonnen.
	else if(phase == 1) {
		if(!finished) {
			DoTeamScore(attacker, 1);
			finished = true;
		}
	}
  }
}

/* Steuerung */

func Timer()
{
	if(FindObject(CHOS))
		return();

  //OMGROFLOLHA$X=RFURLOL X-DD!11! :DD!ORLGMF!
  // ^- Überreste einer Gobby-Sitzung .
  
  if(phase == 0) timer1++;
  else timer2++;

  // in Phase 2 hat das Angreiferteam versagt.
  if(timer2 > timer1 && phase == 1)
  { 
    //CustomMessage(Format("Win!"),0,NO_OWNER,0,0,GetTeamColor(2),0,0,MSG_NoLinebreak);

	if(!finished) {
		DoTeamScore(defender, 1);
		finished = true;
	}
  }
  InitScoreboard();
  UpdateHUDs();
}

private func InitScoreboard()
{
  //var iColID = TEAM_GoalColumn;

  SetScoreboardData(SBRD_Caption, SBRD_Caption,       Format("{{%i}} Assault",GetID()));
  SetScoreboardData(0,SBRD_Caption,"Timer");
  SetScoreboardData(0,1, Format("<c %x>%d : %.2d</c>",GetTeamColor(2),timer1/60,timer1%60));
  SetScoreboardData(1,1, Format("<c %x>%d : %.2d</c>",GetTeamColor(1),timer2/60,timer2%60));
}

public func Activate(int iPlr) {
  var count;
  for(var index = 0; index < GetLength(targetstatus); index++)
    if(targetstatus[index] && !targetopt[index])
      count++;

  return(MessageWindow(Format("$MsgGoalUnfulfilled$", count),iPlr));
}

private func UpdateScoreboard()
{
	// leer :(
}

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
  if(iPlr == -1 || !GetPlayerName(iPlr)) return();

	// Scoreboard-stuff brauchen wir nicht

  // Geld
  Money(iPlr, pClonk, iMurdererPlr);
}

/* Entwicklungshilfe */

public func Debug()
{
	var i,j = GetLength(targets);
	for(i = 0; i < j; i++)
	{
		if(!targets[i])
			Log("%2d:   Zerstört/Nicht vorhanden");
		if(targetopt[i])
			Log("%2d:   %s - Nebenziel", i+1, GetName(targets[i]));
		else
			Log("%2d:   %s", i+1, GetName(targets[i]));
	}
}
