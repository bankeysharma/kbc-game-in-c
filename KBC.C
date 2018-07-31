#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include<bios.h>
#include<time.h>
#include<graphics.h>

#define GRAY DARKGRAY
#define UBHELP 15

void moneytree(int);
void loadqbank(void);
void lifelines(void);
void drawqbase(void);
void drawbox(float,float,float,float,int);
void fillbox(int,int);
void prnstr(char *, float, float, int);
void drawbase(int);
void apply5050(void);
void drawlogo(int);
void showmess(char *,int,int);
void showcredit(void);

int confirm(void);
int showquestion(int,int);
int check(int,char);
int applylifeline(void);
int getlifelines(void);
int getqid(int);
int getmenu(void);

char getans(int);
char convert(long int);

int vMaxX=80, vMaxY=25, gQIndex=-1;
char *qbank[75][5];
char qans[75][2];
int arylifeline[]={1,1,1};

char *menu[]={"Start","Resume","Question Bank","Help","Options","Exit"};
char *money[]={"1000","2000","3000","5000","10,000","20,000","40,000",
						"80,000","1,60,000","3,20,000","6,40,000","12,50,000",
						"25,00,000","50,00,000","1,00,00,000"};
char *help[]={"***",
				  "FAMILIARIZING",
				  "Press the corresponding character to select option.",
				  "Press key {ENTER} to confirm your option.",
				  "Press key {F1} to activate the lifeline bar.",
				  "Press key {Esc} to quit the current session.",
				  "Choose RESUME to continue from your last quit point.",
				  "Choose START to begin a new game.",
				  "Choose EXIT to quit the game.",
				  "***",
				  "LIFE LINES",
				  "50 : 50, to eliminate two wrong options.",
				  "ASK 2 UR FRIEND, to take help from your friend.",
				  "CHANGE QUESTION, to change current question with another one.",
				  "***",
				  "Press any key ..."
				  };

void main()
{
	int i,gdriver,gmode,menuid=0,lifelineno=0, tag=0, tmp, oldlifeline=0, resume=-1;
	char res;
	char endmsg[30];
	clrscr();
	detectgraph(&gdriver,&gmode);
	initgraph(&gdriver,&gmode," ");

// **************************************
// Loading questions
// **************************************
	loadqbank();
//***************************************

	while((menuid=getmenu())!=6) {
		if(menuid==1 || menuid==2)	{
			if(menuid==1) {
				lifelineno=0; tag=0; oldlifeline=0; i=0;
				arylifeline[0]=1;
				arylifeline[1]=1;
				arylifeline[2]=1;
				resume=0;
			} else {
				tag=-1;
				if(resume != -1) resume=1;
			}

			cleardevice();
			drawlogo(BLUE);

			for(;(i<15 && resume!=-1);i++) {
				if (lifelineno>0 && resume!=1) arylifeline[lifelineno-1] = !arylifeline[lifelineno-1];
				if(lifelineno==0) lifelineno=oldlifeline;
				moneytree(i);
				lifelines();
				drawqbase();
				if (tag==0) gQIndex=getqid(i);
				if(tag==-1 && gQIndex==-1) gQIndex=getqid(i);
				//gQIndex=0;
				showquestion(gQIndex,tag);
				if (lifelineno!=2) oldlifeline=lifelineno;
				else if (lifelineno==2) lifelineno=oldlifeline;

				if (lifelineno==1) {
					apply5050();
					res=getans(1);
				} else if (lifelineno==2)
					lifelineno=oldlifeline;
				else if (lifelineno==3)	{
					tmp=getqid(i);
					if (tmp==gQIndex) gQIndex=(((tmp+1)%5)+(i*5));
					else gQIndex=tmp;
					tag=-1;
					i--;
					lifelineno=-1;
					continue;
				}
				else res=getans(0);

				if(res=='X')
				if(confirm()==1) break;
				else{tag=-1;	i--;	continue;}
				if (res=='F') {
					drawbase(BLUE);
					lifelineno=getlifelines();
					tag=-1;
					resume=0;
					i--;
					continue;
				}

				if (check(gQIndex,res)==0) break;
				tag=0;
				lifelineno=-1;
				resume=0;
				showmess(money[i],YELLOW,3);
				sleep(1);
			}
			tmp=getqid(0);	// just for messgae.

			strcpy(endmsg,"Total prize money won : ");

			if(res=='X') {
				if(i==0) strcat(endmsg,"0");
				else	strcat(endmsg,money[i-1]);

				showmess(endmsg,YELLOW,2);
				sleep(1);
			}
			else if(resume==-1) showmess("No previous details found.",GREEN,2);
			else if(i==15)	{
				if (tmp<2)	showmess("You won the quize!!!",YELLOW,2);
					else if(tmp<4)	showmess("You won 1,00,00,000 Rs.!!!",YELLOW,2);
					else if(tmp==4) showmess("You have create a history!!!",YELLOW,2);
					sleep(2);
			}
			else if(i<15)	{
				if(i > 9) {
					moneytree(9);
					strcat(endmsg,money[9]);
				}
				else if(i > 4) {
					moneytree(4);
					strcat(endmsg,money[4]);
				}
				else {
					moneytree(0);
					strcpy(endmsg,"Afshoooosh afshoooosh ...");
				}

				showmess(endmsg,YELLOW,2);
				sleep(1);
				resume=-1;
			}
			sleep(2);
		} else if (menuid == 4) {  // HELP
			float vtop, vleft, vMargin;
			struct textsettingstype textinfo;

			gettextsettings(&textinfo);

			cleardevice();
			drawlogo(GRAY);

			vMargin = 1;
			settextstyle(DEFAULT_FONT,textinfo.direction,1);

			vtop = (getmaxy() / 2) - ((textheight("A") * 2) * (int) (UBHELP / 2));

			for(i=0;i<UBHELP;i++){
				vleft = (getmaxx() / 2) - (textwidth(help[i]) / 2);

				setcolor(BLACK);
				moveto(vleft-vMargin,vtop+vMargin); outtext(help[i]);

				setcolor(BROWN);
				moveto(vleft,vtop); outtext(help[i]);

				vtop += (textheight("A") * 2);
			}

			settextstyle(textinfo.font,textinfo.direction,textinfo.charsize);
			bioskey(0);

		}
	}
	showcredit();
	bioskey(0);
	closegraph();
}

// *********************************
// Function to print money tree.
// *********************************
void moneytree(int index)
{
	char tmpstr[20];
	char icon[2];
	int i;
	float height,left, top;
	struct fillsettingstype fillinfo;

	icon[0]=(char) 4;
	icon[1]='\0';
	left=getmaxx()-100;
	height=(textheight("B")*1.8);
	top=getmaxy()-(height*14);

	/* Clearing base */
	getfillsettings(&fillinfo);
	setfillstyle(SOLID_FILL,BLACK);

	bar(left-textwidth(icon)-5,(top-(height * 16)),getmaxx(),top);

	setfillstyle(fillinfo.pattern,fillinfo.color);
	/* ****************** */

	for(i=0;i<15;i++)
	{
		moveto(left-textwidth(icon),top);
		top-=height;

			if (i-1<index)
			{
				setcolor(YELLOW);
				moveto(left-textwidth(icon)-5,top); outtext(icon);
			}

			if ((i+1)%5==0)	setcolor(YELLOW);
			else if (i>=index) setcolor(GRAY);
			else setcolor(WHITE);

		if(i==index)
		{
			setfillstyle(1,BROWN);
			bar(left,top-3,(left+textwidth(money[14])),(top+textheight(money[14])));
		}
		moveto(left,top);	outtext(money[i]);
//		delay(20);
	}
	textcolor(LIGHTGRAY);
}

