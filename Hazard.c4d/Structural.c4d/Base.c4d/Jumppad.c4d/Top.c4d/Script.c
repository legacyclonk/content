/*-- Jumper --*/

#strict

local strength;

public func IsMachine() { return(true); }

private func Initialize() {
   
  return(1);
}

public func SetStr(int str)
{
  strength = str; 
}

private func CheckBounce() // Timer um Clonks wegzusto�en
{
  if(EMPShocked())
  	return();
  for(var pClonk in FindObjects(Find_InRect(-8, -8, 16, 16), Find_Or(Find_OCF(OCF_Alive()),Find_Func("IsBouncy")), Find_NoContainer()))
    Bounce(pClonk);
}

private func Bounce(object pClonk) // St��t unwissende Clonks usw. weg
{
  // mijon komplifikizierte Berechnung!
  var nXDir, nYDir, oXDir, oYDir;
  oXDir = pClonk->GetXDir();
  oYDir = pClonk->GetYDir();
  nXDir = Sin(GetR(), strength);
  nYDir = -Cos(GetR(), strength);
  // Der Clonk beh�lt ein wenig seiner alten Geschwindigkeit bei
  nXDir += oXDir/10;
  nYDir += oYDir/10;
  // Puff!
  pClonk->SetAction("Jump");
  pClonk->SetSpeed(nXDir, nYDir);
  Sound("Jumppad");
}

public func EMPShock()
{
	SetAction("Disabled");
	EMPShockEffect(38*3);
}

public func EMPShockEnd() {
	SetAction("Attach");
}

public func NoSerialize() { return(true); }