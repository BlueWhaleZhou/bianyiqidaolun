/*----------------------------------------------------------------------------------------------
Version 1.0
1. Read NT and T correct.
Version 1.1
1. Read Grammar but need test
2. Structalization in grammar but not in symbol
Version 1.2
1. Symbol reading into tempstr problem
2. nt and t problem
Version 1.3
1. Correct previous problems
2. Symbol and grammar reading correct, error check correct
3. Need finish first and follow functions, and set operation
Version 1.4
1. Finished the first(), also added and the set union operation
Version 1.5
1. Finished follow()
2. Sort the terminal list
3. Print the final result
Version 1.6
1. Replace strcpy_s with strcpy, because gcc can't compile strcpy_s
Version 1.7
1. Add: A -> B c, if B contains epsilon, then add c to A's first set
2. Correct a bug in the epsilon of the first set
Version 1.8
1. Correct the firstset computation logic
2. No terminals is no longer recognized as error.
3. Correct the logic error in epsilon passage
----------------------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SYMNUM 100	//Symbol name<=100
#define MAXSTR 10000	//1st + 2nd section<=10000; grammar <=10000
#define RIGHT 20		//num of right hand side symbol<=20

struct gramnode{
	char left[SYMNUM];
	char** right;
	int rightnum;
};

void main();
void getSymbol();
void getGrammar();
int checkSpace(char);
int checkID(char, int*);
int checkArrow(char);
int checkNT(char*, char);
int checkT(char*);
void first();
void ntFirst();
void getFirst(int);
void follow();
int setUnion(int*, int*, int);
void output();
void sortTerminal();

char* error[]={"ERROR CODE 0","ERROR CODE 1","ERROR CODE 2","ERROR CODE 3"};
char start[SYMNUM];
char tempstr[MAXSTR];
char** nonterminal;
char** terminal;
int* leftnt; 
int leftntcount;
struct gramnode* grammar;
int ntnum=0, tnum=0, gramnum=0;
int err[4]={0, 0, 0, 0};
int** tFirstlist;
int** ntFirstlist;
int** tFollowlist;
int** ntFollowlist;

void main(){
	int i, flag=0;
	getSymbol();
	if(err[0]==1){
		printf ("%s\n", error[0]);
		return;
	}
	getGrammar();

	if(err[0]==1){
		printf ("%s\n", error[0]);
		return;
	}
	for (i=1;i<=3;i++){
		if(err[i]==1){
			flag=1;
			printf ("%s\n", error[i]);
		}
	}
	if(flag==1)
		return;
	first();
	follow();
	output();
}

//--------------------------------------------------------read NT and T-----------------------------------------------
void getSymbol(){
	int i=-1, j=0, k=0, hashnum=0;
	char c;
 
	c=getchar();
	if(checkSpace(c))		// clear the spaces after the 2nd section
		c=getchar();
	while(1){
	//*****************check ID*****************
		if(checkID(c, &i)){
			c=getchar();
			if(hashnum==0)
				ntnum++;
			else
				tnum++;
		}
	//*****************check #*****************
		else if(c=='#'){
			if((hashnum==0 && ntnum==0)/*||(hashnum==1 && tnum==0)*/){
				err[0]=1;
				return;
			}
			hashnum++;	
			if(hashnum==2)
				break;
			c=getchar();
		}
		else{
			err[0]=1;
			return;
		}
	//*****************check space*****************
		if(checkSpace(c)){	
			if(hashnum==1 && tnum==0){}
			else{
				i++;
				tempstr[i]=' ';
			}
			c=getchar();
		}
		else{
			err[0]=1;
			return;
		}
	}
	//*****************put symbols into corresponding strings*****************
	nonterminal=(char**)malloc(sizeof(char*)*ntnum);
	terminal=(char**)malloc(sizeof(char*)*tnum);
	leftnt=(int*)malloc(sizeof(int)*ntnum);
	leftntcount=ntnum;

	for(i=0; i<ntnum;i++){					// i=space/ID number, j=char number in ID, k=char number in tempstr
		nonterminal[i]=(char*)malloc(sizeof(char)*SYMNUM);
		leftnt[i]=0;
		for(j=0; j<SYMNUM && tempstr[k]!=' '; j++, k++){
			nonterminal[i][j]=tempstr[k];
		}
		if(j<SYMNUM)
			nonterminal[i][j]='\0';
		k++;	
	}
	strcpy(start, nonterminal[0]);
	
	for(i=0; i<tnum;i++){					// i=space/ID number, j=char number in ID, k=char number in tempstr
		terminal[i]=(char*)malloc(sizeof(char)*SYMNUM);
		for(j=0; j<SYMNUM && tempstr[k]!=' '; j++, k++){
			terminal[i][j]=tempstr[k];
		}
		if(j<SYMNUM)
			terminal[i][j]='\0';
		k++;	
	}
	//*****************sortTerminal*****************
	sortTerminal();
	return;
}