/* *********************************
	Function to Draw Lifelines
	********************************* */
void lifelines()
{
	float xpos,ypos;
	char symb[1];
	char icon[2];
	xpos=getmaxx()-120;
	ypos=30;
	setcolor(WHITE);
	setfillstyle(1,BROWN);

/* **************************** 50:50 */
	if(arylifeline[0]==0) setfillstyle(1,GRAY);

	fillellipse(xpos,ypos,22,12);
	moveto(xpos-19,ypos-3);
	outtext("50:50");

// ****************************
	xpos+=(22*2)+5;
// **************************** Ask the friend
	if(arylifeline[1]==0) setfillstyle(1,GRAY);
	else setfillstyle(1,BROWN);

	fillellipse(xpos,ypos,22,12);
	icon[0]=(char) 17;
	icon[1]='\0';
	moveto(xpos-18,ypos-3);	outtext(icon);
	icon[0]=(char) 240;
	icon[1]='\0';
	moveto(xpos+textwidth(icon)-18,ypos-3);	outtext(icon);
	icon[0]=(char) 1;
	icon[1]='\0';
	moveto(xpos+(textwidth(icon)*3)-18,ypos-3);	outtext(icon);
// ****************************
	xpos+=(22*2) + 5;
// **************************** 4 / 2
	if(arylifeline[2]==0) setfillstyle(1,GRAY);
	else setfillstyle(1,BROWN);

	fillellipse(xpos,ypos,22,12);
	moveto(xpos-10,ypos-3);
	icon[0]=(char) 239;
	icon[1]='\0';
	outtext(icon);
	moveto(xpos-8,ypos-2);
	icon[0]=(char) 25;
	icon[1]='\0';
	outtext(icon);
	moveto(xpos+4,ypos-3);
	outtext("?");
// ****************************
}

// *************************************
// Function to Draw Base for Questions
// *************************************
void drawqbase()
{
	float left, top, lmargin, length;
	int color[]= {GRAY,GRAY,GRAY,WHITE,LIGHTGRAY,GRAY,GRAY,GRAY};
//	int color2[]= {GRAY,YELLOW,YELLOW,YELLOW,YELLOW,GRAY};
	float i;
	int j;
	struct fillsettingstype fillinfo;

	left=10;
	top=getmaxy()-(160);
	lmargin=30;
	length=getmaxx()-(lmargin+(left*3));

	/* Clearing base */
	getfillsettings(&fillinfo);
	setfillstyle(SOLID_FILL,BLACK);

	bar(left,top-lmargin,getmaxx(),getmaxy());

	setfillstyle(fillinfo.pattern,fillinfo.color);
	/* ****************** */

	for(i=0,j=0;i<3.5;i+=.5,j++)   // Question
		drawbox(left+i,top,lmargin-i,length-i,color[j]);

	length = (length/2)-lmargin;
	lmargin-=10;
	left+=13;

	for(i=1,j=0;i<4.0;i+=.5,j++)
		drawbox(left-3+i,top+60,lmargin-i,length-i,color[j]); // for Option1.

	for(i=1,j=0;i<4.0;i+=.5,j++)
		drawbox(left+length+(lmargin+left+3)+i,top+60, lmargin-i, length-i, color[j]); // for Option2.

	for(i=1,j=0;i<4.0;i+=.5,j++)
		drawbox(left-3+i,top+120, lmargin-i, length-i, color[j]); // for Option3.

	for(i=1,j=0;i<4.0;i+=.5,j++)
		drawbox(left+length+(lmargin+left+3)+i,top+120, lmargin-i,length-i, color[j]); // for Option4.
}

// *********************************
// Function to Draw Box
// *********************************
void drawbox(float left, float top, float lmargin, float length, int color)
{
	setcolor(color);
	line(left,top,left+lmargin,top-lmargin);	// : /
	line(left+lmargin,top-lmargin,(left+length),top-lmargin); // : ---
	line((left+length),top-lmargin,(left+length+(lmargin)),top); // : \
	line((left+length+(lmargin)),top,(left+length),top+lmargin); // : /
	line((left+length),top+lmargin,left+lmargin,top+lmargin); // : ---
	line(left+lmargin,top+lmargin,left,top); // : \
	setcolor(WHITE);
}

// *********************************
// Function to fill background Box
// *********************************
void fillbox(int option, int color)
{
	float left, top, lmargin, length, ypos, xpos;
	int i;

	setcolor(color);

	left=10;
	top=getmaxy()-(160);
	lmargin=16;
	length=getmaxx()-(lmargin+(left*3));

	length = (length/2)-lmargin-27;
	left+=(lmargin+2);

	switch(option)
	{
		case 1:
			left-=3;
			top+=60;
			break;
		case 2:
			left+=length+(lmargin*3)+4;
			top+=60;
			break;
		case 3:
			left-=3;
			top+=120;
			break;
		case 4:
			left+=length+(lmargin*3)+4;
			top+=120;
	}
	ypos=top-lmargin;
	for(ypos+=1,xpos=0;ypos<top+lmargin-1;ypos+=1)
	{
		if(ypos<=top) xpos+=1; else xpos-=1;
		line(left-xpos+lmargin+1,ypos,left+length+xpos-1,ypos);
	}
	setcolor(WHITE);
}

// *********************************
// Function to show Question Bank
// *********************************
int showquestion(int index, int option)
{
	int i;
	int left, top, lmargin, length;
	char a[]={"A: "};
	char b[]={"B: "};
	char c[]={"C: "};
	char d[]={"D: "};

	left=10;
	top=getmaxy()-(160);
	lmargin=30;
	length=getmaxx()-(lmargin+(left*3));

/*	setcolor(BLACK);
	setbkcolor(BLACK); */

	if (option == 0 || option==-1) prnstr(qbank[index][0],left+lmargin,(top-20),length); // Question
	if (option == 0) sleep(3);

	length = (length/2)-lmargin;
	lmargin-=10;
	left+=lmargin;

	fflush(stdin);	fflush(stdout);
	if (option==0 || option==1 || option==-1)
	{
		setcolor(YELLOW);
		moveto(left+lmargin,top+55);  outtext(a);
		setcolor(WHITE);
		moveto(left+lmargin+textwidth(a),top+55);  // Option A
		fflush(stdin);	fflush(stdout);
		outtext(qbank[index][1]);
		if (option == 0) delay(500);
	}

	fflush(stdin);	fflush(stdout);
	if (option==0 || option==2 || option==-1)
	{
		setcolor(YELLOW);
		moveto(((length*2)+lmargin+left+10)-(length-(lmargin*2)),top+55);	outtext(b);
		setcolor(WHITE);
		fflush(stdin);	fflush(stdout);
		moveto(((length*2)+lmargin+left+10)-(length-(lmargin*2))+textwidth(b),top+55);
		outtext(qbank[index][2]);
		if (option == 0) delay(500);
	}

	if (option==0 || option==3 || option==-1)
	{
		setcolor(YELLOW);
		moveto(left+lmargin,top+115);	outtext(c);
		setcolor(WHITE);
		moveto(left+lmargin+textwidth(c),top+115);	// Option C
		fflush(stdin);	fflush(stdout);
		outtext(qbank[index][3]);
		if (option == 0) delay(500);
	}

	if (option==0 || option==4 || option==-1)
	{
		setcolor(YELLOW);
		moveto(((length*2)+lmargin+left+10)-(length-(lmargin*2)),top+115);	outtext(d);
		setcolor(WHITE);
		moveto(((length*2)+lmargin+left+10)-(length-(lmargin*2))+textwidth(d),top+115);
		fflush(stdin);	fflush(stdout);
		outtext(qbank[index][4]);
	}
	return(index);
}

