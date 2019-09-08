#strict

protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}

protected func Timer() {
  var plr_num;
  for (var plr_idx = 0; plr_idx <= GetPlayerCount(); plr_num = GetPlayerByIndex(plr_idx++)) {
    HandlePlayer(plr_num);
  }
}

private func GetKnowledgeCategory(index) {
  if (!index--) return(C4D_Structure());
  if (!index--) return(C4D_Vehicle());
  if (!index--) return(C4D_Object());
}

private func HandlePlayer(plr_num) {
  var cat, cat_num = 0;
  while (cat = GetKnowledgeCategory(cat_num++)) {
    var knw_id, knw_num = 0;
    while (knw_id = GetPlrKnowledge(plr_num, 0, knw_num++, cat)) {
      TransferToAllies(plr_num, knw_id);
    }
  }
}

private func TransferToAllies(plr_num, knw_id) {
  var ally_num;
  for (var ally_idx = 0; ally_idx <= GetPlayerCount(); ally_num = GetPlayerByIndex(ally_idx++)) {
    // Ist nicht der Spieler selbst
    if (ally_num != plr_num) {
      // Ist tatsächlich verbündet
      if (!Hostile(ally_num, plr_num)) {
	// Hat den Bauplan noch nicht
	if (!GetPlrKnowledge(ally_num, knw_id)) {
	  SetPlrKnowledge(ally_num, knw_id);
	}
      }
    }
  }
}