//--------------------------------------------------------read grammar-----------------------------------------------
void getGrammar(){
	int i=-1, j=0, k=0, m=0;
	char c;

	memset(tempstr, 0, sizeof(tempstr));
	c=getchar();
	if(checkSpace(c))		// clear the spaces after the 2nd section
		c=getchar();
	while(1){
	//*****************check left ID*****************
		if(checkID(c, &i)){ 
			c=getchar();
		}
		else{
			err[0]=1;
			return;
		}
	//*****************check space*****************
		if(checkSpace(c)){	
			i++;
			tempstr[i]=' ';
			c=getchar();
		}
		else{
			err[0]=1;
			return;
		}
	//*****************check ->*****************
		if(checkArrow(c)){
			i++;
			tempstr[i]='>';
			c=getchar();
		}
		else{
			err[0]=1;
			return;
		}
	//*****************check space*****************
		if(checkSpace(c)){	
			i++;
			tempstr[i]=' ';
			c=getchar();
		}
		else{
			err[0]=1;
			return;
		}
	//*****************check right ID*****************	
		while(c!='#'){
			if(checkID(c, &i)){
				c=getchar();
			}
			else{
				err[0]=1;
				return;
			}
			if(checkSpace(c)){	
				i++;
				tempstr[i]=' ';
				c=getchar();
			}
			else{
				err[0]=1;
				return;
			}			
		}
		gramnum++;
		i++;
		tempstr[i]='#';
		c=getchar();
		if(checkSpace(c)){	
			i++;
			tempstr[i]=' ';
			c=getchar();
		}
		else if(c=='#'){
			i++;
			tempstr[i]='\0';
			break;
		}
	}
	//*****************put grammar into data structure*****************
	grammar=(struct gramnode*)malloc(sizeof(struct gramnode)*gramnum);
	for(i=0; i<gramnum; i++){
		grammar[i].right=(char**)malloc(sizeof(char*)*RIGHT);
		grammar[i].rightnum=0;

		for(j=0; j<SYMNUM && tempstr[k]!=' '; j++, k++){
			grammar[i].left[j]=tempstr[k];
		}
		if(j<SYMNUM)
			grammar[i].left[j]='\0';
		k++;	
		
		if(checkT(grammar[i].left)>=0)
			err[3]=1;
		else if(checkNT(grammar[i].left, 'l')<0 && checkT(grammar[i].left)<0)
			err[2]=1;
				
		while(tempstr[k]==' '||tempstr[k]=='>')
			k++;
		
		for(j=0; j<RIGHT && tempstr[k]!='#'; j++, k++){
			grammar[i].right[j]=(char*)malloc(sizeof(char)*SYMNUM);
			for(m=0;m<SYMNUM && tempstr[k] !=' ';m++,k++)
				grammar[i].right[j][m]=tempstr[k];
			if(m<SYMNUM){
				grammar[i].rightnum++;				
				grammar[i].right[j][m]='\0';
			}
			
			if(checkNT(grammar[i].right[j], 'r')<0 && checkT(grammar[i].right[j])<0)
				err[2]=1;			
		}			

		if(grammar[i].rightnum==0){
			grammar[i].right[0]=(char*)malloc(sizeof(char)*SYMNUM);
			grammar[i].right[0][0]='#';
			grammar[i].right[0][1]='\0';
			grammar[i].rightnum++;
		}
		k++;
		if(tempstr[k]=='\0'){
			if(leftntcount>0)
				err[1]=1;
			break;
		}
		else
			k++;
	}
	return;
}
//--------------------------------------------------------calculate First set-----------------------------------------------
void first(){
	int i, j, k, m=0, initial=0, tpos, ntpos, flag=0, change, changeTemp, emptycount=0;
	//*****************initialize tFirstlist*****************
	tFirstlist=(int**)malloc(sizeof(int*)*(ntnum+1)); //+1 for the terminal set
	for(i=0; i<ntnum+1; i++){
		tFirstlist[i]=(int*)malloc(sizeof(int)*(tnum+1));	//+1 for the empty string
		for(j=0; j<tnum+1; j++){
			if(i==ntnum)
				initial=1;
			tFirstlist[i][j]=initial;
			if(i<ntnum){
				for(k=0; k<gramnum; k++){
					if(strcmp(grammar[k].left, nonterminal[i])==0){
						tpos=checkT(grammar[k].right[0]);
						if(tpos>=0)
							tFirstlist[i][tpos]=1;
						if(grammar[k].right[0][0]=='#')
							tFirstlist[i][tnum]=1;
					}
				}
			}
		}
	}
	//*****************initialize ntFirstlist*****************
	ntFirstlist=(int**)malloc(sizeof(int*)*(ntnum+1)); //+1 for the nonterminal set
	for(i=0; i<ntnum+1; i++){
	 	ntFirstlist[i]=(int*)malloc(sizeof(int)*(ntnum));	 
		for(j=0; j<ntnum; j++){
			initial=0;
			if(i==ntnum)
				initial=1;
			ntFirstlist[i][j]=initial;
			if(i<ntnum){
				for(k=0; k<gramnum; k++){
					if(strcmp(grammar[k].left, nonterminal[i])==0){
						ntpos=checkNT(grammar[k].right[0], 'r');
						while(ntpos>=0 && m<grammar[k].rightnum && flag==0){
							ntFirstlist[i][ntpos]=1;
							flag=1;
							if( tFirstlist[ntpos][tnum]==1){
								m++;
								flag=0;
								if(m<grammar[k].rightnum)
									ntpos=checkNT(grammar[k].right[m], 'r');
							}
						}
						if(flag==0 && ntpos<0 && m<grammar[k].rightnum){
							tpos=checkT(grammar[k].right[m]);
							if(tpos>=0)
								tFirstlist[i][tpos]=1;
						}
						m=0;
						flag=0;
					}
				}
			}
		}
	} 
	//*****************calculate first set*****************
	flag=0, change=1, changeTemp=0;
	while(change==1){
		change=0;
		for(i=0; i<ntnum; i++){
			for(j=0; j<ntnum; j++){
				if(ntFirstlist[i][j]==1){
					changeTemp =setUnion(tFirstlist[i], tFirstlist[j], tnum);  //e.g. First(S)=First(A) means First(S)= First(S) U  First(A)
					if(changeTemp == 1)										   //this also checks if there are any changes from the last 
						change=1;													   //First set and the new First set. If there is no difference between the 
																						   //old one and the new one, terminate the updating of the First sets			
				}
			}

			for(j=0; j<gramnum; j++){
				emptycount=0;
				if(i==checkNT(grammar[j].left, 'r')){
					for(k=0; k<grammar[j].rightnum; k++){
						ntpos=checkNT(grammar[j].right[k], 'r');

						if(emptycount==k){
							if(ntpos>=0){
								ntFirstlist[i][ntpos]=1;
								if(tFirstlist[ntpos][tnum]==1)
									emptycount++;
								else
									break;
							}
							else{
								tpos=checkT(grammar[j].right[k]);
								if(tpos>=0)
									tFirstlist[i][tpos]=1;
								break;
							}
						}
						else
							break;
				}
					if(emptycount==grammar[j].rightnum)
						tFirstlist[i][tnum]=1;	
				}
			}
		}
	}
	return;
}
//--------------------------------------------------------calculate Follow set-----------------------------------------------
void follow(){
	int i, j, change, changeTemp, tempNT1, tempNT2, tempT, ntpos1, ntpos2;
	//*****************initialize followlists*****************
	tFollowlist=(int**)malloc(sizeof(int*)*ntnum);
	ntFollowlist=(int**)malloc(sizeof(int*)*ntnum);
	for(i=0; i<ntnum; i++){
		tFollowlist[i]=(int*)malloc(sizeof(int)*(tnum+1));		//+1: for eof
		for(j=0; j<tnum+1; j++)
			tFollowlist[i][j]=0;
		ntFollowlist[i]=(int*)malloc(sizeof(int)*ntnum);
		for(j=0; j<ntnum; j++)
			ntFollowlist[i][j]=0;
	}
	tFollowlist[0][tnum]=1;
	//*****************construct tFollowlist and ntFollowlist (only in two neighboring nodes)*****************
	for(i=0; i<gramnum; i++){
		for(j=0; j<grammar[i].rightnum; j++){
			tempNT1=checkNT(grammar[i].right[j], 'r');
			if(tempNT1>=0 && j+1<grammar[i].rightnum ){
				tempNT2=checkNT(grammar[i].right[j+1], 'r');
				tempT=checkT(grammar[i].right[j+1]);
				if(tempT>=0)
					tFollowlist[tempNT1][tempT]=1;
				else if(tempNT2>=0){
					ntFollowlist[tempNT1][tempNT2]=1;						
				}
			}
		}
	}
	//*****************if A->B, then Follow(A) belongs to Follow(B), including EOF*****************
	for(i=0; i<gramnum; i++){
		ntpos1=checkNT(grammar[i].left, 'r');
		for(j=grammar[i].rightnum-1; j>=0; j--){
			ntpos2=checkNT(grammar[i].right[j], 'r');
			if(ntpos2>=0){
				setUnion(ntFollowlist[ntpos2], ntFollowlist[ntpos1], ntnum);
				setUnion(tFollowlist[ntpos2], tFollowlist[ntpos1], tnum+1);
				if(tFirstlist[ntpos2][tnum]==0)
					break;
			}
			else
				break;
		}
	}
	//*****************if A->B1B2, then Follow(B1) contains First(B2)-epsilon*****************
	change=1, changeTemp=0;
	while(change==1){
		change=0;		
		
		for(i=0; i<ntnum; i++){
			for(j=0; j<ntnum; j++){
				if(ntFollowlist[i][j]==1)
					changeTemp=setUnion(tFollowlist[i], tFirstlist[j], tnum);
				if(changeTemp==1)
					change=1;
			}														    
		}	
	}
	return;	
}