// ****************************************
// Function to print string
// ****************************************
void prnstr(char *str, float xpos, float ypos, int length)
{
	int i=0;
	char tmpstr[100];
	float realxpos;
	realxpos=xpos;

	ypos -= textheight("B")-5;

	while(1) {
		*tmpstr='\0';

		fflush(stdin);	fflush(stdout);

		for (i=0; (*str) && (textwidth(tmpstr) < (length-textwidth("AB"))); i++)
			tmpstr[i] = *str++;

		fflush(stdin);	fflush(stdout);

//		if(*str != ' ' && *str) tmpstr[i++] = '-';
		tmpstr[i] = '\0';

		xpos=realxpos+((length/2)-(textwidth(tmpstr)/2));
		ypos += textheight("B")+5;
		moveto(xpos,ypos);	outtext(tmpstr);

		if(!(*str)) return;
	}
}

// *************************************
// Function to get answer from keyprees
// *************************************
char getans(int mode)
{
	long int key1, key2;
	char keychar='\0', res;
	int diff;
	float lmargin, length;

//	left=10;
//	top=getmaxy()-(160);
	length = (length/2)-5;
	lmargin-=10;

	do
	{
		fflush(stdin);	fflush(stdout);
		key1=bioskey(0);
		res=convert(key1);
		moveto(100,100);

		if ((res>='A' && res<='D') && (mode==0 || mode==1))
		{
			if ((res!=keychar) && (keychar!='\0'))
			{
				fillbox(keychar-65+1,BLACK);
				showquestion(gQIndex,keychar-65+1);
			}

			keychar=res;
			if (mode==1)  // 50:50 mode
				if (res==qans[gQIndex][0] || res==qans[gQIndex][1])
					fillbox(keychar-65+1,GRAY);
				else	keychar='\0'; /* refreshing the value of variable
											  to make that normal */
			else fillbox(keychar-65+1,GRAY);
			showquestion(gQIndex,keychar-65+1);
		}
		else if (res=='>' && mode==2) return('1'); // getting input for lifelines.
		else if (res=='<' && mode==2) return('0'); // getting input for lifelines.
		else if (res=='R' && mode==2) return('R'); // getting input for lifelines.
		else if ((res=='R' && keychar!='\0') && (mode==0 || mode==1)) return(keychar); // normal mode
		else if (res=='F' && (mode==0 || mode==1)) return (res); //Normal mode- F1 key pressed
		else if (res=='X') return(res);
	} while(1);
}

// **********************************
// Function to apply 50 : 50 lifeline
// **********************************
void apply5050()
{
	if(qans[gQIndex][0] != 'A' && qans[gQIndex][1] !='A')	fillbox(1,BLACK);
	if(qans[gQIndex][0] != 'B' && qans[gQIndex][1] !='B')	fillbox(2,BLACK);
	if(qans[gQIndex][0] != 'C' && qans[gQIndex][1] !='C')	fillbox(3,BLACK);
	if(qans[gQIndex][0] != 'D' && qans[gQIndex][1] !='D')	fillbox(4,BLACK);
}

// *********************************
// Function to covert long integer
// into respective character
// *********************************
char convert(long int index)
{
	char key;

	if (index==7777 || index == 7745) key = 'A';
	if (index==12386 || index == 12354) key = 'B';
	if (index==11875 || index == 11843) key = 'C';
	if (index==8292 || index == 8260) key = 'D';
	if (index==7181) key='R';	 // {Enter}
	if (index==283) key='X';	 // {Esc}
	if (index==15104) key='F';	 // {F1}
	if (index==19712) key='>';	 // {Right Arow Key}
	if (index==19200) key='<';	 // {Left Arrow Key}
	if (index==18432) key='^';	 // {Up Arrow Key}
	if (index==20480) key='!';	 // {Down Arrow Key}
	return(key);
}

// *********************************
// Function to get question's index
// *********************************
int getqid(int index)
{
	int id=5,i;
	time_t tmr;
	tmr=time(NULL);
	id*=index;
	i= (int) tmr;
	if (i<0) i*=-1;
	id+=(i%5);
	return(id);
}

// ************************************
// Function to check the entered option
// ************************************
int check(int index, char option)
{
	int i;

	fillbox(option-65+1,BROWN);
	showquestion(gQIndex,option-65+1);
	sleep(2);

	for(i=0;i<=8;i++)
	{
		if (qans[index][0]!=option)
		  if (i%2==0) fillbox(option-65+1,RED);
		  else fillbox(option-65+1,BROWN);

		  if (i%2==1) fillbox(qans[index][0]-65+1,GREEN);
		  else
		  {
			  if (qans[index][0]==option)	fillbox(qans[index][0]-65+1,BROWN);
				else 	fillbox(qans[index][0]-65+1,BLACK);
		  }

		  showquestion(gQIndex,option-65+1);
		  showquestion(gQIndex,qans[index][0]-65+1);
		  delay(500);
	}
	if (qans[index][0]==option) return(1); else return(0);
}

// ************************************
// Function to check the entered option
// ************************************
void drawbase(int bkcolor)
{
	float left, top, lmargin, length, i, dist;
	int color[]= {BLUE,BLUE,BLUE,LIGHTBLUE,LIGHTBLUE,BLUE,BLUE,BLUE};
	int j;
	lmargin=(float) 40;
	left=(float) 5;
	top=(float) (getmaxy()-(40*2))+30;
	length=(float) getmaxx()-(lmargin+15);

// ******** Filling background
	setcolor(bkcolor);
//	ypos=top;
//	xpos=left;

	// Second half
	dist=lmargin;
	for(j=(int)i;j<=length+lmargin;j+=2)
	{
		if(j+lmargin>=length) dist--;
		line(left+j,top,left+dist+j,top+dist);
	}

	// First half
	dist=lmargin;
	for(j=(int)length+lmargin;j>i;j-=2)
	{
		if(j-lmargin<=lmargin+left) dist--;
		line(left+j,top,left-dist+j,top-dist);
	}

	// Outer Border
	for(i=0,j=0;i<=3.5;i+=.5,j++)
		drawbox(left+i,top,lmargin-i,length-i,color[j]);
}

