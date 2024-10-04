/*-- Pumpe --*/

#strict
#include PIPL

local left, right, up, down, valve, connects, eff,con,
      efflux, conflux, temp, temp2,
      running;

public func Initialize() {
  SetAction("Pump");
  efflux = CreateArray();
  conflux = CreateArray();
  // Standardleistung
  power = 2;
}

/* Wichtig für die Pumpen:

    Erster Anschluss -> Zufluß
    Zweiter Anschluss -> Abfluß
*/

public func Left(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  if(connects >= 2) return();

  return(_inherited(iRepeat, solid, valve, pipeId, iPower, connect));
}

public func Right(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  if(connects >= 2) return();

  return(_inherited(iRepeat, solid, valve, pipeId, iPower, connect));
}

public func Up(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  if(connects >= 2) return();

  return(_inherited(iRepeat, solid, valve, pipeId, iPower, connect));
}

public func Down(int iRepeat, bool solid, bool valve, id pipeId, int iPower, object connect) {
  if(connects >= 2) return();

  return(_inherited(iRepeat, solid, valve, pipeId, iPower, connect));
}

public func ConnectTo(object connect, int dir) {
  if(!connects) con = connect;
  if(connects == 1) eff = connect;
  connects++;

  return(_inherited(connect, dir));
}

/* Öffentliches */

public func UpdateEfflux() {
  efflux = CreateArray();

  if(eff)
  {
    if(eff == left)  eff->UpdateSystem(this(), PIPE_Right, 2);
    if(eff == right) eff->UpdateSystem(this(), PIPE_Left, 2);
    if(eff == down)  eff->UpdateSystem(this(), PIPE_Up, 2);
    if(eff == up)    eff->UpdateSystem(this(), PIPE_Down, 2);
  }

  UpdateConflux();
}

public func UpdateConflux() {
  conflux = CreateArray();

  if(con)
  {
    if(con == left)  con->UpdateSystem(this(), PIPE_Right, 1);
    if(con == right) con->UpdateSystem(this(), PIPE_Left, 1);
    if(con == down)  con->UpdateSystem(this(), PIPE_Up, 1);
    if(con == up)    con->UpdateSystem(this(), PIPE_Down, 1);
  }
}

public func UpdateSystem(object start, int From, bool pump) {
  // Andere Pumpe
  if(pump) start->~TwoPumpError();
  // Prinzipiell regelt das die Pumpe
  return();
}

public func NewConflux(object new) {
  conflux[GetLength(conflux)] = new;
}

public func NewEfflux(object new) {
  efflux[GetLength(efflux)] = new;
}

// Fehler!
public func TwoPumpError() {
  Message("$Error$", this());
}

/* Pumpen */

protected func Pumping() {
  if(!running) return();
  if(efflux[0] == 0) return();
  if(conflux[0] == 0) return();

  var mat;
  for(var i=0 ; i < power ; i++)
    {
    mat = ExtractLiquid(AbsX(GetX(conflux[temp2])), AbsY(GetY(conflux[temp2])));
    efflux[temp]->CastLiquid(mat, 35);
    }

  if(!Random(4)) Smoke(0,0, 15);
  if(!Random(4)) Bubble(AbsX(GetX(conflux[temp2])), AbsY(GetY(conflux[temp2])));

  temp++;
  temp2++;
  if(efflux[temp] == 0) temp = 0;
  if(conflux[temp2] == 0) temp2 = 0;
}

public func On() {
  running = true;
  Sound("Gear", 0, this(), 50,0, 1);
}

public func Off() {
  running = false;
  Sound("Gear", 0, this(), 0,0,-1);
}

public func Status() { return(running); }

/* Steuerkonsole */

public func ConsoleControl(int i) {
  if(i == 1)
    {
    if(running) return("$TurnOff$");
    else return("$TurnOn$");
    }
}

public func ConsoleControlled(int i) {
  if(i == 1)
    {
    if(!running) On();
    else Off();
    }
}

/* Steuerung mit Schalter */
func Access()
{
  if(running) Off();
  else On();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (running)
		extra[GetLength(extra)] = "On()";
	if (con)
		extra[GetLength(extra)] = ["LocalN(\"con\")=Object(%d)", con];
	if (eff)
		extra[GetLength(extra)] = ["LocalN(\"eff\")=Object(%d)", eff];
}
