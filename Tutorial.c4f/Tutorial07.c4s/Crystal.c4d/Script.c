/*--- Kristall ---*/

#strict

protected func Initialize()
{
	CreateObject(GLOW)->Attach(this);	
}

protected func Hit()
{
  Sound("Crystal*");
  return(1);
}
