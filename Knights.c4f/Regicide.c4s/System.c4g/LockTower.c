/*-- Abschließbarer Turm --*/

#strict
#appendto CPT2
#appendto CPT3
#appendto CPT4
#appendto CPTR
#appendto CPTL

 protected func ControlThrowDouble(){  
   if(FindObject(CPTE,-5,-46,10,10)){ 
     Sound("Error");  
     Message("Treppenhaus bereits offen",this()); 
   }else{ 
     CreateObject(CPTE, 0, -31, GetOwner()); 
     Message("Treppenhaus geöffnet",this()); 
   }
   return(1);
 }
 
 protected func ControlDigDouble(){ 
   if(Var()=FindObject(CPTE,-5,-46,10,10)){
     RemoveObject(Var(),1);
     Message("Treppenhaus geschlossen",this()); 
   }
   return(1); 
 }