#strict 2 

global func GetTaggedTeamName(int plr) {
  var teamName = GetTeamName(GetPlayerTeam(plr));
  if(!teamName) return "";
  return(Format("<c %x><%s></c>", GetTeamColor(GetPlayerTeam(plr)),teamName));
}