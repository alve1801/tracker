#include<stdio.h>

char a,pp,prog[256],labels[256];

char hex(char a){
	if('0'<=a && a<='9')return a-'0';
	if('a'<=a && a<='f')return a-'a'+10;
	return a;
}

void main(){
	pp=0;
	while((a=getchar())!=255){
		switch(a){
			// play note
			case'n':prog[pp++]=getchar();break;

			case'p': // pause
				prog[pp++]=0x80|hex(getchar());break;

			case's': // play note w/ nonstandard length
				prog[pp++]=0x90|hex(getchar());
				prog[pp++]=getchar();
				break;

			case'l': // change standard note length
				prog[pp++]=0xa0|hex(getchar());break;

			case'r': // change register values - unused for now
				a=getchar();
				switch(a){
					case'a':prog[pp++]=0xb0;break;
					case'd':prog[pp++]=0xb1;break;
					case's':prog[pp++]=0xb2;break;
					case'r':prog[pp++]=0xb3;break;
					// XXX etc
				}
				a=hex(getchar());
				a=(a<<4)|hex(getchar());
				prog[pp++]=a;
				break;

			case'.': // set label
				labels[getchar()]=pp;
				break;

			case'j': // jmp
				prog[pp++]=0xb8;
				prog[pp++]=getchar();
				break;

			case'c': // call
				prog[pp++]=0xb9;
				prog[pp++]=getchar();
				break;

			case'f': // fork
				prog[pp++]=0xba;
				prog[pp++]=getchar();
				break;

			case'z':prog[pp++]=0;break; // end of track
			case' ':case'\n':break; // ignores whitespace
			default:prog[pp++]=a&0x7f; // if unknown command, assume its a note literal
		}
	}

	for(int i=0;i<pp;i++){
		putchar(prog[i]);
		if(prog[i]==0xb8 || prog[i]==0xb9 || prog[i]==0xba){
			i++;
			putchar(labels[prog[i]]);
		}
	}

}

/*  ' '!"#$%&'
 ()*+, -./0123
 45678 9:;<=>?
 @ABCD EFGHIJK
||_|_|||_|_|_|||_|_||
|_|_|_|_|_|_|_|_|_|_|
52      59    62      71
 LMOPQ RSTUVWX*/
