/*-- Zielscheibendrone --*/

#strict

local drop, move;
local xpos, ypos, x,y,xdir,ydir, xrange, yrange;

func Hit() {
  var target;
  if(target = FindObject(TARG,0,0,0,0,0,0,this()))
    target->Damage();

  Explode(20);
}

func Damage()
{
	Hit();
}

func IsMachine() { return(1); }

func EMPShock() {
  SetAction("Falling");
  move = 0;
}

func Initialize() {
  SetAction("Fly");
  FadeIn(0,0,50);
  var targ = CreateObject(TARG, 0,0, 1);
  targ->SetAction("Attach",this());
}

func JobDone() {
  if(drop) {
    CreateObject(drop);
    Explode(20);
    Sound("Blast3");
  }
	FadeOut(0,0,50);
}

func StartMoving(Dirx, Diry, Rangex, Rangey, xstart, ystart) {
  xdir = Dirx;
  ydir = Diry;
  xrange = Rangex;
  yrange = Rangey;
  x = xstart;
  y = ystart;
  xpos = GetX();
  ypos = GetY();
  move = 1;
}

func Move() {
  if(!move)
    return();
    
  x += xdir;
  y += ydir;
  if(Abs(x) >= xrange)
    xdir *= -1;
  if(Abs(y) >= yrange)
    ydir *= -1;
  
  SetPosition(xpos+x, ypos+y);
}
