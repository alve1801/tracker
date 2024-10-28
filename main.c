#include<stdio.h>
#include<math.h>
#undef EOF
#define EOF 255 // were using unsigned chars
const int rate=8000,notelen=rate/8;
char DEBUG=0,prog[256];

int t=1;

typedef struct track{
	char cs[8]; // callstack. first element is index into callstack itself
	int time,len,nlen; // countdown, current note length, and default note length
	float freq; // current frequency
	// char a,d,s,r; // envelope
}track;

track tracks[8];

void inittrack(track*a){
	// dont think we can automate this...
	for(int i=0;i<4;i++)a->cs[i]=0;
	a->time=a->len=0;
	a->nlen=notelen;
}

void main(int argc,char**argv){
	if(argc<2)return;
	if(argc>2)DEBUG=1;
	FILE*f=fopen(argv[1],"r");
	{char a,b=0;while((a=getc(f))!=EOF && b<256)prog[b++]=a;}
	fclose(f);

	for(int i=0;i<8;i++)inittrack(tracks+i);

	// XXX initialise first track
	tracks[0].cs[0]++;

	while(t++){ // main loop
		int ret=0,sum=0;
		for(int ta=0;ta<8;ta++)if(tracks[ta].cs[0]){
			track*a=tracks+ta; // be a lot cooler if i could do this by reference, yet alas
			while(!a->time && a->cs[0]){
				if(DEBUG)printf("track %i (%i,%i %2x): ",ta,a->cs[0],a->cs[a->cs[0]],prog[a->cs[a->cs[0]]]);

				char op=prog[a->cs[a->cs[0]]++],arg;

				if(!op){ // return or kill
					if(DEBUG)printf("ret\n");
					a->cs[0]--;
				}else if((op&0x80)==0){ // only one w/ else - all others have bits set
					arg=op&0x7f;
					if(DEBUG)printf("playing note %i\n",arg);
					if(!arg)a->freq=0;else
					a->freq = 2*M_PI*440*powf(2,(float)(arg-49)/12)/rate;
					a->time=a->len=a->nlen;
				}

				if((op&0xf0)==0x80){
					arg=op&0x0f;
					if(DEBUG)printf("pause for %i (%i) beats\n",arg,notelen<<arg);
					a->freq=0;a->time=a->len=notelen<<arg;
				}

				if((op&0xf0)==0x90){
					arg=prog[a->cs[a->cs[0]]++];
					if(DEBUG)printf("playing note %i for length %i (%i)\n",
						arg,op&0xf,notelen<<(op&0xf));
					if(!arg)a->freq=0;else
					a->freq = 2*M_PI*440*powf(2,(float)(arg-49)/12)/rate;
					a->time=a->len=notelen<<(op&0xf);
				}

				if((op&0xf0)==0xa0){
					arg=op&0x0f;
					if(DEBUG)printf("note length is now %i (%i)\n",arg,notelen<<arg);
					a->nlen=notelen<<arg;
				}

				if((op&0xf8)==0xb0){
					arg=prog[a->cs[a->cs[0]]++];
					if(DEBUG)printf("setting adsr parameter %x to %i\n",op&0xf,arg);
					// we dont yet have the parameters, but im leaving it here for later
					// first four are adsr, rest deal w/ synthesis?
				}

				if(op==0xb8){
					arg=prog[a->cs[a->cs[0]]++];
					if(DEBUG)printf("jmp to %i\n",arg);
					a->cs[a->cs[0]]=arg;
				}

				if(op==0xb9){
					arg=prog[a->cs[a->cs[0]++]++];
					if(DEBUG)printf("call %i\n",arg);
					a->cs[a->cs[0]]=arg;
				}

				if(op==0xba){
					arg=prog[a->cs[a->cs[0]]++];
					if(DEBUG)printf("frk to %i\n",arg);
					for(int i=0;i<8;i++)if(!tracks[i].cs[0]){
						tracks[i].cs[0]=1;
						tracks[i].cs[1]=arg;
						break;
					}
				}

			}

			if(a->cs[0]){ // if were still running
				a->time--;sum++;
				if(!a->freq)ret+=127;else // XXX expand this for adsr etc
				ret+=sinf((float)t*a->freq)*127+127;
			}
		}

		if(sum)ret/=sum;else ret=0;

		if(DEBUG){int i;
			for(i=0;i<ret;i+=4)putchar('#');
			for(;i<0xff;i+=4)putchar('.');
			putchar(10);
		}else putchar(ret);
	}
}
