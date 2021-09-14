#include<fstream.h>
#include<iostream.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<stdio.h>
#include<ctype.h>
#include<dos.h>
#include<graphics.h>
#include<time.h>

int count;
int no_of_times[26]={9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1};
int alpha[26]={1, 3, 3, 2, 1, 4, 2, 2, 1, 8, 5, 4, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
void display_ins(void);
struct player
{
  struct date dt_play;
  int score;
  char name[40];
  char letters[7];
};

class GAME
{
     char mat[15][15],word[14],word_temp[14];
     int coordinate[2];
  public:
     GAME();
     player play[2];
     void letterinit(int);       //initialises letters
     void letterallotment(int);  //allots random letters to user
     int letterchecker(int);     //checks whether input word contains the given letters or not
     void result_display(void);  //displays the result after every round
     int exit_check(void);  //checks if all letters have been used
     void display_board(int);  //for displaying board
     void get_playersinput(int);   //inputs players inputs, checks for all conditions and outputs word
     void update_score(int);       //updates the score of the user after every turn
     int word_checker(void); //checks if word exists in dictionary or not
     int check_cross(int,int,char); //checks whether word of user matches and crosses already put up  words
     int check_used(void);        //checks if user has not entered a previously entered word
     void copy(char[][15],char[][15]);
     void append_player();
};
void GAME::append_player()
{
      fstream f("c:\\turboc3\\cs_prj\\score.dat",ios::app);
      f.write((char*)&play[0], sizeof(play[0]));
      f.write((char*)&play[1], sizeof(play[1]));
      f.close();

}

GAME::GAME()
{
	ofstream fout1("c:\\turboc3\\cs_prj\\words.txt");
	fout1.close();
	count=0;
	play[0].score=0;
	play[1].score=0;
	getdate(&play[0].dt_play);
	getdate(&play[1].dt_play);
	int i,j;
	for(i=0;i<15;i++)
	{
	   for(j=0;j<15;j++)
	   {
	       mat[i][j]=' ';
	    }
	 }

	 char temp_arr[5] = {'A', 'A', 'I', 'O', 'U'};
	 mat[7][7] = temp_arr[rand()%5];
}

void main()
{
    GAME all;
    int gd=DETECT, gm;
    initgraph (&gd,&gm,"C:\\TURBOC3\\BGI");
    cleardevice();
    setbkcolor(LIGHTGREEN);
    setcolor(BLUE);
    display_ins();    //Display of Instructions
    all.letterinit(1);
    all.letterinit(0);
    getch();
    char ch;
    cleardevice();
    setbkcolor(LIGHTGREEN);
    setcolor(BLUE);
    outtextxy(0,10, "_______________________________::SCRABBLE FOR TWO::_____________________________");
    gotoxy(25,8);
    cout<<"Enter player name to start"<<endl;
    for(int i=0;i<2;i++)
    {
	gotoxy(25,10+2*i);
	setcolor(BLUE);
	cout<<"Player "<<i+1<<" : ";
	cin.getline(all.play[i].name,40,'\n');
	fflush(stdin);
    }
    do
    {
       for(int i=0;i<2;i++)
	{
	    all.letterallotment(i);
	    all.display_board(i);
	    ch=getch();
	    if(ch==27)  //escape key
	      break;
	    fflush(stdin);
	    all.get_playersinput(i);
	}
	count++;
	all.result_display();
    }while(all.exit_check() && ch!=27);
  if(all.exit_check()==0)
	all.append_player();
}
void GAME::display_board(int player_num)
{
   cleardevice();
   setbkcolor(LIGHTGREEN);
   setcolor(BLUE);
   outtextxy(500,20,"SCRABBLE FOR TWO");
   rectangle(20,20,470,470);
   for(int i = 50; i<= 450; i+=30)
       line(i,20,i,470);
   for(i = 50; i<= 450; i+=30)
       line(20,i,470,i);
   char k[3];
   int l=1;
   for(i=35;i<470;i+=30)
   {
	k[0]=(l/10)+48;
	k[1]=(l%10)+48;
	k[2]='\0';
	outtextxy(i,10,k);
	l++;
   }
   l=1;
   for(i=35;i<470;i+=30)
   {    k[0]=(l/10)+48;
	k[1]=(l%10)+48;
	k[2]='\0';
	outtextxy(4,i,k);
	l++;
   }
   for(i =1;i<=15;i++)
   {
      for(int j=1;j<=15;j++)
      {
	 if(mat[i-1][j-1]!=' ')
	 {
	    char ot[2];
	    int gx,gy;
	    gx = j*30+5;
	    gy = i*30+5;
	    ot[0]=mat[i-1][j-1];
	    ot[1]='\0';
	    outtextxy(gx,gy,ot);
	 }
      }
   }
      char temp[2];
      temp[0]=player_num+49;
      temp[1]='\0';
      outtextxy(500,20,"SCRABBLE FOR TWO");
      outtextxy(500,30,"Player");
      outtextxy(550,30,temp);
      for(int d=0;d<7;d++)
      {  char tempo[2];
	 tempo[0]=play[player_num].letters[d];
	 tempo[1]='\0';
	 outtextxy(500+15*d,40,tempo);
      }


}



void GAME::get_playersinput(int play_num)
{
	char c;
	char direction;
	int j=0;
	setcolor(BLUE);
	outtextxy(500,72,"Coordinate X:");
	gotoxy(70,6);
	cin>>coordinate[0];
	outtextxy(500,102,"Coordinate Y: ");
	gotoxy(70,8);
	cin>>coordinate[1];
	if((coordinate[0]==-1)||(coordinate[1]==-1)) //skip a chance
	{
		for(int to=0;to<7;to++)
		{
		       no_of_times[play[play_num].letters[to]-'A']++;
		}
		letterinit(play_num);
		return;
	}
	outtextxy(500,132,"Direction (A/D):");
	gotoxy(70,10);
	cin>>direction;
	direction = toupper(direction);
	outtextxy(540,162,"WORD : ");
	setcolor(BLACK);
	int y=64;
	do
	{
	    word[j] = getch();
	    word[j]=toupper(word[j]);
	    gotoxy(y,12);
	    setcolor(BLACK);
	    y+=3;
	    cout<<word[j];
	    j++;
	}while(word[j-1]!='\r');

	word[j-1] = '\0';
	fflush(stdin);
	strcpy(word_temp,word);
	int lettr_check=0, cross_check=0, word_check=0, used_checker=0;
	word_check=word_checker();
	used_checker=check_used();
	if(word_check==1 && used_checker==1)
	{
		cross_check=check_cross(coordinate[0],coordinate[1],direction);
		if(cross_check == 1)
			lettr_check=letterchecker(play_num);
	}
	gotoxy(61,15);
	cout<<"Word check : "<<word_check<<"\n";
	gotoxy(61,16);
	cout<<"Used check : "<<used_checker<<"\n";
	gotoxy(61,17);
	cout<<"Cross check : "<<cross_check<<"\n";
	gotoxy(61,18);
	cout<<"Letter check : "<<lettr_check<<"\n";
	getch();
	if((word_check==1) && (used_checker==1) && (cross_check==1) && (lettr_check==1))
	{
	      int len= strlen(word);
	      update_score(play_num);
	      fstream fout("c:\\turboc3\\cs_prj\\words.txt",ios::app); //storing the word for check used function
	      fout<<word_temp<<"\n";
	      fout.close();

	      switch(direction)
	      {
		    case 'D':
			    for(int sc=0;sc<len;sc++)
			    {
				if(word[sc] == ' ')
					continue;
			       mat[coordinate[1]-1+sc][coordinate[0]-1]=word[sc];
			    }
			    break;


		    case 'A':
			    for(int i=0;i<len;i++)
			    {
				if(word[i] == ' ')
					continue;
				mat[coordinate[1]-1][coordinate[0]-1+i]=word[i];
			    }
			    break;
	      }

	}
	else
	{
	    display_board(play_num);
	    get_playersinput(play_num);
	}
	return;

}

void GAME::letterinit(int play_num)
{
	for(int i=0;i<7;i++)
	 play[play_num].letters[i]=' ';

}
void GAME::letterallotment(int play_num)
{
	int i,k,x;
	srand(time(0));
	for(i=0;i<7;i++)
    {
	if(play[play_num].letters[i]==' ')
       { 	x=rand()%26;
	     if(no_of_times[x]!=0)
	      {
		  play[play_num].letters[i]=x+65;
		  no_of_times[x]--;
	      }
	      else
	      i--;

       }
    }
}
int GAME::letterchecker(int play_num)
{
	int i,k,lflag,p=0;
	int len=strlen(word);
	char temp_letter[7];
	for(int t=0;t<7;t++)
	{
	      temp_letter[t] =	play[play_num].letters[t];
	}

	lflag=0;
	for(i=0;i<len;i++)
	{
		if(word[i] == ' ')
		{
		    p++;
		    continue;
		}

		for(k=0;k<7;k++)
		{	if(word[i]==temp_letter[k])
		       {
				p++;
				temp_letter[k] = ' ';
				break;
		       }
		}
	}

	if(p==len)
	{

		for(i=0;i<len;i++)
		{
			if(word[i] == ' ')
			{
				continue;
			}

			for(k=0;k<7;k++)
			{
				if(word[i]==play[play_num].letters[k])
				{
					play[play_num].letters[k]=' ';
					break;
				}
			}
		}
	  lflag=1;
	  letterallotment(play_num);
	}
 return lflag;
}
int GAME::check_cross(int XCord, int YCord,char direction)
{
	XCord-=1;
	YCord-=1;
	int leng=strlen(word);
	int flag=0;

	 int k=0;
	  char temp[15][15];
	  flag=0;
	  int flag1=0,flag2=0;

	 switch(direction)
	 {
		case 'A':  for(k=0;k<leng;k++)
			   {
				if(mat[YCord][XCord] == ' ')
				{
				      flag1=1;
				}
				else if(mat[YCord][XCord] == word[k])
				{
				      word[k] = ' '; //so that word gets cleared in letter check afterwards
				      flag2=1;
				}
				else
				{
					flag2=0;
					flag=0;
					break;
				}
				XCord++;
			   }


			   if(flag1==1 && flag2==1)
			   {
				     flag=1;

			   }
			   break;

		case 'D':
			 for(k=0;k<leng;k++)
			 {
			      if(mat[YCord][XCord] == ' ')
			      {
				 flag1=1;
			      }
			      else if(mat[YCord][XCord] == word[k])
			      {
				  word[k] = ' ';
				  flag2=1;
			      }
			      else
			      {
				flag2=0;
				 flag=0;
				 break;
			      }
			      YCord++;
			 }

			if(flag1==1 && flag2==1)
			{
				flag=1;
			}
		}
	   return flag;

}
void GAME:: copy(char a[][15],char b[][15])
{
	for(int i=0;i<15;i++)
	{
		for(int j=0;j<15;j++)
		{
			b[i][j]=a[i][j];
		}
	}
}
int GAME::word_checker(void)
{
    char check[14];
    int flag=0;
    ifstream finp("c:\\turboc3\\cs_prj\\dict.txt");
    while(!finp.eof())
    {
	    finp.getline(check,14,'\n');
	    if(!strcmp(word,check))
	    {   flag=1;
		break;
	    }

     }
     finp.close();
     return flag;
}
int GAME::check_used(void)
{
    char check[14];
    int flag=0;
    char s;
    ifstream fin("c:\\turboc3\\cs_prj\\words.txt");
    fin.seekg(0, ios::end);
    if(!fin.tellg())
    {
	return 1;
    }
    fin.seekg(0);
    while(!fin.eof())
    {
	    fin.getline(check,14,'\n');
	    if(strcmp(word,check))
	    {
		flag=1;
		break;
	    }

     }
     fin.close();
     return flag;
}
void GAME ::result_display(void)
  {
	char tem[2];
	tem[0]=count+48;
	tem[1]='\0';
	outtextxy(480,350,"Result after round");
	outtextxy(480,365,tem);
	tem[0]=play[0].score+48;
	outtextxy(480,385,tem);
	outtextxy(500,385, "points of ");
	outtextxy(480,395,play[0].name);
	tem[0]=play[1].score+48;
	outtextxy(480,415,tem);
	outtextxy(500,415, "points of ");
	outtextxy(480,425,play[1].name);
	getch();
     }
void display_ins(void)
{
	setbkcolor(LIGHTGREEN);
	setcolor(BLUE);
	cout<<"_________________________Instructions for scrabble game:________________________ "<<endl;
	cout<<"     1.Words have to be inserted starting from Centre in the first turn"<<endl;
	cout<<"     2.After first turn, New words have to be put which are passing through the       previously given words"<<endl;
	cout<<"     3.If the words do not match, coordinates will have to be reentered and          valid word has to be provided"<<endl;
	cout<<"     4.Words entered will be checked whether there could be valid word in the         dictionary."<<endl;
	cout<<"     5.Scores will be displayed after each round, press ESC key before entering       coordinates to EXIT"<<endl;
	cout<<"     6.If player wants to skip, kindly enter coordinate as -1 in any place "<<endl;
	cout<<"     7.There will be four checks for aceepting a word as valid- dictionary            check, word used check, word cross check and letters check, one needs to          pass all to print a word on the board"<<endl;
	cout<<"     8.The Scores for Letters used are: "<<endl;
	char alp='A';
	for(int i=0;i<26;i++)
	{
		  cout<<alp++<<" - ";
		 cout<<alpha[i]<<" \t";
	}
       cout<<endl;
}
int GAME::exit_check(void)
{
    int c=0;
    for(int i=0;i<26;i++)
    {
       c+=no_of_times[i];

    }
    return c;
}
void GAME ::update_score(int play_num)
{


	for(int i=0;i<strlen(word_temp);i++)
	{
	    play[play_num].score=play[play_num].score + alpha[word_temp[i]-'A'];

	}
}