void output(){
	int i, j, flag=0;
	//*****************print the First sets*****************
	for (i=0; i<ntnum; i++){
		flag=0;
		printf("FIRST(%s) = { ", nonterminal[i]);
		if(tFirstlist[i][tnum]==1){
			printf("#");
			flag=1;
		}
		for(j=0; j<tnum; j++){
			if(tFirstlist[i][j]==1){
				if(flag==1)
					printf(", ");
				printf(terminal[j]);
				flag=1;				
			}
		}
		printf(" }\n");
	}
	//*****************print the Follow sets*****************
	for (i=0; i<ntnum; i++){
		flag=0;
		printf("FOLLOW(%s) = { ", nonterminal[i]);
		if(tFollowlist[i][tnum]==1){
			printf("$");
			flag=1;
		}
		for(j=0; j<tnum; j++){
			if(tFollowlist[i][j]==1){
				if(flag==1)
					printf(", ");
				printf(terminal[j]);
				flag=1;
			}
		}
		printf(" }\n");
	}
	return;
}
//--------------------------------------------------------support functions-----------------------------------------------
int checkSpace(char c){
	int flag=0;
	if(isspace(c)){		//check space
		flag=1;
		c=getchar();
		while(isspace(c))
			c=getchar();
	}
	if(flag==1){
		ungetc(c, stdin);		
	}
	return flag;
}

