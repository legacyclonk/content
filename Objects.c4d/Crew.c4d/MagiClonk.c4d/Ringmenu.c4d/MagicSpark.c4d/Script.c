/* Magischer Funken */

#strict

protected func Initialize()
{
	SetVisibility(VIS_Owner());
	SetAction("Exist");	
}



protected func Destroy()
{
	RemoveObject();
}
