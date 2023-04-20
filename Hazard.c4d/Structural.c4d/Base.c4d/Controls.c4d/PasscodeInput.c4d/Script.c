/*-- PIN-Terminal --*/

#strict

local target, input, number, clonk;

/* Eingabe */


protected func ControlThrow(object pClonk)	{
	clonk = pClonk;
	Menu(0);
}


protected func Menu(int iSel) {
    var string_in;
    if(!input) string_in = "$InputCode$";
	else string_in = Format("$Input$: %d",input);

	// Menü erstellen
	CreateMenu (GetID(), clonk, this(), 0, 0, 0, 0, 1);
	// weil das Clonkmenü 4 breit ist, machen wir halt Hexadezimal...
	for(var i=1; i<10; ++i) {
		AddMenuItem(string_in,Format("Input(%d)",i),PCSO,clonk,0,0,0,2,i);
	}
	AddMenuItem(string_in,"StartOver()",PCSO,clonk,0,0,0,2,10);
	AddMenuItem(string_in,"Input(0)",PCSO,clonk,0,0,0,2,0);
	AddMenuItem("$Cancel$","Cancel()",PCSO,clonk,0,0,0,2,11);

	SetMenuSize(3,4,clonk);
	Schedule(Format("SetMenuSize(3,4,Object(%d));", ObjectNumber(clonk)), 2, 0, this());
	SelectMenuItem(iSel,clonk);
}

protected func StartOver() {
  var menusel = GetMenuSelection(clonk);
  Cancel();
  Menu(menusel);
}

protected func Cancel() {
  Sound("Bip");
  CloseMenu(clonk);
  input = 0;
}

protected func Input(int i) {
  var menusel = GetMenuSelection(clonk);
  input = input*10+i;
  CloseMenu(clonk);

  
  // freundlich beepen
  Sound("Bip");

  // fertig
  if(Length(input) == Length(number)) {
    if(!CheckInput()) Menu(menusel);
    input = 0;
  }
  else {
    Menu(menusel);
  }

}

private func Length(int a) {
  var i = 1;
  while(a>9) {
    a /= 10;
    ++i;
  }
  return(i);
}

private func CheckInput() {
  // success!
  if(input == number) {
    if(target) {
      target->~Access();

      // Effekt
      Sound("BipBipBip");
      CreateParticle("NoGravSpark", 5, 1, 0, 0, 50, RGBa(0, 255, 0, 50));
      return(true);
    }
  }
  // wrong code!
  else {
      Sound("Error");
      CreateParticle("NoGravSpark", 5, 1, 0, 0, 50, RGBa(255, 0, 0, 50));
      return(false);
  }
}

public func Set(object pTarget, int i)
{
  number = i;
  target = pTarget;
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (target)
		extra[GetLength(extra)] = [Format("Set(Object(%%d),%i)", number), target];
}
