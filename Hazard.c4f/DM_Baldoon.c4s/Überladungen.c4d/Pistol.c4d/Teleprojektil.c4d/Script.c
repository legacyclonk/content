/*-- Teleprojektil --*/

#strict

local iRemoveTimer;
local user;

func Initialize()
{
user=GetCrew(GetController());
iRemoveTimer=175;
SetPlrViewRange(50);
}

func Hit()
{
    Sound("Bip");
}

func ControlDigDouble()
{
    //var user=
    var xpos=GetX();
    var ypos=GetY();
    
    if(GBackSolid(-2,-20) || GBackSolid(2,-20) || GBackSolid(0,-10)) {Message("$BadPlace$",user); SetCursor(GetController(),user); return(RemoveObject());}
    
    DrawParticleLine("XSpark",0,0,GetX(user)-xpos,GetY(user)-ypos,1,50,RGBa(255,0,0,128),RGBa(0,255,0,128));
    SetPosition(xpos,ypos-12,user);
    SetCursor(GetController(),user);
    RemoveObject();
    Sound("Electric");
    CastObjects(SPRK,20,30,0,-20);
    Sound("Fire1");
    
}

func Launch(int xdir, int ydir, int iDmg)
{
  SetSpeed(xdir, ydir);
}

func CheckCursor() {
    iRemoveTimer--;
    if(!iRemoveTimer) {SetCursor(GetController(),user); RemoveObject();}
    //if(!GetCursor(GetOwner(this()))==this()) RemoveObject();
}

func IsBouncy() { return(true); }
