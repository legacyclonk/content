/*-- EMP Granate --*/

#strict

// Ist eine Granate
#include GREN

func HitObject()
{
    CreateEMPWave();
    RemoveObject();
}

func CreateEMPWave()
{
    Sound("EMP");
    CreateObject(SHCK, 0, 0, GetOwner())->SetMaxSize(iDamage);
}

func ExplodeDelay() {   return(20); }
