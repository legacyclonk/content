#strict

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }

// Wird von nun an mit dem Langbogen erforscht
public func GetResearchLink() { return(BOW2); }

// Damit es nicht billiger als normales Fleisch wird
public func CalcDefValue() { return(4); }
