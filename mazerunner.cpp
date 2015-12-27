#include<iostream.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<stdio.h>
#include<graphics.h>

#define MAX 20

void setPage();
void startPage();
void loading();
void drawLine();

void initGame();
void showTem();
void rowWall(int r);
void colWall(int c);
void rowRan(int r,int n);
void colRan(int c,int n);
void bricksRan(int n);
int check(int,int);
void copyTem();
void push(int x,int y);
int get(int);
void show();
int gameOver();
struct location mark(struct location c,int,int);
struct location track(struct location tem1,struct location cur);

int maxx,maxy,midx,midy;
char *msg;

int row,col,top=-1,score=10000,reducer=1,highscore;
int mat[MAX][MAX],tem[MAX][MAX];
int dir[8][2]={-1,0,0,1,1,0,0,-1,-1,-1,-1,1,1,1,1,-1};

struct location
{
	int x,y,v;
}pre,next,stack[200];

void main()
{
	struct location tem,cur;
	FILE *fp,*fp1;
	clrscr();
	int gd=DETECT,gm;
	int xx,x,yy,x1,x2,y1,y2;

	initgraph(&gd,&gm,"..\\bgi");
	maxx=getmaxx();
	maxy=getmaxy();
	midx=maxx/2;
	midy=maxy/2;
	startPage();

	loading();
	fp=fopen("high.txt","r");
	fscanf(fp,"%d",&highscore);
	fp1=fopen("high.txt","w");
	initGame();
	cur.x=row/2;
	cur.y=col/2;
	cur.v=2;
	mat[cur.x][cur.y]=cur.v;
	while(!gameOver())
	{
		show();
		do
		{
			settextstyle(3,0,1);
			setcolor(RED);
			outtextxy((x=50),(y1=400),"ENTER  ROW  AND  COLUMN  : (   ,   ) ");
			x1=x+textwidth("ENTER  ROW  AND  COLUMN  : ( ")-25;
			x2=x+textwidth("ENTER  ROW  AND  COLUMN  : (   , ")-30;
			xx=get(row+2)-1;
			setcolor(CYAN);
			sprintf(msg,"%d",xx+1);
			outtextxy(x1,y1,msg);
			yy=get(col+2)-1;
			sprintf(msg,"%d",yy+1);
			outtextxy(x2,y1,msg);
			if(mat[xx][yy]==0&&!(check(xx,yy)))
				break;
			show();
			setcolor(RED);
			outtextxy(x,430,"INVALID LOCATION CHOSEN !");
		}while(1);
		mat[xx][yy]=1;
		score-=reducer*reducer;
		reducer++;
		show();
		tem=mark(cur,0,8);
		next=track(tem,cur);
		mat[cur.x][cur.y]=0;
		mat[next.x][next.y]=2;
		if(next.x==0)
		{
			cleardevice();
			setPage();
			settextstyle(4,0,5);
			setcolor(14);
			outtextxy(midx-textwidth("YOU WON THE GAME !")/2,180,"YOU WON THE GAME !");
			if(score>highscore)
			{
				sprintf(msg,"NEW HIGH SCORE : %d",score);
				setcolor(RED);
				outtextxy(midx-textwidth("NEW HIGH SCORE : 1000")/2,220,msg);
				fprintf(fp1,"%d",score);
			}
			else
			{
				sprintf(msg,"SCORE : %d",score);
				setcolor(RED);
				outtextxy(midx-textwidth("SCORE : 1000")/2,220,msg);
				fprintf(fp1,"%d",highscore);
			}
			getch();
			fclose(fp);
			fclose(fp1);
			return;
		}
		cur=next;
		cur.v=2;
	}
	show();
	cleardevice();
	setPage();
	settextstyle(4,0,5);
	setcolor(14);
	outtextxy(midx-textwidth("YOU LOSE THE GAME !")/2,180,"YOU LOSE THE GAME !");
	score=10000-score;
	sprintf(msg,"SCORE : %d",score);
	setcolor(RED);
	if(highscore<score)
		fprintf(fp1,"%d",score) ;
	else
		fprintf(fp1,"%d",highscore);
	outtextxy(midx-textwidth("SCORE : 1000")/2,220,msg);
	getch();
	fclose(fp);
	fclose(fp1);
}
void push(int x,int y)
{
	if(top>=200)
	{
		printf("\nStack Overflow !");
		exit(1);
	}
	top++;
	stack[top].x=x;
	stack[top].y=y;
	stack[top].v=-1;
}
void rowWall(int r)
{
	int i;
	for(i=0;i<col+2;i++)
		mat[r][i]=1;
}
void colWall(int c)
{
	int i;
	for(i=0;i<row+2;i++)
		mat[i][c]=1;
}
void copyTem()
{
	int i,j;
	for(i=0;i<row+2;i++)
		for(j=0;j<col+2;j++)
			tem[i][j]=mat[i][j];
}
void rowRan(int r,int n)
{
	int i,ran;
	randomize();
	for(i=0;i<n;i++)
	{
		ran=random(100)%(row)+1;
		if(!mat[r][ran])
		{
			push(r,ran);
			mat[r][ran]=-1;
		}
		else
			i--;
	}
}
void colRan(int c,int n)
{
	int i,ran;
	randomize();
	for(i=0;i<n;i++)
	{
		ran=random(100)%(col)+1;
		if(!mat[ran][c])
		{
			push(ran,c);
			mat[ran][c]=-1;
		}
		else
			i--;
	}
}
int check(int x,int y)
{
	int i;
	for(i=0;i<=top;i++)
		if(x==stack[i].x&&y==stack[i].y)
			return 1;
	return 0;
}
int gameOver()
{
	int i;
	for(i=0;i<=top;i++)
		if(mat[stack[i].x][stack[i].y]!=-1)
			return 1;
	return 0;
}
void bricksRan(int n)
{
	int i,j,r,c,m;
	randomize();
	for(i=0;i<n;i++)
	{
		r=random(100)%row+1;
		c=random(100)%col+1;
		if(mat[r][c]!=0)
			i--;
		else
			mat[r][c]=1;
	}
}
void initGame()
{
	int n;
	cleardevice();
	setPage();
	settextstyle(4,0,4);
	setcolor(4);
	outtextxy(midx-textwidth("YOUR SPECIFICATION")/2,50,"YOUR SPECIFICATION");
	settextstyle(3,0,2);
	setcolor(5);
	outtextxy(40,120,"ENTER NUMBER OF ROWS ");
	outtextxy(midx+70,120,":");
	row=get(MAX);
	setcolor(3);sprintf(msg,"%d",row);
	outtextxy(midx+90,120,msg);
	setcolor(5);
	outtextxy(40,190,"ENTER NUMBER OF COLUMNS ");
	outtextxy(midx+70,190,":");
	col=get(MAX);
	setcolor(3);
	sprintf(msg,"%d",col);
	outtextxy(midx+90,190,msg);
	for(int i=0;i<15;i++)
	{
		settextstyle(4,0,4);
		sprintf(msg,"LETS PLAY !!!");
		outtextxy((maxx/2)-(textwidth("LETS PLAY !!!")/2),350,msg);
		delay(100);
	}
	rowWall(0);
	rowWall(row+1);
	colWall(0);
	colWall(col+1);
	rowRan(1,row/3);
	rowRan(row,row/3);
	bricksRan((row+col)*3/2);
	colRan(1,col/3);
	colRan(col,col/3);
}
struct location mark(struct location c,int done,int n)
{
	int rear=-1,front=0,i,j,val;
	struct location que[300],tem1,max,min;
	copyTem();
	rear++;
	que[rear]=c;
	max=c;
	while(front<=rear)
	{
		tem1=que[front];
		front++;
		val=tem1.v;
		for(i=0;i<n;i++)
		{
			if(tem[tem1.x+dir[i][0]][tem1.y+dir[i][1]]==0||tem[tem1.x+dir[i][0]][tem1.y+dir[i][1]]==-1)
			{
				rear++;
				que[rear].x=tem1.x+dir[i][0];
				que[rear].y=tem1.y+dir[i][1];
				que[rear].v=val+1;
				tem[que[rear].x][que[rear].y]=val+1;
				if(max.v<val+1)
					max=que[rear];
			}
		}
	}
	min.v=999;
	for(i=0;i<=top;i++)
	{
		if(tem[stack[i].x][stack[i].y]>0)
		{
			if(min.v>tem[stack[i].x][stack[i].y])
			{
				min=stack[i];
				min.v=tem[stack[i].x][stack[i].y];
			}
		}
	}
	if(min.v==999)
		return max;
	return min;
}
struct location track(struct location tem1,struct location cur)
{
	int t=-1,i;
	struct location c,a=tem1,s[50];
	t++;
	s[t]=tem1;
	while(s[t].x!=cur.x||s[t].y!=cur.y)
	{
		c=s[t];
		for(i=0;i<8;i++)
		{
			if(tem[c.x+dir[i][0]][c.y+dir[i][1]]!=1&&tem[c.x+dir[i][0]][c.y+dir[i][1]]<tem[c.x][c.y])
			{
				t++;
				s[t].x=c.x+dir[i][0];
				s[t].y=c.y+dir[i][1];;
				s[t].v=tem[c.x+dir[i][0]][c.y+dir[i][1]];
				break;
			}
		}
	}
	if(t==0)
	{
		s[t].x=0;
		return s[t];
	}
	return s[t-1];
}
void setPage()
{
	setbkcolor(BLACK);
	setcolor(WHITE);
	setlinestyle(0,0,1);
	rectangle(0,0,maxx,maxy);
	rectangle(3,3,maxx-3,maxy-3);
}
void loading()
{
	cleardevice();
	setPage();
	setcolor(13);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,0);
	setusercharsize(2, 1, 1, 1);
	outtextxy(138,198,"LOADING");
	setcolor(13);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,0);
	setusercharsize(2, 1, 1, 1);
	setcolor(BLUE);
	rectangle(119,199,421,231);
	rectangle(118,198,422,232);
	setcolor(10);
	for(int ii=120;ii<=420;ii++)
	{
		line(ii,200,ii,230);
		sound(ii+1500);
		delay(10);
		nosound();
	}
}
void startPage()
{
	setPage();
	setcolor(4);
	settextstyle(4,0,6);
	outtextxy(midx-textwidth("THE MAZE RUNNER !")/2,midy-170,"THE MAZE RUNNER !");
	delay(500);
	setPage();
	settextstyle(3,0,3);
	outtextxy(midx-textwidth("DEVELOPED BY")/2,midy-100,"DEVELOPED BY");
	delay(400);
	setlinestyle(1,1,2);
	rectangle(midx-220,midy-40,midx+220,midy+90);
	for(int p=0;p<15;p++)
	{
		setcolor(p);
		settextstyle(4,0,8);
		outtextxy(midx-textwidth("MANISH")/2,midy-30,"MANISH");delay(200);
		sound(350);
	}
	nosound();
	getch();
}
void show()
{
	clrscr();
	cleardevice();
	setPage();
	int x1,x2,y1,y2,box,xpad,ypad,px1,py1,px2,py2,pt;
	box=22;
	xpad=8;
	ypad=5;
	px1=x1=midx-((row+2)*box+(row+2)*xpad)/2;
	py1=y1=midy-((col+2)*box+(col+2)*ypad)/2-(maxy-400)/2;
	px2=x2=x1+box;
	py2=y2=y1+box;
	py1-=box-15;
	pt=(px1+px2)/2;
	settextstyle(1,0,1);
	setcolor(13);
	outtextxy(5,12,"HIGH SCORE");
	setcolor(YELLOW);
	sprintf(msg,"%d",highscore);
	outtextxy(50,30,msg);
	settextstyle(1,0,1);
	setcolor(13);
	outtextxy(maxx-130,12,"YOUR SCORE");
	setcolor(YELLOW);
	sprintf(msg,"%d",score);
	outtextxy(maxx-100,30,msg);
	for(int i=0;i<row+2;i++)
	{
		settextstyle(0,0,1);
		sprintf(msg,"%d",i+1);
		pt-=textwidth(msg)/2;
		outtextxy(pt+3,py1-5,msg);
		pt+=box+xpad+xpad/2;
	}
	pt=(py1+py2)/2;
	px1-=box-10;
	for(i=0;i<col+2;i++)
	{
		settextstyle(0,0,1);
		sprintf(msg,"%d",i+1);
		pt-=textheight(msg)/2;
		outtextxy(px1-5,pt+3,msg);
		pt+=box+2*ypad;
	}
	for(i=0;i<row+2;i++)
	{
		x1=midx-((row+2)*box+(row+2)*xpad)/2;
		x2=x1+box;
		for(int j=0;j<col+2;j++)
		{
			if(mat[i][j]==1)
			   setfillstyle(1,RED);
			else if(mat[i][j]==2)
			   setfillstyle(1,GREEN);
			else if(mat[i][j]==-1)
			   setfillstyle(1,BLUE);
			else if(mat[i][j]==0)
			   setfillstyle(1,WHITE);
			bar3d(x1,y1,x2,y2,(x2-x1)/8,0);
			x1+=box+xpad;
			x2=x1+box;
		}
		y1+=box+ypad;
		y2=y1+box;
	}
	delay(1000);
}
int get(int range)
{
	int tem1,tem,rescl,i=0;
	char *a;
	while(i<3)
	{
		a[i]=getche();
		i++;
		if(a[i-1]==13||a[i-1]==32)
			break;
	}
	a[i]=NULL;
	tem1=atoi(a);
	if(tem1>0&&tem1<=range)
	{
		return tem1;
	}
	else
	{
		sprintf(msg,"INPUT ERROR !!!");
		outtextxy((maxx/2)-(textwidth("INPUT ERROR !!!")/2),maxy-100,msg);
		delay(500);
		rescl=getbkcolor();
		setcolor(2);
		setcolor(rescl);outtextxy((maxx/2)-(textwidth("INPUT ERROR !!!")/2),maxy-100,msg);
		setcolor(15);
		sprintf(msg,"GIVE INPUT IN BETWEEN 0 AND %d",range+1);
		outtextxy((maxx/2)-(textwidth(msg)/2),maxy-100,msg);
		delay(500);
		rescl=getbkcolor();
		setcolor(2);
		setcolor(rescl);outtextxy((maxx/2)-(textwidth(msg)/2),maxy-100,msg);
		setcolor(GREEN);
		tem1=get(range);
	}
	return tem1;
}