// *********************************
// Function to get life lines
// *********************************
int getlifelines()
{
	float lmargin, ypos, xpos, i, dist, rad1, rad2;
	int color[]= {GRAY,GRAY,GRAY,WHITE,LIGHTGRAY,GRAY,GRAY,GRAY,LIGHTGRAY,GRAY};
	int j,count,lineid;
	char icon[3];
	char res;
	long int key1,key2;

	struct textsettingstype textinfo;

	if (arylifeline[0]==1) lineid=0;
	else if (arylifeline[1]==1) lineid=1;
	else if (arylifeline[2]==1) lineid=2;
	else lineid=-2;

	gettextsettings(&textinfo);

	do
	{
		lmargin=40;
//		length=getmaxx()-(lmargin+15);
		rad1=45;
		rad2=28;
		xpos=lmargin+rad1+20;
		ypos=(getmaxy()-(40*2))+30;

		for(count=0;count<3;count++)
		{
			if (lineid==count) setfillstyle(1,BROWN);
			else	setfillstyle(1,LIGHTGRAY);

			if(arylifeline[count]==0) setfillstyle(1,GRAY);

			fillellipse(xpos,ypos,rad1-.5,rad2-.5);
				for(i=0,j=0;i<=2.7;i+=.3,j++)
				{
					setcolor(color[j]);
					ellipse(xpos,ypos,360,0,rad1-i,rad2-i);
				}
			xpos+=(rad1*4)+30;
		}

		settextstyle(DEFAULT_FONT,textinfo.direction,2);
		setcolor(WHITE);

		xpos=lmargin+rad1-19;
		moveto(xpos,ypos-5);	outtext("50:50");

		xpos+=(rad1*4)+34;
		icon[0]=(char) 17;
		icon[1]='\0';
		moveto(xpos,ypos-5);	outtext(icon);
		icon[0]=(char) 240;
		icon[1]='\0';
		moveto(xpos+textwidth(icon),ypos-5);	outtext(icon);
		icon[0]=(char) 1;
		icon[1]='\0';
		moveto(xpos+(textwidth(icon)*3),ypos-5);	outtext(icon);

		xpos+=(rad1*4)+48;
		moveto(xpos,ypos-5);
		icon[0]=(char) 239;
		icon[1]='\0';
		outtext(icon);
		moveto(xpos+4,ypos-2);
		icon[0]=(char) 25;
		icon[1]='\0';
		outtext(icon);
		moveto(xpos+30,ypos-3);
		outtext("?");
		res=getans(2);
		if (res=='0' && (arylifeline[lineid-1]!=0) && lineid!=-2) lineid--;
		else if (res=='1' && (arylifeline[(lineid+1)%3]!=0) && lineid!=-2) lineid++;
		else if (res=='X') break;
		else if (res=='R') break;

		if(lineid<0 && lineid!=-2 && arylifeline[2]!=0) lineid=2;
		else if(lineid>2 && lineid!=-2) lineid=0;
		else if(lineid<0 && lineid!=-2 && arylifeline[2]==0) lineid=0;
	} while(1);

// re-setting text environment
	settextstyle(textinfo.font,textinfo.direction,textinfo.charsize);
	if (res=='R')	return(lineid+1);
	else if(res=='X') return(0);
}

// *********************************
// Function to darw logo
// *********************************
void drawlogo(int colorid)
{
	float midx,midy,rad1,rad2,mar,lpos,ypos;
	int i,DRK, LIT;
	int color1[]= {BLUE,BLUE,BLUE,LIGHTBLUE,LIGHTBLUE,BLUE,BLUE,BLUE};
	int color2[]= {GRAY,GRAY,GRAY,LIGHTGRAY,LIGHTGRAY,GRAY,GRAY,GRAY};
	int *color;

//	char *head1="KAUN BANEGA";
	char *head2="CROREPATI";

	struct fillsettingstype fillinfo;
	struct textsettingstype textinfo;

	gettextsettings(&textinfo);

	midx=(getmaxx()/2);
	if(colorid == BLUE)	midy=(getmaxy()/2)-97;
	else midy=(getmaxy()/2);
	rad1=rad2=140;


	/* Clearing base */
	getfillsettings(&fillinfo);
	setfillstyle(SOLID_FILL,BLACK);

	bar(midx-rad1,midy-rad1,midx+rad1,midy+rad1);

	setfillstyle(fillinfo.pattern,fillinfo.color);
	/* ****************** */


	if(colorid == BLUE){
		color=color1;
		LIT=LIGHTBLUE;
		DRK=BLUE;
	} else {
		LIT=LIGHTGRAY;
		DRK=GRAY;
		color=color2;
	}

	setfillstyle(1,DRK); setcolor(DRK);
	pieslice(midx,midy,0,360,rad1-1);

	for(i=0,mar=0;i<8;i++,mar+=.3)
	{
		setcolor(color[i]);
		circle(midx,midy,rad1-mar);
	}

	rad1-=30;
	setfillstyle(1,LIT); setcolor(LIT);
	pieslice(midx,midy,0,360,rad1);
	for(i=0,mar=0;i<8;i++,mar+=.3)
	{
		setcolor(color[i]);
		circle(midx,midy,(rad1)-mar);
	}
	setcolor(DRK);

	rad2=3;
	midx+=1;
	for(i=0;i<11;i++,rad2+=10)
	{
		ellipse(midx,midy,270,90,rad1-5,rad2);
		ellipse(midx,midy,90,270,rad1-5,rad2);
		ellipse(midx,midy,270,90,rad2,rad1-5);
		ellipse(midx,midy,90,270,rad2,rad1-5);
	}
	if(colorid == BLUE) {
		setcolor(LIGHTGRAY);
		settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
		lpos=midx-(textwidth(head2)/2)+3;
		ypos=midy-(textheight(head2)/2);
		moveto(lpos,ypos);	outtext(head2);
	}
	settextstyle(textinfo.font,textinfo.direction,textinfo.charsize);
}

// *********************************
// Function to confirm your input
// *********************************
int confirm()
{
	float xpos, ypos, midx, midy,len,height,cmd_len,cmd_height,cmd_dist;
	int i,cmd_id=2;
	char *mess="Wish to quit?";
	char *cap_t="Yes";
	char *cap_f="No";
	char res;

	midx=(getmaxx()/2);
	midy=(getmaxy()/2);
	xpos=midx-100;
	ypos=midx-150;
	len=200;
	height=100;
	cmd_len=50;
	cmd_height=20;
	cmd_dist=100;

	setcolor(LIGHTRED);
	for(i=0;i<len;i+=2) line(xpos+i,ypos,xpos+i,ypos+height);
	for(i=0;i<height;i+=2) line(xpos,ypos+i,xpos+len,ypos+i);

	setcolor(WHITE);
	rectangle(xpos,ypos,xpos+200,ypos+100);

	setcolor(BLACK);
	moveto(xpos+((len/2)-(textwidth(mess)/2))-1,ypos+20+1);
	outtext(mess);

	setcolor(YELLOW);
	moveto(xpos+((len/2)-(textwidth(mess)/2)),ypos+20);
	outtext(mess);

	do
	{
		xpos=(midx-(len/2))+25;
		ypos=(midy+(height/2))-45;

		if(cmd_id==1) setfillstyle(1,GRAY); else setfillstyle(1,LIGHTGRAY);
		bar(xpos-2,ypos+2,xpos+(cmd_len-2),ypos+(cmd_height+2));
		xpos+=cmd_dist;
		if(cmd_id==2) setfillstyle(1,GRAY); else setfillstyle(1,LIGHTGRAY);
		bar(xpos-2,ypos+2,xpos+(cmd_len-2),ypos+(cmd_height+2));

		xpos-=cmd_dist;
		setcolor(BLACK);
		setfillstyle(1,LIGHTGRAY);

		bar(xpos,ypos,xpos+cmd_len,ypos+cmd_height);
		moveto((xpos+(cmd_len/2))-(textwidth(cap_t)/2),(ypos+(cmd_height/2))-(textheight(cap_t)/2));
		outtext(cap_t);

		xpos+=cmd_dist;
		bar(xpos,ypos,xpos+cmd_len,ypos+cmd_height);
		moveto((xpos+(cmd_len/2))-(textwidth(cap_f)/2),(ypos+(cmd_height/2))-(textheight(cap_f)/2));
		outtext(cap_f);

		res=convert(bioskey(0));
		if(res=='>' && cmd_id==1) cmd_id=2;
		else if(res=='<' && cmd_id==2) cmd_id=1;
		else if(res=='R' || res=='X') break;
	} while(1);

	drawlogo(BLUE);

	if (res=='R') return(cmd_id%2);
	else if(res=='X') return(0);
}

