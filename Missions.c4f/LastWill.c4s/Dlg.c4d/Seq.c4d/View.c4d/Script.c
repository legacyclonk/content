/*-- Sequenzen: Sichtverfolgung --*/

#strict 2

local view_target; // Clonk, der die Sicht verfolgt
local sequence_obj;

/* Initialisierung */

public func InitView(object target, object seq)
  {
  // Daten speichern
  sequence_obj = seq;
  view_target = target;
  // Verfolgen!
  MakeCrewMember(this, GetOwner());
  // Sicht einstellen
  SetCursor(GetOwner(), this, true, true);
  SetViewCursor(GetOwner(), view_target);  
  SetAttach(view_target);
  SetName(" ");
  }
  
public func SetAttach(object target)
  {
  SetAction("Attach", target);
  UpdateView();
  }

public func ResetAttach()
  {
  SetAttach(view_target);
  }

protected func CrewSelection(bool deselect, bool is_cursor)
  {
  if (is_cursor)
    {
    if (deselect)
      {
      SetViewCursor(GetOwner(), 0);
      }
    else
      {
      SetViewCursor(GetOwner(), view_target);
      SetPlrView(GetOwner(), this);
      }
    }
  }

protected func AttachTargetLost()
  {
  SetAction("Free");
  SetXDir(); SetYDir();
  }

protected func Destruction()
  {
  // Sicht gelöscht: Sicht zurücksetzen auf den Ursprungsclonk
  if (GetCursor(GetOwner()) == this && view_target) SetCursor(GetOwner(), view_target);
  }

protected func UpdateView()
  {
  if (GetCursor(GetOwner()) == this)
    {
    var view_obj = GetActionTarget();
    if (!view_obj) view_obj = this;
    SetPlrView(GetOwner(), view_obj);
    }
  }


/* Sequenzmenü */
  
protected func ControlSpecial2(object view)
  { [$TxtSkip$|Image=SV69]
  if (sequence_obj) sequence_obj->OpenSkipMenu(GetOwner());
  }
