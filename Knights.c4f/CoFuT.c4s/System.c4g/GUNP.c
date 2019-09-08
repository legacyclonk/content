/* Gunpowder */
// Prevent super-jumps from one castle to the other by doing extra damage to Clonks

#strict

global func Explode(int size, object obj)
  {
  if (GetID(obj) == GUNP)
    {
    for (var clonk in FindObjects(Find_OCF(OCF_Alive), Find_Distance(size, GetX(obj)-GetX(), GetY(obj)-GetY())))
      {
      clonk->DoEnergy(size/-2);
      }
    }
  return(_inherited(size, obj));
  }
