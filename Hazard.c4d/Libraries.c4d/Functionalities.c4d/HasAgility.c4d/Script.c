/* Has agility functionality */

#strict

/* Dieses Script bietet die Grundfunktionalität der Beweglichkeit des
    Hazardclonks. Um diese Funktionalität zu bedienen, im inkludierenden
    Script in folgenden Control*-Funktionen eine Zeile an der geeigneten
    Stelle hinzufügen. Und zwar bei

	-Up, -Left, -Right, -LeftDouble, -RightDouble, -UpDouble.
	
    Im Beispiel von ControlLeftSingle:
	
	if(ControlAgility("ControlLeftSingle")) return(1);

    Außerdem muss im StartCall von Jump-Aktionen die Funktion JumpStart();
    aufgerufen werden bzw. wenn diese schon im Script vorhanden ist am Ende
   _inherited aufgerufen werden.
   
   Agility kann mit der Leiterklettern-Funktionalität zusammenarbeiten.
*/
	
/* Control from the clonk */

private func ControlAgility(string strControl) {
	// return 0 if the rest of the clonks script should be executed

	// left
	if(strControl eq "ControlLeft") {
		if(this()->IsJumping()) {
			SetDir(DIR_Left);
			SetComDir(COMD_Left); 
		}
		
		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		EffectVar(0, this, iEff) = COMD_Left;
		return 0;
	}
  
	// right
	if(strControl eq "ControlRight") {
		if(this()->IsJumping()) {
			SetDir(DIR_Right);
			SetComDir(COMD_Right); 
		}
		
		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		EffectVar(0, this, iEff) = COMD_Right;
		return 0;
	}
  
	// left double
	if(strControl eq "ControlLeftDouble") {
		if(MayFlip())
		{
			if(GetDir() == DIR_Left)
				ForwardFlip(-30);
			else
				BackFlip(-30);

			return 1;
		}
	}

	// right double
	if(strControl eq "ControlRightDouble") {
		if(MayFlip())
		{
			if(GetDir() == DIR_Right)
				ForwardFlip(-30);
			else
				BackFlip(-30);

			return 1;
		}
	}
	
	// up
	if(strControl eq "ControlUp") {
		if(WildcardMatch(GetAction(), "Scale*") && GetActTime() < 5)
			EffectVar(2, this, AddEffect("ControlStack", this, 110, 5, this)) = true;

		if(GetEffect("ControlStack", this))
			EffectVar(1, this, GetEffect("ControlStack", this)) = COMD_Up;

		return 0;
	}
	
	if(strControl eq "ControlUpDouble") {
		if(this()->IsJumping() && !GetEffect("ExtraJump", this))
			if(GetYDir() <= 5)
				return ExtraJump();
			
		return 0;
	}
	
}

private func MayFlip() {
	if(this()->IsJumping())
	  if(!GetEffect("ExtraJump", this))
	    return(true);
}

/* JumpStart */

// Jump-StartCall: Spezialsprung?
// overloaded by Clonk
public func JumpStart(bool bBackflip)
{
	// Clonk auf festem Boden?
	if (!GetEffect("ExtraJump", this)) {
		var iEff, iComd, iStr = 0, iYDir = 0;
		if(iEff = GetEffect("ControlStack", this))
		{
			if(EffectVar(1, this, iEff) != COMD_Up && !bBackflip) return;
			if(EffectVar(2, this, iEff)) { iYDir = -60; iStr = -40; }
			iComd = EffectVar(0, this, iEff);

			RemoveEffect(0, this, iEff);
			if(iComd == COMD_Left)
			{
				if(GetDir() == DIR_Left)
					ForwardFlip(iStr, 0, iYDir);
				else
					BackFlip(iStr, 0, iYDir);
			}
			if(iComd == COMD_Right)
			{
				if(GetDir() == DIR_Left)
					BackFlip(iStr, 0, iYDir);
				else
					ForwardFlip(iStr, 0, iYDir);
			}
		}
	}
	// für ladder
	_inherited(bBackflip);
}

// Leiter-Funktionalität benutzend

// NOTE: this overloads ladder functionality. So this one needs to be included
// after the ladder functionality
public func ReleaseLadderStop() {
	//Backflip! :D
	if(this()->ReleaseLadder(-20*(GetDir()*2-1))) {
	  SetAction("BackFlip");
	  ScheduleCall(0, "BackFlipBoost", 1, 1);
	  AddEffect("ExtraJump", this, 111, 5, this);
	}
}
	
/* Jump functions */

protected func ExtraJump()
{
	var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
	SetYDir(-40 * jump / 100);
	SetXDir(GetXDir() / 2);
	AddEffect("ExtraJump", this, 111, 5, this);
}

protected func FxExtraJumpTimer()
{
	if(!WildcardMatch(GetAction(), "Jump*")
	&& !WildcardMatch(GetAction(), "*Flip")
	&& !WildcardMatch(GetAction(), "*Flight")
	&& GetAction() ne "Dive")
			return -1;
}

protected func BackFlip(int iStr, bool fNoJumpCalc, int iYDir)
{
	SetAction("BackFlip");
	ScheduleCall(0, "FlipBoost", 1, 1, !GetDir(), iStr, fNoJumpCalc, iYDir);
        AddEffect("ExtraJump", this, 111, 5, this);
}

protected func ForwardFlip(int iStr, bool fNoJumpCalc, int iYDir)
{
	if(!iStr && !iYDir) { iStr = -20; iYDir = -45; }
	SetAction("ForwardFlip");
	ScheduleCall(0, "FlipBoost", 1, 1, GetDir(), iStr, fNoJumpCalc, iYDir);
        AddEffect("ExtraJump", this, 111, 5, this);
}

protected func FlipBoost(int iDir, int iStr, bool fNoJumpCalc, int iYDir)
{
	if(!iYDir) iYDir = -24;
	iDir = iDir *2 -1;
	var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
	jump = BoundBy(jump + iStr, 0, 300);
	if(fNoJumpCalc) jump = iStr;
	SetXDir(40 * iDir * jump / 100);
	SetYDir(iYDir * jump / 100);
}

//  Alte Funktion wird noch von ReleaseLadderStop benutzt
//   |
//  v

protected func BackFlipBoost()
{
  // Wenn die Sprungkraft gerade verändert ist, gilt das auch für den Backflip
  var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
  SetXDir(40*((GetXDir()>0)*2-1) * jump / 100); // SetXDir(GetXDir()/*<-20*/* 3/2);
  SetYDir(-24 * jump / 100);
}

// ^
// |

public func FxControlStackEffect(string newEffect, object pTarget, int iNo)
{
	if(newEffect ne "ControlStack") return;
	ChangeEffect(0, pTarget, iNo, 0, -1);
	return -2;
}