// *********************************
// Function to show your message
// *********************************
void showmess(char *mess,int color, int size)
{
	float left, top, lmargin, length, ypos, xpos, dist;
	struct textsettingstype textinfo;

	gettextsettings(&textinfo);

	lmargin=40;
	left=5;
	top=(getmaxy()-(40*2))+30;
	length=getmaxx()-(lmargin+15);

	drawbase(LIGHTBLUE);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,size);
	xpos=((left+lmargin)+(length/2))-(textwidth(mess)/2);
	ypos=top-(textheight(mess))/2;

	setcolor(BLACK);
	moveto(xpos-2,ypos+2);	outtext(mess);

	setcolor(color);
	moveto(xpos,ypos);	outtext(mess);

	settextstyle(textinfo.font,textinfo.direction,textinfo.charsize);
}

// *********************************
// Function to show credit window
// *********************************
void showcredit()
{
	float midx,midy,lpos,ypos;
	int i;
	char *head, *addi1;
	char *text[6];
	struct textsettingstype textinfo;

	gettextsettings(&textinfo);
	midx=getmaxx()/2;
	midy=getmaxy()/2;

	text[0]="Anand Singh Jalal";
	text[1]="Mahesh Chand Gupta";
	text[2]="Manish Gupta";
	text[3]="Abhishek Singhal";
	text[4]="Deepak Gautam";
	text[5]="Bankey Bihari Sharma";

	for(i=0;(kbhit()==0 && i<6);i++)
	{
		cleardevice();
		if(i==5) head="Developed by";
		else if(i==0 || i==1) head="Special Thanks";
		else head="Credit";

		if(i==0) addi1="Class Advisor";
		else if(i==1) addi1="Lab Incharge";
		else	addi1="MCA 1st Year";

		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		lpos=midx-(textwidth(head)/2);
		ypos=midy-textheight(head)*2;
		setcolor(GRAY);
		outtextxy(lpos-2,ypos+2,head);
		setcolor(LIGHTGRAY);
		outtextxy(lpos,ypos,head);

		settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
		ypos=midy;
		lpos=midx-(textwidth(text[i])/2);
		setcolor(GRAY);
		outtextxy(lpos-2,ypos+2,text[i]);
		setcolor(WHITE);
		outtextxy(lpos,ypos,text[i]);

		ypos=midy+textheight(text[i])+10;
		lpos=midx+(textwidth(text[i])/2);
		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		lpos-=textwidth(addi1);
		setcolor(GRAY);
		outtextxy(lpos-2,ypos+2,addi1);
		setcolor(LIGHTGRAY);
		outtextxy(lpos,ypos,addi1);
		delay(2000);
	}
	settextstyle(textinfo.font,textinfo.direction,textinfo.charsize);
}

// *********************************
// Function to Load Question Bank
// *********************************
int getmenu()
{
	int menuid=0,oldid=-1,i;
	float midx,midy,xpos,ypos;
	char res;
	struct textsettingstype textinfo;

	cleardevice();
	gettextsettings(&textinfo);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	midx=(float) getmaxx()/2;
	midy=(float) getmaxy()/2;

	do
	{
		ypos=midy-(textheight(menu[0])*5);
		for(i=0;i<6;i++)
		{
			if(menuid==oldid) break;
			xpos=midx-(textwidth(menu[i])/2);
			if(i==menuid) setcolor(GRAY);
			else  setcolor(BLACK);
			moveto(xpos-2,ypos+2);
			outtext(menu[i]);

			if(i==menuid) setcolor(WHITE);
			else setcolor(LIGHTGRAY);
			moveto(xpos,ypos);
			outtext(menu[i]);

			ypos+=textheight(menu[i])*2;
		}
		oldid=menuid;
		res=convert(bioskey(0));
		if(res=='^' && menuid>0) menuid--;	// if up key pressed
		else if(res=='!' && menuid<5)	menuid++; // if down key pressed
		else if(res=='R')	break;
		else if(res=='X')  /* to quit the program on {Esc} */
		{
			menuid=5;
			break;
		}
	} while(1);
	settextstyle(textinfo.font,textinfo.direction,textinfo.charsize);
	return(menuid+1);
}

