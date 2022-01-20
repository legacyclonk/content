/* Use ladder functionality */

#strict


/* Dieses Script bietet die Grundfunktionalität des an Leitern kletterns. Damit
   der Clonk auch wirklich daran klettern kann, muss folgendes noch hinzugefügt
   werden. In der ActMap die Aktion hinzufügen:

[Action]
Name=ScaleLadder
Procedure=NONE
Directions=2
FlipDir=1
Length=16
Facet=0,20,16,20,0,0
NextAction=ScaleLadder

   In allen Jump-Aktionen wie "Jump", "JumpArmed" oder "Dive"
   
StartCall=JumpStart

   hinzufügen. Sowie in allen Control*-Funktionen eine Zeile an der geeigneten
   Stelle hinzufügen. Im Beispiel von ControlUp:
   
if(ControlLadder("ControlUp")) return(1);
   
   Und zwar für Up, Down, Right, Left, Dig, Throw, Command (für "MoveTo")
*/

local lastladder;

/* Zum Klettern an Leitern und Co. */

// Funktionen:
// GetLadderSpeed -		Gibt Leiterklettergeschwindigkeit zurück
// FindLadder -			Findet kletterbares in der Umgebung
// GrabLadder - 		Clonk greift angegebene Leiter
// ReleaseLadder - 		Clonk lässt Leiter los
// ReleaseLadderUp -	Clonk klettert nach oben und lässt los
// ReleaseLadderDown - 	Clonk klettert nach unten und lässt los
// ReleaseLadderStop - 	Clonk klettert nicht und lässt los
// LadderThrow - 		Werfen beim Klettern
// LadderDig -			Graben beim Klettern
// 
// ClimbLadder -		Mauskontrolle
// ControlLadder -		Steuern des Leiterkletterns
// AdjustLadderOffset - Lässt den Clonk an der Leiter kleben
//
// FxScalingLadderStart-Effekt fürs Klettern
// FxScalingLadderTimer-...
//
// JumpStart -			StartCall von Jump
// IsJumping -			Einfacher Callback ob der Clonk springt
// 
// FxJumpingTimer -		Effekt fürs Springen (checkt ob Leiter da)


// Um wieviel der Clonk an Leitern schneller klettert
// Wert/10 = Wand/Leiter; 10 = 1.0, 15 = 1.5,...
// Abwärtskompatibilität!
static const HZCK_LADDERSPEED = 12;
public func GetLadderSpeed() { return(HZCK_LADDERSPEED); }

//Ladder wird hier nur so gebraucht, alle Objekte, bei denen IsClimbable true zurückgibt können bestiegen werden!
public func FindLadder()
{
  var iOffset = 6;
  if (GetDir() == DIR_Left())
    iOffset = -6;
  var tmp;
  while(tmp = FindObject(0, iOffset, 0,0,0,0,0,0,0,tmp))
    if(Abs(GetX() - GetX(tmp)) < 6)
      if(tmp->~IsClimbable())
      return(tmp);
  return(0);
}

public func GrabLadder(object pLadder)
{
  if(lastladder == pLadder)
    return(0);
  // Kletteraktion
//  SetAction("ScaleLadder", pLadder);
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Stop);  

  //erstmal checken ob Backflip 4 great Specialeffects!
  if((GetAction() S= "BackFlip" && !Stuck())) {
    //Sound, weil Geschwindigkeit oder so
    Sound("Kime*");
  }

  SetXDir();
  SetYDir();

  //Position anpassen (in der Luft klettern ist doch weird ;))
  AdjustLadderOffset(pLadder);

  //nö, dann eben normal anhalten
  SetAction("ScaleLadder", pLadder);
   
  // Klettereffekt
  AddEffect("ScalingLadder", this(), 1, 1, this());
}

public func ReleaseLadder(int iXDir, int iYDir)
{
  var dir;
  
  var diffx, pLadder = GetActionTarget(), xpos = GetX(pLadder);
  // von was loslassen..?
  if(!pLadder)
   return(0);
   
  lastladder = pLadder;
    
  // Klettereffekt beenden
  RemoveEffect("ScalingLadder", this());

  //und runter von der Leiter
  if(dir < 0 || GetContact(0,1,CNAT_Bottom)) {
    SetAction("Walk");
  }
  if(dir > 0) {
    SetAction("Hangle");
  }
  else if(GetAction() ne "Tumble")  
    SetAction("Jump");
    
  SetXDir(iXDir);
  SetYDir(iYDir);
  return(1);
}

