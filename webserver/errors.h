////////////////////////////////////////////////
// File:        errors.h                      //
// Author:      Sam Henry                     //
// Date:        March 17, 2017                //
// Class:       CpS 320, Unix Programming     //
// Desc:        Fun error messages for the    //
//				webserver for CpS 320 :) 	  //
////////////////////////////////////////////////

#include "utils.h"

char *ERR_NOT_FOUND = 
	"HTTP/1.0 404 Not Found\n"
	COLOR_GREEN "From: Sam Henry\n"
	COLOR_RED "The file you are looking for does not "
	"exist on this server. Please verify that "
	"you have your information correct. Just "
	"a tip, it might be a slightly better idea "
	"to hack Google... Or maybe even the Pentagon.\n"
	COLOR_CYAN "Here's a good joke:\n"
	"\"Hi, I'd like to hear a TCP joke.\"\n"
	"\"Hello, would you like to hear a TCP joke?\"\n"
	"\"Yes, I'd like to hear a TCP joke.\"\n"
	"\"Okay, I'll tell you a TCP joke.\"\n"
	"\"Okay, I will hear a TCP joke.\"\n"
	"\"Are you ready to hear a TCP joke?\"\n"
	"\"Yes, I am ready to hear a TCP joke.\"\n"
	"\"Okay, I am about to send a TCP joke. It will last"
	"10 seconds, it has two characters, it does not have a setting,"
	"it ends with a punchline.\"\n"
	"\"Okay, I am ready to get your TCP joke that will last "
	"10 seconds, has two characters, does not have a setting, and "
	"ends with a punchline.\"\n"
	"\"I'm sorry, your connection has timed out.\"\n"
	"\"Hello, would you like to hear a TCP joke?\"\n" COLOR_RESET;

char *ERR_FORBIDDEN = 
	"HTTP/1.0 403 Forbidden\n"
	COLOR_GREEN "From: Sam Henry\n"
	COLOR_RED "What do you think you are DOING? Don't you "
	"dare try to access that file. It is not for "
	"your use. I am not to be played with. Be "
	"careful, or you may regret it someday. Mwahaha.\n"
	COLOR_YELLOW "Just kidding, I'm not nearly that mean. But honestly, "
	"you don't have permission to access that file. So "
	"please, don't try to kill me.\n\n"
	COLOR_CYAN "Oh, wanna hear a joke? Here's a good one I heard:\n"
	"A mite is sitting on the back of a fly.\n"
	"Fly: \"Hey, bug on my back, are you a mite?\"\n"
	"Mite: \"I mite be.\"\n"
	"Fly: \"Stupidest pun I ever heard.\"\n"
	"Mite: \"What do you expect? I just made it up on the fly.\"\n\n"
	"That was a good one, right??? xD Puns are the best kind of jokes.\n"
	"Anyway, until next time... hope you have a great day! :)\n" COLOR_RESET;

char *ERR_OVERFLOW =
	"HTTP/1.0 400 Bad Request\n"
	COLOR_GREEN "From: Sam Henry\n"
	COLOR_RED "Ever heard of a buffer overflow attack? Yeah? "
	"Well, that's exactly what you just tried to do to me. So "
	"here's my response: DENIED. How could you be so evil and "
	"try to overflow the webserver? :'(\n"
	COLOR_YELLOW "Jk Jk, no hard feelings at all! I would never be mad at you :) "
	"Please though, can you be kind? Buffer overflows are not nice. "
	"The computer does not like them... \n\n"
	COLOR_CYAN "On another note... Wanna hear a joke??? =D\n"
	"Q: What do you get when you drop an atomic bomb on a palace????\n"
	"A: A NEW KING!!! (nuking... get it?) \n"
	"Hehehehehehehehehehehehehehehehehehehehehehehehehehehehehehehehe"
	"hehehehehehehehehehehehehehehehehehehehehehehehehehehehehehehe"
	"hehehehehehehehehehehehehehehehehehehehehehehehehehehehehehehe"
	"hehehehehehehehehehehehehehehehehehehehehehehehehehehehehehehe"
	"hehehehehehehehehehehehehehehehehehehehehehehehehehehehehehehe\n"
	"Anyway, have a nice day! :) :)\n" COLOR_RESET;

char *ERR_FORMAT = 
	"HTTP/1.0 400 Bad Request\n"
	COLOR_GREEN "From: Sam Henry\n"
	COLOR_RED "Sorry, but your request header is not in the correct "
	"format. If want some documentation on how to format a request "
	"header correctly, please visit "
	COLOR_BLUE "https://protect.bju.edu/cps/docs/rfcs/rfc1945.html"
	COLOR_RED ". Sorry I couldn't just \"know what you want!\" I'm "
	"not quite that smart... yet... ;)\n\n"
	COLOR_CYAN "Oh hey... a joke for you! =D\n"
	"One day, a brunette was jumping up and down on some train tracks "
	"shouting \"23!! 23!! 23!!\" A blonde was near and saw this brunette. "
	"We know that blondes want to be like brunettes, so the blonde jumped "
	"up on tracks and started shouting \"23!! 23!! 23!!\" Then, out of "
	"nowhere, a train started coming. The brunette jumped off, but the "
	"train hit the blonde. After a few minutes, the brunette jumped back "
	"onto the empty tracks and started shouting \"24!! 24!! 24!!\"\n"
	"Sorry, had to... ;) Anyway, I'll send you an error message later!\n" COLOR_RESET;

char *ERR_NOT_IMPLEMENTED = 
	"HTTP/1.0 501 Not Implemented\n"
	COLOR_GREEN "From: Sam Henry\n"
	COLOR_RED "So sorry... the server has not implemented that type of HTTP request "
	"yet... Please please please don't be mad at me! I'm only trying to be a good "
	"server, doing my best to please those who use me! No judging is allowed...\n"
	COLOR_CYAN "Here's a little joke for you:\n"
	"Q: What do you get when you cross a religious man and a bag of Doritos?\n"
	"A: A chip-munk!!!! Hehehehehehehehehehehehehehehehehehehehehehehehe.....\n"
	"Anyway, until next time... :)\n" COLOR_RESET;