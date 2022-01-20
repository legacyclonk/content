/* globale Log-Funktionen */

#strict

static LOG_debug;

// Logs an error message
global func ErrorLog(string msg) {
  var out = Format(msg,Par(1),Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9));
  var out = Format("ERROR: %s",out);
  Log(out);
  if(this()) 
    Log(" in: %s @(%d,%d)",GetName(),GetX(),GetY());
}

// Logs if debug mode is on
global func DebugLog(string msg, name) {
  if(Debug(name))
    Log(msg,Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9));
}

// Displays the message if the debug mode is on
// always per-object
global func DebugMessage(string msg,  name) {
  if(Debug(name))
    Message(msg,this(),Par(2),Par(3),Par(4),Par(5),Par(6),Par(7),Par(8),Par(9));
}

// Checks wether the debug mode is on
global func Debug(name) {
  if(!LOG_debug) return(false);

  // this namespace on
  if(LOG_debug->ContainsItem(name)) return(true);
  
  // generally on
  if(LOG_debug->ContainsItem("all")) return(true);
      
  return(false);
}

global func DebugMode(bool switch, name) {

  if(!name) name = "all";

  // create the set?
  if(!LOG_debug) LOG_debug = CreateObject(L_ST);
 
  // on
  if(switch) {
    if(!(LOG_debug->ContainsItem(name)))
      LOG_debug->AddItem(name);
  }
  // off
  else {
    if(name == "all") LOG_debug->Clear();
    else LOG_debug->RemoveItem(name);
  }
}

