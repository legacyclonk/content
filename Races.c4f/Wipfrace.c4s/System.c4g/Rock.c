/* Raucheffekte des Steines im Berg */
#strict
#appendto ROCK

func Hit()
{
  _inherited();
  if(Local(0)) {
        CastParticles("FSpark", 5,10, 0,0, 50, 75);
      RemoveObject();
    }
    return(1);
}    