// *********************************
// Function to Load Question Bank
// *********************************
void loadqbank(void)
{
// 1. ****************************************************
	qbank[0][0] = "Longest railway of the world?";
		qbank[0][1] = "Jammu-Kanyakumari";	qbank[0][2] = "Trans-Siberian";
		qbank[0][3] = "Newyork-SentLord";	qbank[0][4] = "None of the above";
	qans[0][0] = 'B';
	qans[0][1] = 'C';

	qbank[1][0] = "Largest island of the world?";
		qbank[1][1] = "Greenland";	qbank[1][2] = "Switzerland";
		qbank[1][3] = "Newzeland";	qbank[1][4] = "Africa";
	qans[1][0] = 'A';
	qans[1][1] = 'C';

	qbank[2][0] = "Largest desert of Asia?";
		qbank[2][1] = "Gobbi";	qbank[2][2] = "Kummu";
		qbank[2][3] = "Thar";	qbank[2][4] = "Suru";
	qans[2][0] = 'A';
	qans[2][1] = 'B';

	qbank[3][0] = "Jalian wala bagh tragedy took place in...?";
		qbank[3][1] = "1925";	qbank[3][2] = "1929";
		qbank[3][3] = "1919";	qbank[3][4] = "1909";
	qans[3][0] = 'C';
	qans[3][1] = 'D';

	qbank[4][0] = "Who was the writer of great hindu epic \'MAHABHARAT\'?";
		qbank[4][1] = "Valmiki";	qbank[4][2] = "Shri Ganesh";
		qbank[4][3] = "Tulsidas";	qbank[4][4] = "Ved Vyasa";
	qans[4][0] = 'B';
	qans[4][1] = 'D';

// 2. ***********************************************************
	qbank[5][0] = "Artificial radioactivity was discovered by?";
		qbank[5][1] = "Madam Marie Curie";	qbank[5][2] = "Curie, Joliot and Iren";
		qbank[5][3] = "George Mendel";		qbank[5][4] = "Sir C.V. Raman";
	qans[5][0] = 'B';
	qans[5][1] = 'A';

	qbank[6][0] = "Which of the following dances requires mask wearing?";
		qbank[6][1] = "Kathakali";	qbank[6][2] = "Manipuri";
		qbank[6][3] = "Kuchipuri";	qbank[6][4] = "Odisy";
	qans[6][0] = 'A';
	qans[6][1] = 'D';

	qbank[7][0] = "\'Forward Block\' was started by...?";
		qbank[7][1] = "J.L. Nehru";	qbank[7][2] = "Bal G. Tilak";
		qbank[7][3] = "S.C. Bose";		qbank[7][4] = "Not given";
	qans[7][0] = 'C';
	qans[7][1] = 'B';

	qbank[8][0] = "The deepest ocean of the world?";
		qbank[8][1] = "Pacific";	qbank[8][2] = "Indian";
		qbank[8][3] = "Arabian";	qbank[8][4] = "None";
	qans[8][0] = 'A';
	qans[8][1] = 'C';

	qbank[9][0] = "The country with largest electorate is...?";
		qbank[9][1] = "China";		qbank[9][2] = "America";
		qbank[9][3] = "U.S.S.R.";	qbank[9][4] = "India";
	qans[9][0] = 'D';
	qans[9][1] = 'A';

// 3. ***********************************************************
	qbank[10][0] = "The bigest dome of the world?";
		qbank[10][1] = "Aris University";	qbank[10][2] = "Gol gumbaz";
		qbank[10][3] = "Taj Mahal";			qbank[10][4] = "Lal kila";
	qans[10][0] = 'B';
	qans[10][1] = 'A';

	qbank[11][0] = "The largest river of the world?";
		qbank[11][1] = "Amazon";	qbank[11][2] = "Brihmaputra";
		qbank[11][3] = "Ganga";		qbank[11][4] = "Misisipy";
	qans[11][0] = 'A';
	qans[11][1] = 'B';

	qbank[12][0] = "Fastest bird of the world?";
		qbank[12][1] = "Swift";			qbank[12][2] = "Humming bird";
		qbank[12][3] = "Twiny-bird";	qbank[12][4] = "Eagel";
	qans[12][0] = 'A';
	qans[12][1] = 'D';

	qbank[13][0] = "The only wing less bird in the world is...?";
		qbank[13][1] = "Ostrich";	qbank[13][2] = "Kiwi";
		qbank[13][3] = "Pengvin";	qbank[13][4] = "None";
	qans[13][0] = 'B';
	qans[13][1] = 'C';

	qbank[14][0] = "... is tree that weeps?";
		qbank[14][1] = "Palm";			qbank[14][2] = "Oak";
		qbank[14][3] = "Lauranmask";	qbank[14][4] = "Laurel";
	qans[14][0] = 'D';
	qans[14][1] = 'C';

// 4. ***********************************************************
	qbank[15][0] = "Omar khayam, the great poet was basically from...?";
		qbank[15][1] = "Persia";	qbank[15][2] = "Rome";
		qbank[15][3] = "Pakistan";	qbank[15][4] = "India";
	qans[15][0] = 'A';
	qans[15][1] = 'D';

	qbank[16][0] = "C# was announced by Microsoft in...";
		qbank[16][1] = "May'2000";	qbank[16][2] = "June'2000";
		qbank[16][3] = "Jan'2001";	qbank[16][4] = "Mar'2001";
	qans[16][0] = 'B';
	qans[16][1] = 'D';

	qbank[17][0] = "Category of C language is?";
		qbank[17][1] = "Low Level";		qbank[17][2] = "System Level";
		qbank[17][3] = "Middle Level";	qbank[17][4] = "High Level";
	qans[17][0] = 'C';
	qans[17][1] = 'D';

	qbank[18][0] = "\'Vande-Matram\' was taken from?";
		qbank[18][1] = "Gitanjali";	qbank[18][2] = "Anand math";
		qbank[18][3] = "Hind-Vahini";	qbank[18][4] = "Kapal-Kundala";
	qans[18][0] = 'B';
	qans[18][1] = 'C';

	qbank[19][0] = "Writer of 'Harsha Chritra' was...?";
		qbank[19][1] = "Ban Bhatt";				qbank[19][2] = "S.N. Banrjee";
		qbank[19][3] = "Bankim C. Chatterje";	qbank[19][4] = "Kalidas";
	qans[19][0] = 'A';
	qans[19][1] = 'D';

// 5. ***********************************************************
	qbank[20][0] = "Christopher Cloumbus was a navigator from...?";
		qbank[20][1] = "America";	qbank[20][2] = "Italy";
		qbank[20][3] = "Egypt";		qbank[20][4] = "France";
	qans[20][0] = 'B';
	qans[20][1] = 'D';

	qbank[21][0] = "Mona Lisa was painted by the italian painter...?";
		qbank[21][1] = "Li Sing";			qbank[21][2] = "Lionardo the vincci";
		qbank[21][3] = "Rafter G. Dan";	qbank[21][4] = "None";
	qans[21][0] = 'B';
	qans[21][1] = 'A';

	qbank[22][0] = "Who was the founder of \'National Movement and Kashmir State Confrence'?";
		qbank[22][1] = "M. Ali jinna";		qbank[22][2] = "A. Shekha Mohhamad";
		qbank[22][3] = "Farukh Abdullah";	qbank[22][4] = "J.L. Nehru";
	qans[22][0] = 'B';
	qans[22][1] = 'A';

	qbank[23][0] = "Who was the first man to see the satellite of the Jupiter?";
		qbank[23][1] = "Gland Stoner";	qbank[23][2] = "Gibbon Edward";
		qbank[23][3] = "Gibbon Field";	qbank[23][4] = "Galelio";
	qans[23][0] = 'D';
	qans[23][1] = 'B';

	qbank[24][0] = "Who discover that plants have nervous system?";
		qbank[24][1] = "Sir. J.C. Bose";		qbank[24][2] = "Sir G.K. Gokhale";
		qbank[24][3] = "Sir H.G. Khurana";	qbank[24][4] = "Sir. H.J. Bhabha";
	qans[24][0] = 'A';
	qans[24][1] = 'C';

// 6. ***********************************************************
	qbank[25][0] = "\'J.P.' is Stands for?";
		qbank[25][1] = "Justice of the Peace";	qbank[25][2] = "Janta Party";
		qbank[25][3] = "Jargons & Principal";	qbank[25][4] = "Jay Prakash Narayan";
	qans[25][0] = 'D';
	qans[25][1] = 'A';

	qbank[26][0] = "The decimal number system was introduced by ...?";
		qbank[26][1] = "Egyptiens";	qbank[26][2] = "Indians";
		qbank[26][3] = "Chines";		qbank[26][4] = "Russian";
	qans[26][0] = 'B';
	qans[26][1] = 'C';

	qbank[27][0] = "Which is known as the \'Roof of the World'?";
		qbank[27][1] = "The Pamris";	qbank[27][2] = "Venice";
		qbank[27][3] = "Norway";		qbank[27][4] = "Canada";
	qans[27][0] = 'A';
	qans[27][1] = 'B';

	qbank[28][0] = "... happens when moon comes in between the sun and earth?";
		qbank[28][1] = "Lunar eclips";	qbank[28][2] = "Solar eclips";
		qbank[28][3] = "Black hole";		qbank[28][4] = "Night";
	qans[28][0] = 'B';
	qans[28][1] = 'A';

	qbank[29][0] = "First battle of \'Panipat' was fought between?";
		qbank[29][1] = "Babar-Ibrahim lodhi";	qbank[29][2] = "Babar-Rana Sanga";
		qbank[29][3] = "Bairam Khan-Hemu";		qbank[29][4] = "Akbar-Rana Pratap";
	qans[29][0] = 'A';
	qans[29][1] = 'C';

// 7. ***********************************************************
	qbank[30][0] = "R.A.W. is stands for?";
		qbank[30][1] = "Research & Analysis Wing";	qbank[30][2] = "Rear Admiral of War";
		qbank[30][3] = "Roman Atlantic Wing";			qbank[30][4] = "None of the above";
	qans[30][0] = 'A';
	qans[30][1] = 'C';

	qbank[31][0] = "What is the following represents the \'Play ground of europe'?";
		qbank[31][1] = "Egypt";		qbank[31][2] = "Switzerland";
		qbank[31][3] = "Canada";	qbank[31][4] = "Cuba";
	qans[31][0] = 'B';
	qans[31][1] = 'C';

	qbank[32][0] = "A bill of exchange payable on order, is known as?";
		qbank[32][1] = "Cheque";		qbank[32][2] = "Demand Draft";
		qbank[32][3] = "Overdraft";	qbank[32][4] = "None";
	qans[32][0] = 'B';
	qans[32][1] = 'A';

	qbank[33][0] = "A situation in which further negotiations are impossible.";
		qbank[33][1] = "Dead lock";	qbank[33][2] = "Die Hard";
		qbank[33][3] = "Curfew";		qbank[33][4] = "Envoy";
	qans[33][0] = 'A';
	qans[33][1] = 'C';

	qbank[34][0] = "The language spoken by the largest number of people is?";
		qbank[34][1] = "Hindi";		qbank[34][2] = "English";
		qbank[34][3] = "Chinies";	qbank[34][4] = "Urdu";
	qans[34][0] = 'C';
	qans[34][1] = 'B';

// 8. ***********************************************************
	qbank[35][0] = "The first Hot Line was set-up between ...?";
		qbank[35][1] = "Whitehouse-Kremlin";	qbank[35][2] = "Kremlin-Pantagon";
		qbank[35][3] = "Whitehouse-Rastelin";	qbank[35][4] = "Pantagon-Whitehouse";
	qans[35][0] = 'B';
	qans[35][1] = 'A';

	qbank[36][0] = "Which of the country has the system of dual citizenship?";
		qbank[36][1] = "U.S.A.";		qbank[36][2] = "U.S.S.R.";
		qbank[36][3] = "Bangladesh";	qbank[36][4] = "Australia";
	qans[36][0] = 'A';
	qans[36][1] = 'D';

	qbank[37][0] = "\'Monte Carlo' is a famous place for... in france?";
		qbank[37][1] = "Gambling";			qbank[37][2] = "Polo";
		qbank[37][3] = "Hill Station";	qbank[37][4] = "Lovers point";
	qans[37][0] = 'A';
	qans[37][1] = 'C';

	qbank[38][0] = "Stone image of lion with a man's head carved on it is known as?";
		qbank[38][1] = "Statue of liberty";	qbank[38][2] = "Leo";
		qbank[38][3] = "Narsinha";				qbank[38][4] = "Sphinx";
	qans[38][0] = 'D';
	qans[38][1] = 'C';

	qbank[39][0] = "In which the term \'Bully' is used?";
		qbank[39][1] = "Football";		qbank[39][2] = "Hockey";
		qbank[39][3] = "Volleyball";	qbank[39][4] = "Badminton";
	qans[39][0] = 'B';
	qans[39][1] = 'C';

// 9. ***********************************************************
	qbank[40][0] = "Writer of \'Indian Philosophy\'?";
		qbank[40][1] = "Dr. S. Radhakrishnan";	qbank[40][2] = "Sir C.V. Raman";
		qbank[40][3] = "Rahul Sankrityayan";	qbank[40][4] = "A.B. Vajpayee";
	qans[40][0] = 'A';
	qans[40][1] = 'C';

	qbank[41][0] = "Who was known as \'National Scholar\'?";
		qbank[41][1] = "Dr. H.G. Khurana";	qbank[41][2] = "Sir C.V. Raman";
		qbank[41][3] = "Vivekanand";			qbank[41][4] = "Dr. J.C. Bose";
	qans[41][0] = 'B';
	qans[41][1] = 'C';

	qbank[42][0] = "A place of pilgrimage for the Buddhist, known for buddhist stupas?";
		qbank[42][1] = "Ranchi";	qbank[42][2] = "Gaya";
		qbank[42][3] = "Khilwada";	qbank[42][4] = "Sanchi";
	qans[42][0] = 'D';
	qans[42][1] = 'A';

	qbank[43][0] = "Nalanda was an...university?";
		qbank[43][1] = "Philosophy";		qbank[43][2] = "Political";
		qbank[43][3] = "Bhuddhist";		qbank[43][4] = "Sanskrit";
	qans[43][0] = 'C';
	qans[43][1] = 'A';

	qbank[44][0] = "Kutub Minar was build by?";
		qbank[44][1] = "Altmash";	qbank[44][2] = "Akbar";
		qbank[44][3] = "Babar";		qbank[44][4] = "Aurangjeb";
	qans[44][0] = 'A';
	qans[44][1] = 'B';

// 10. ***********************************************************
	qbank[45][0] = "Napoleon I. Bornapart was the commander-in-chief of...?";
		qbank[45][1] = "Britain";	qbank[45][2] = "Germany";
		qbank[45][3] = "Austrailia";	qbank[45][4] = "France";
	qans[45][0] = 'D';
	qans[45][1] = 'B';

	qbank[46][0] = "Dr. Hargovinda Khurana was awarded the Nobel Prize for?";
		qbank[46][1] = "Physiology and Medicine";	qbank[46][2] = "Genetic Codes";
		qbank[46][3] = "Rules of Electricity";		qbank[46][4] = "None of the above";
	qans[46][0] = 'B';
	qans[46][1] = 'D';

	qbank[47][0] = "The line between India-Pakistan's territory?";
		qbank[47][1] = "McMohan Line";		qbank[47][2] = "Raid Cliph Line";
		qbank[47][3] = "Nehru-Jinna Line";	qbank[47][4] = "No line present";
	qans[47][0] = 'B';
	qans[47][1] = 'A';

	qbank[48][0] = "Founder of ORACLE Corporation...?";
		qbank[48][1] = "Paul Allen";		qbank[48][2] = "Bill Gates";
		qbank[48][3] = "Lauren Alision";	qbank[48][4] = "G.W. Bush";
	qans[48][0] = 'C';
	qans[48][1] = 'A';

	qbank[49][0] = "Who among the following have not believed in the DRAIN THEORY of Dada Bhai Nauroji";
		qbank[49][1] = "Bal Gangadhar Tilak";	qbank[49][2] = "R.C. Dutta";
		qbank[49][3] = "M.G. Ranaday";			qbank[49][4] = "Sir Said A. Khan";
	qans[49][0] = 'A';
	qans[49][1] = 'D';

// 11. ***********************************************************
	qbank[50][0] = "Official Report of British Government are known as?";
		qbank[50][1] = "Yellow Book";	qbank[50][2] = "White Book";
		qbank[50][3] = "Blue Book";	qbank[50][4] = "White Papper";
	qans[50][0] = 'C';
	qans[50][1] = 'B';

	qbank[51][0] = "Cleopatra, the famous egyptian queen was the wife of?";
		qbank[51][1] = "Julius Caesar";	qbank[51][2] = "Alexzendar";
		qbank[51][3] = "Chalis";			qbank[51][4] = "Lord Lobert";
	qans[51][0] = 'A';
	qans[51][1] = 'C';

	qbank[52][0] = "Adolf hitler was born in?";
		qbank[52][1] = "Germany";		qbank[52][2] = "India";
		qbank[52][3] = "Austrailia";	qbank[52][4] = "France";
	qans[52][0] = 'C';
	qans[52][1] = 'A';

	qbank[53][0] = "Which of the SAARC member is not in the G-15 Group?";
		qbank[53][1] = "India";	qbank[53][2] = "Sri Lanka";
		qbank[53][3] = "Nepal";	qbank[53][4] = "None";
	qans[53][0] = 'C';
	qans[53][1] = 'D';

	qbank[54][0] = "EULA if related with?";
		qbank[54][1] = "Software";		qbank[54][2] = "Elections";
		qbank[54][3] = "Money";			qbank[54][4] = "All";
	qans[54][0] = 'A';
	qans[54][1] = 'D';

// 12. ***********************************************************
	qbank[55][0] = "The \'Congress Socialist Party' was formed by...?";
		qbank[55][1] = "M.C. Gandhi";		qbank[55][2] = "M. Ali Jinna";
		qbank[55][3] = "Bhagat Singh";	qbank[55][4] = "J. P. Narayan";
	qans[55][0] = 'D';
	qans[55][1] = 'B';

	qbank[56][0] = "Who was the architect of Rashtrapati Bhawan?";
		qbank[56][1] = "Liyod Gorge";	qbank[56][2] = "Lutyens Sir Edwin";
		qbank[56][3] = "G. Lawrence";	qbank[56][4] = "Viswontari Das Ghos";
	qans[56][0] = 'B';
	qans[56][1] = 'C';

	qbank[57][0] = "When was the first amendment to the constitutaion adopted?";
		qbank[57][1] = "1951";	qbank[57][2] = "1949";
		qbank[57][3] = "1953";	qbank[57][4] = "1964";
	qans[57][0] = 'A';
	qans[57][1] = 'D';

	qbank[58][0] = "Who is the \'ex-officio' chairman of Rajyasabha?";
		qbank[58][1] = "Speaker";			qbank[58][2] = "Chief Member of R.S.";
		qbank[58][3] = "Vice President";	qbank[58][4] = "Prime Minister";
	qans[58][0] = 'C';
	qans[58][1] = 'B';

	qbank[59][0] = "Who was the Indian's chief of army staff during the 1977 Indo-Pak war?";
		qbank[59][1] = "Gen. T.V. Raina";			qbank[59][2] = "Gen. J.N. Chaudhary";
		qbank[59][3] = "Gen. S.H.F.J. Manekshow";	qbank[59][4] = "G.P.D. Kumaramangalam";
	qans[59][0] = 'C';
	qans[59][1] = 'A';

// 13. ***********************************************************
	qbank[60][0] = "IBM is known as... in America?";
		qbank[60][1] = "Sys Tycoon";	qbank[60][2] = "Megasoft";
		qbank[60][3] = "Big Blues";	qbank[60][4] = "Not given";
	qans[60][0] = 'C';
	qans[60][1] = 'D';

	qbank[61][0] = "Dr. B.R. Ambedkar was elected in constitution committe from...?";
		qbank[61][1] = "West Bengal";					qbank[61][2] = "Bombay Presidency";
		qbank[61][3] = "Co-Current Middle India";	qbank[61][4] = "Panjab";
	qans[61][0] = 'B';
	qans[61][1] = 'C';

	qbank[62][0] = "Which of the following is India's biggest source of revenue?";
		qbank[62][1] = "Land Revenue";	qbank[62][2] = "Income Tax";
		qbank[62][3] = "Wealth Tax";		qbank[62][4] = "Excise Duty";
	qans[62][0] = 'D';
	qans[62][1] = 'B';

	qbank[63][0] = "According to hindu mythology Lord Krishna, incarnated in which Dwapar?";
		qbank[63][1] = "1";		qbank[63][2] = "12";
		qbank[63][3] = "27";		qbank[63][4] = "Each";
	qans[63][0] = 'C';
	qans[63][1] = 'D';

	qbank[64][0] = "Which of the following British Prime Minister told Mahatma Gandhi \'The Begger of India'?";
		qbank[64][1] = "Ramsse McDonald";	qbank[64][2] = "Nevil Chamberlane";
		qbank[64][3] = "Wilson Auishle";		qbank[64][4] = "Whinston Chirchil";
	qans[64][0] = 'D';
	qans[64][1] = 'B';
// 14. ***********************************************************

	qbank[65][0] = "Which party was in power in Britain when India achieved Independence?";
		qbank[65][1] = "The conservative Party";	qbank[65][2] = "The Liberal Party";
		qbank[65][3] = "The Labour Party";			qbank[65][4] = "A Coalition of all above";
	qans[65][0] = 'C';
	qans[65][1] = 'B';

	qbank[66][0] = "Which of the following celebrated 125th anniversary in 1978?";
		qbank[66][1] = "Postoffice & Telegraph";	qbank[66][2] = "Indian Railway";
		qbank[66][3] = "Tata Iron & Steel";			qbank[66][4] = "None of these";
	qans[66][0] = 'B';
	qans[66][1] = 'D';

	qbank[67][0] = "Who is called \'The man of blood and iron'?";
		qbank[67][1] = "Hitler";	qbank[67][2] = "Goebbles";
		qbank[67][3] = "Bismark";	qbank[67][4] = "Hindenburg";
	qans[67][0] = 'C';
	qans[67][1] = 'A';

	qbank[68][0] = "Bhootlingam Committe was concered with national...?";
		qbank[68][1] = "Wage Policy";		qbank[68][2] = "Textile Policy";
		qbank[68][3] = "Sugar Policy";	qbank[68][4] = "Export Policy";
	qans[68][0] = 'A';
	qans[68][1] = 'C';

	qbank[69][0] = "Where did central intelligece agency plant plutonium powered SPY device in 1980?";
		qbank[69][1] = "Kanchanchunga";	qbank[69][2] = "Annapurna";
		qbank[69][3] = "Nanda Devi";		qbank[69][4] = "Amarnath";
	qans[69][0] = 'C';
	qans[69][1] = 'B';
// 15. ***********************************************************

	qbank[70][0] = "Taj Mahal was built by the experts of India and...?";
		qbank[70][1] = "Egypt";		qbank[70][2] = "Iran";
		qbank[70][3] = "Portugal";	qbank[70][4] = "Persia";
	qans[70][0] = 'D';
	qans[70][1] = 'A';

	qbank[71][0] = "After withdrerw in November 1977 United States of America decided to rejoin the International labour organistaion in ...?";
		qbank[71][1] = "Feb' 1980";	qbank[71][2] = "Jan' 1982";
		qbank[71][3] = "Dec' 1977";	qbank[71][4] = "Never";
	qans[71][0] = 'A';
	qans[71][1] = 'C';

	qbank[72][0] = "Who was the viceroy of India when the British capital was shifted from Calcutta to Delhi in 1911?";
		qbank[72][1] = "Lord Hardinge";	qbank[72][2] = "Lord Cornawallis";
		qbank[72][3] = "Lord Curzon";		qbank[72][4] = "Lord William";
	qans[72][0] = 'A';
	qans[72][1] = 'B';

	qbank[73][0] = "Which of the following does not about the Abrahim Lincon?";
		qbank[73][1] = "16th President of USA";			qbank[73][2] = "Abolished Salavery in USA";
		qbank[73][3] = "Believed in defence pact";		qbank[73][4] = "Assissinated by J. Wilkes";
	qans[73][0] = 'C';
	qans[73][1] = 'D';

	qbank[74][0] = "Which of the following was the vice-chancellor of nalanda university?";
		qbank[74][1] = "Nagarg Uha";		qbank[74][2] = "Sushruta";
		qbank[74][3] = "Varahamihira";	qbank[74][4] = "None of these";
	qans[74][0] = 'A';
	qans[74][1] = 'B';
}
/* ************************* End of Question Bank******************* */