int checkID(char c, int* i){
	int flag=0;
	if((c>='A' && c<='Z')||(c>='a' && c<='z')){ //check 1st char of ID
		flag=1;
		(*i)++;
		tempstr[*i]=c;
		c=getchar();		
		while((c>='A' && c<='Z')||(c>='a' && c<='z')||(c>='0' && c<='9')){ //check (digit+letter)* of ID
			(*i)++;
			tempstr[*i]=c;
			c=getchar();
		}
	}
	if(flag==1)
		ungetc(c, stdin);	
	return flag;
}

int checkArrow(char c){
	int flag=0;
	if(c=='-'){
		c=getchar();
		if(c=='>')
			flag=1;	
	}
	return flag;
}

int checkNT(char* symbol, char lr){
	int i, flag=-1;
	for (i=0;i<ntnum;i++){
		if(strcmp(symbol,nonterminal[i])==0){
			if(leftnt[i]==0 && lr=='l'){
				leftnt[i]=1;
				leftntcount--;
			}
			flag=i;
			break;
		}
	}
	return flag;
}

int checkT(char* symbol){
	int i, flag=-1;
	for(i=0;i<tnum;i++){
		if(strcmp(symbol,terminal[i])==0)
			flag=i;
	}
	return flag;
}

int setUnion(int* set1, int* set2, int length){
	int i, flag=0;
	for (i=0; i<length; i++){
		if (set1[i]==0 && set2[i]==0)
			set1[i]=0;
		else if(set1[i]==1 && set2[i]==1)
			set1[i]=1;
		else if(set1[i]==0 && set2[i]==1){
			set1[i]=1;
			flag=1;
		}
		else
			set1[i]=1;
	}
	return flag;
}

void sortTerminal(){
	int i, j, min=0;
	char* temp;
	temp=(char*)malloc(sizeof(char)*SYMNUM);
	for(i=0; i<tnum; i++){
		for(j=i+1; j<tnum; j++){
			if(strcmp(terminal[i],terminal[j])>0){
				strcpy(temp, terminal[i]);
				strcpy(terminal[i], terminal[j]);
				strcpy(terminal[j], temp);
			}
		}
	}
	return;
}
