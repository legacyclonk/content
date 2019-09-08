/*-- LossOfBase --*/

#strict

private func Execute() {
  // Alle Spieler überprüfen
  var plr_num;
  for (var plr_cnt = GetPlayerCount(); plr_cnt; plr_num++) {
    if (GetPlayerName(plr_num)) {
      plr_cnt--;
      if (GetPlayerTeam(plr_num) != -1) if (!FindBase(plr_num)) 
        EliminatePlayer(plr_num);
    }
  }
}
      
protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}
