/* Kaltes Feuer */

#strict

func Activate()
{
  Sound("Magic1");
  Sound("Inflame");
  Sound("Fuse");
  Var(0)=-60;
  if (GetDir(Par(0))) Var(0)=60;
  CastObjects(CFLM,3,20,Var(0),0);
  RemoveObject();
  return(1);
}