public func ReleaseLadderUp() {
	//Normal runterspringen
	ReleaseLadder(-10*(GetDir()*2-1));
}

public func ReleaseLadderDown() {
	//Normal runterspringen
	ReleaseLadder(-10*(GetDir()*2-1));
}

public func ReleaseLadderStop() {
	//Normal runterspringen
	ReleaseLadder(-10*(GetDir()*2-1));
}

public func LadderThrow() {
	// Ablegen
	SetComDir(COMD_Stop);
	SetCommand(0, "Drop");
}

public func LadderDig() {

}

protected func ClimbLadder()
{
  if(!GetCommand()) return();
  // Testen, ob wir noch klettern
  if(GetAction() ne "ScaleLadder") return();
  // Feststellen, ob nach oben oder unten
  var targetx = GetCommand(this(), 2);
  var targety = GetCommand(this(), 3);

  if(targety < GetY()) SetComDir(COMD_Up);
  if(targety > GetY()) SetComDir(COMD_Down);
 
  // Wegpunkt oben? früher abspringen.
  if(Inside(GetY()-targety,5,15) && Abs(GetX()-targetx) < 30)
  {
    var comd = GetComDir();
    if(targetx < GetX())
    {
    	ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }
    
  // Sind wir ungefähr da?
  if(Inside(GetY(), targety-5, targety+5) && PathFree(GetX(), GetY(), targetx, targety))
  {
    // Wir versuchen mal, abzuspringen (das klappt spätestens beim 2. Mal(theoretisch))
    
    //wenn Ziel weiter oben: immer nach oben abspringen
    if(GetY() > targety)
      SetComDir(COMD_Up);
    //Ziel weiter unten? Nach unten abspringen.
    else if(Abs(GetY()-targety) > 50)
      SetComDir(COMD_Down);
    //Ziel weiter weg? Backflip! :D
    else if(Abs(GetX()-targetx) > 80)
      SetComDir(COMD_Stop);
    
    var comd = GetComDir();
    if(targetx < GetX())
    {
    	ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }
}

private func ControlLadder(string strControl, par1) // par1 is comdir for ControlUpdate
{

  // Wenn man schnell genug ist, kann man sich so sogar wieder an der Leiter anhalten!
  if(lastladder)
	{
    lastladder = 0;
	}

  // Nicht an einem kletterbaren Objekt.
  if (GetAction() ne "ScaleLadder") return(0); 

  // Klettert an der Leiter
  if(strControl S= "ControlCommand") {
    ClimbLadder();
    return(1);
  }

  //Log("%s: %s",GetName(),strControl); //DEBUGZ!

  // JnR
  if (strControl S= "ControlUpdate")
  {
    var comdir = par1;
    // Strip away COMD_Left/COMD_Right, this is handled via ControlLeft/ControlRight
    if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);
    if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
    SetComDir(comdir);
  }
  
  // Rauf
  if (strControl S= "ControlUp" && !GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Up);
  // Runter
  if (strControl S= "ControlDown" && !GetPlrCoreJumpAndRunControl(GetController()))
    SetComDir(COMD_Down);
  // Rechts
  if (strControl S= "ControlRight")
  {
    // Richtung wechseln
    if (GetDir() == DIR_Right) {
      SetDir(DIR_Left);
      SetComDir(COMD_Stop);
      if (!AdjustLadderOffset(GetActionTarget())) //können wir überhaupt?
        SetDir(DIR_Right());
    }
    // Abspringen
    else {
      if(GetComDir() == COMD_Up) ReleaseLadderUp();
      else if(GetComDir() == COMD_Stop) ReleaseLadderStop();
      else ReleaseLadderDown();
    }
  }
  // Links
  if (strControl S= "ControlLeft")
  {
    // Richtung wechseln
    if (GetDir() == DIR_Left) {
      SetDir(DIR_Right);
      SetComDir(COMD_Stop);
      if (!AdjustLadderOffset(GetActionTarget()))
        SetDir(DIR_Left);
    }
    // Loslassen
    else {
      if(GetComDir() == COMD_Up) ReleaseLadderUp();
      else if(GetComDir() == COMD_Stop) ReleaseLadderStop();
      else ReleaseLadderDown();
    }
  }
  
  // Werfen
  if (strControl S= "ControlThrow") LadderThrow();
  
  //Graben
  if (strControl S= "ControlDig") LadderDig();
  
  return(1);  
}  

// Lässt den Clonk an der Leiter kleben
private func AdjustLadderOffset(object pLadder)
{
  //keine Leiter übergeben? An wtf soll dann adjustiert werden..?
  if(!pLadder)
    return(0);
  // Abstand nach Kletterrichtung
  var iLastX = GetX();
  var iChangeX;
  if(!(iChangeX = pLadder->~LocalN("SizeX",pLadder)/2))
    iChangeX = GetDefCoreVal("Width",0,GetID(pLadder))/4;
    
  if(GetDir() == DIR_Left())
    SetPosition(GetX(pLadder) + iChangeX, GetY());
  else
    SetPosition(GetX(pLadder) - iChangeX, GetY());  
  // Nicht in festes Material klettern
  if(Stuck()) 
  {
    SetPosition(iLastX, GetY());
    return(0);
  }   
  // Ausrichtung erfolgreich
  return(1);
}

protected func FxScalingLadderStart(object pTarget, int iEffectNumber, int iTemp) {
  if(iTemp)
    return();

  EffectVar(0, pTarget, iEffectNumber) = GetY()*100;
}

//Oke, das hier ist wohl großteils aus dem Urwaldclonk *hidez*
protected func FxScalingLadderTimer(object pTarget, int iEffectNumber, int time)
{
  var pLadder;
  // Clonk hat aus irgendeinem Grund seine Kletteraktion verloren
  if (GetAction() ne "ScaleLadder")
  {
    // Klettern beenden
    return(ReleaseLadder(0));
  }

  // Keine Leiter mehr: klettern beenden
  if (!(pLadder = FindLadder()))
  {
    // Oberes Ende: versuchen, die Wand zu erreichen
    if (GetComDir() == COMD_Up()) {
      return(ReleaseLadder(-15 + GetDir() * 30,-20));
    }
    // Unteres Ende: einfach fallen lassen
    else
      return(ReleaseLadder(0));
  }

  // Ggf. neues Objekt zum dran klettern speichern
  SetActionTargets(pLadder);
  // Abstand zum Objekt
  AdjustLadderOffset(pLadder);

  // Klettergeschwindigkeit 
  var iStep = Max(1,(GetPhysical("Scale")*4*GetLadderSpeed())/20000);
  
  // Kletterbewegung
  var iPosY = EffectVar(0, pTarget, iEffectNumber);
  var iLastY = GetY();
  var iPhase = GetPhase();
  if (GetComDir() == COMD_Up()) { iPosY -= iStep; iPhase += iStep*14/1000; }
  if (GetComDir() == COMD_Down()) { iPosY += iStep; iPhase -= iStep*14/1000; }
  
  // stecken wir fest?
  if (Stuck())
  {
    // Nicht in festes Material klettern
		if(GetComDir() == COMD_Up)
		{
			ReleaseLadder(0,-2);
			SetAction("Hangle");
		}
		if(GetComDir() == COMD_Down)
		{
			ReleaseLadder(0,2);
			SetAction("Walk");
		}
		else
		{
	  	SetPosition(GetX(), iLastY);
	  	SetComDir(COMD_Stop());
	  }
  	
  	return();
  }
  
  if (iPhase < 0) iPhase = 15; if (iPhase > 15) iPhase = 0;
  SetPhase(iPhase);
  SetPosition(GetX(), iPosY/100);
  SetXDir(0); SetYDir(0);

  EffectVar(0, pTarget, iEffectNumber) = iPosY;
  
  if(!(time%10)) ClimbLadder();
}

public func JumpStart(bool bBackflip)
{
  _inherited(bBackflip);

  // Spezielle Funktionen während des Sprungs (An Kletterzeugs festhalten)
  AddEffect("Jumping", this(), 1, 2, this());
}

protected func FxJumpingTimer()
{
  // Springt er?
  if (this->IsJumping() || WildcardMatch(GetAction(), "*Flip"))
  {
    // Schaun ob ein kletterbares Objekt da ist (Leiter z.b.)
    //var pVine; ...äh, falsch
    var pLadder; //Leiter natürlich!
    if (pLadder = FindLadder())
      GrabLadder(pLadder);
  }
  // ke, springt er halt nicht. *Effekt wegtun*
  else 
  {
		lastladder = 0;
    return (-1);
  } 
}


