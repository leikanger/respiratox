#include <string>

#ifndef TERMINALOUTPUTCOLORS_H
#define TERMINALOUTPUTCOLORS_H

namespace textColor {

using std::string;

// Text reset
const string RColor = "\e[0m";    // Text Reset

// Regular          			 Bold                				Underline
const string Black 	="\e[0;30m"; const string BBlack="\e[1;30m"; 	const string UBlack="\e[4;30m";   
const string Red 	="\e[0;31m"; const string BRed 	="\e[1;31m"; 	const string URed="\e[4;31m";
const string Green 	="\e[0;32m"; const string BGreen="\e[1;32m"; 	const string UGreen="\e[4;32m";   
const string Yellow ="\e[0;33m"; const string BYellow="\e[1;33m"; 	const string UYellow="\e[4;33m";
const string Blue 	="\e[0;34m"; const string BBlue ="\e[1;34m";	const string UBlue="\e[4;34m";
const string Purple ="\e[0;35m"; const string BPurple="\e[1;35m"; 	const string UPurple="\e[4;35m";
const string Cyan 	="\e[0;36m"; const string BCyan ="\e[1;36m"; 	const string UCyan="\e[4;36m";
const string White 	="\e[0;37m"; const string BWhite="\e[1;37m"; 	const string UWhite="\e[4;37m";

//High Intensity      			 BoldHigh Intens     				Background          			 High Intensity Backgrounds
const string IBla="\e[0;90m";    const string BIBla="\e[1;90m";   	const string On_Bla="\e[40m";    const string On_IBla="\e[0;100m";
const string IRed="\e[0;91m";    const string BIRed="\e[1;91m";   	const string On_Red="\e[41m";    const string On_IRed="\e[0;101m";
const string IGre="\e[0;92m";    const string BIGre="\e[1;92m";   	const string On_Gre="\e[42m";    const string On_IGre="\e[0;102m";
const string IYel="\e[0;93m";    const string BIYel="\e[1;93m";   	const string On_Yel="\e[43m";    const string On_IYel="\e[0;103m";
const string IBlu="\e[0;94m";    const string BIBlu="\e[1;94m";   	const string On_Blu="\e[44m";    const string On_IBlu="\e[0;104m";
const string IPur="\e[0;95m";    const string BIPur="\e[1;95m";   	const string On_Pur="\e[45m";    const string On_IPur="\e[0;105m";
const string ICya="\e[0;96m";    const string BICya="\e[1;96m";   	const string On_Cya="\e[46m";    const string On_ICya="\e[0;106m";
const string IWhi="\e[0;97m";    const string BIWhi="\e[1;97m";   	const string On_Whi="\e[47m";    const string On_IWhi="\e[0;107m";
}

#endif
