#include<stdio.h>
struct para
{
    char word[50];
}s[100000];
int date_check_1(char []);
int text_date_check(char [],char [],char [],char []);
int numeric_date_check(char [],int *,int *,int *);
int date_check(char []);
int mon_check(char []);
int year_check(char []);
void main()
{
    FILE *f=NULL,*fout;
    int n,i,j=0,k=0,l,c_a=0,c_an=0,c_the=0,c_date=0,val;
    char ch,f_name[50],ch1='\0';
    printf("Enter the filename : ");
    gets(f_name);
    f=fopen(f_name,"r");
    fout=fopen("out.txt","w");
    if(f==NULL)
    {
        printf("File not found\n");
        exit(0);
    }
    fscanf(f,"%d",&n);
    ch=fgetc(f);
    for(i=1;i<=n;)
    {
        ch=tolower(fgetc(f));
        if((ch=='\n'||ch==EOF)&&j!=0)
        {
            end_of_para:
            s[j].word[k]='\0';
            k=0;
            j++;
            strcpy(s[j].word,"\0");
            for(l=0;l<j;l++)
            {
                if(strcmp(s[l].word,"a")==0)
                c_a++;
                else if(strcmp(s[l].word,"an")==0)
                c_an++;
                else if(strcmp(s[l].word,"the")==0)
                c_the++;
            }
            for(l=0;l<j;)
            {
                val=date_check_1(s[l].word);
                if(val==1)
                {
                    c_date++;
                    printf("%s\n",s[l].word);
                    l++;
                }
                else
                {
                    if(l<j-2)
                    {
                        if(date_check(s[l].word)||mon_check(s[l].word)||year_check(s[l].word))
                        {
                            val=text_date_check(s[l].word,s[l+1].word,s[l+2].word,s[l+3].word);
                            if(val==1||val==2)
                            {
                                c_date++;
                                printf("%s %s %s %s\n",s[l].word,s[l+1].word,s[l+2].word,s[l+3].word);
                                if(val==1)
                                l=l+3;
                                else
                                l=l+4;
                            }
                            else
                            l++;
                        }
                        else
                        l++;
                    }
                    else
                    l++;
                }
            }
            printf("%d) %d, %d, %d, %d\n\n\n",i,c_a,c_an,c_the,c_date);
            fprintf(fout,"%d) %d, %d, %d, %d\n\n\n",i,c_a,c_an,c_the,c_date);
            i++;
            c_a=0,c_an=0,c_the=0,c_date=0,j=0,k=0;
        }
        else
        {
            if(ch=='/')
            {
                if(isdigit(s[j].word[k-1])==1)
                s[j].word[k++]=ch;
                else
                goto end_of_word;
            }
            else if(ch=='.')
            {
                if(isdigit(s[j].word[k-1])==0)
                goto end_of_word;
                else
                {
                    ch1=tolower(fgetc(f));
                    if(isdigit(ch1)==1)
                    {
                        s[j].word[k++]=ch;
                        s[j].word[k++]=ch1;
                    }
                    else if(ch1=='\n'||ch==EOF)
                    goto end_of_para;
                    else
                    goto end_of_word;
                }
            }
            else if(isalpha(ch)||isdigit(ch)||ch=='-'||ch=='@')
            s[j].word[k++]=ch;
            else
            {
                end_of_word:
                s[j].word[k]='\0';
                if(k!=0)
                j++;
                k=0;
                if(isalpha(ch1))
                {
                    s[j].word[k++]=ch1;
                    ch1='\0';
                }
            }
        }
    }
    fclose(f);
    fclose(fout);
}
int date_check_1(char a[])
{
    int l=strlen(a),i,c1=0,c2=0,c3=0,date=0,mon=0,year=0,flag1=0,flag2=0,k=0;
    char b[20],c[20],d[20];
    for(i=0;i<l;i++)
    {
        if(a[i]=='/')
        c1++;
        else if(a[i]=='-')
        c2++;
        else if(a[i]=='.')
        c3++;
    }
    if(c1==2||c2==2||c3==2)
    {
        i=0;
        while(a[i]!='/'&&a[i]!='-'&&a[i]!='.'&&i<l)
        {
            if(isdigit(a[i])==0)
            flag1=1;
            b[k++]=a[i++];
        }
        b[k]='\0';
        k=0,i++;
        while(a[i]!='/'&&a[i]!='-'&&a[i]!='.'&&i<l)
        {
            if(isdigit(a[i])==0)
            flag2=1;
            c[k++]=a[i++];
        }
        c[k]='\0';
        k=0,i++;
        while(a[i]!='/'&&a[i]!='-'&&a[i]!='.'&&i<l)
        {
            if(isdigit(a[i])==0)
            return 0;
            d[k++]=a[i++];
        }
        d[k]='\0';
        if(flag1==1||flag2==1)
        {
            return text_date_check(b,c,d,"\0");
        }
        else
        {
        	//printf("%s %s %s\n",b,c,d);
            if(numeric_date_check(b,&date,&mon,&year)==0)
            return 0;
            if(numeric_date_check(c,&date,&mon,&year)==0)
            return 0;
            if(numeric_date_check(d,&date,&mon,&year)==0)
            return 0;
            if(date==1&&mon==1&&year==1)
            return 1;
            else
            return 0;
        }
    }
    return 0;
}
int numeric_date_check(char b[],int *date,int *mon,int *year)
{
    int l;
    l=strlen(b);
    if(l==4&&*year==0)
    {
        if(b[0]=='0'&&b[1]=='0'&&b[2]=='0'&&b[3]=='0')
        return 0;
        *year=1;
    }
    else if(l==1&&b[0]!='0')
    {
        if(*mon==0)
        *mon=1;
        else if(*date==0)
        *date=1;
        else
        return 0;
    }
    else if(l==2)
    {
        if(b[0]=='0'&&b[1]=='0')
        return 0;
        if(*mon==1&&*date==1)
        *year=1;
        else if(b[0]=='0')
        {
            if(*mon==0)
            *mon=1;
            else if(*date==0)
            *date=1;
            else if(*year==0)
            *year=1;
        }
        else
        {
        	if(b[0]=='3')
        	{
        		if(b[1]=='0'||b[1]=='1')
        		{
        			if(*date==0)
        			*date=1;
        			else if(*year==0)
        			*year=1;
				}
				else
				{
					if(*year==0)
					*year=1;
				}
			}
            else if(b[0]>'3')
            {
                if(*year==0)
                *year=1;
                else
                return 0;
            }
            else if((b[0]=='1')&&(b[1]=='0'||b[1]=='1'||b[1]=='2'))
            {
                if(*mon==0)
                *mon=1;
                else if(*date==0)
                *date=1;
                else if(*year==0)
                *year=1;
            }
            else
            {
                if(*date==0)
                *date=1;
                else if(*year==0)
                *year=1;
                else
                return 0;
            }
        }
    }
    else
    return 0;
}
int text_date_check(char a[],char b[],char c[],char d[])
{
    int l,f=0,val_date,val_mon,val_year;
    val_date=date_check(a);
    val_mon=mon_check(a);
    val_year=year_check(a);
    if(val_date==1)
    {
        val_mon=mon_check(b);
        if(val_mon==1)
        {
            val_year=year_check(c);
            if(val_year==1)
            {
                return 1;
            }
            else
            {
                val_year=year_check(d);
                if(val_year==1)
                {
                    return 2;
                }
                return 0;
            }
        }
        else
        {
            val_mon=mon_check(c);
            if(val_mon==1)
            {
                val_year=year_check(d);
                if(val_year==1)
                {
                    return 2;
                }
                return 0;
            }
        }
    }
    else if(val_mon==1)
    {
        val_date=date_check(b);
        if(val_date==1)
        {
            val_year=year_check(c);
            if(val_year==1)
            {
                return 1;
            }
            else
            {
                val_year=year_check(d);
                if(val_year==1)
                {
                    return 2;
                }
                return 0;
            }
        }
        else
        {
            val_date=date_check(c);
            if(val_date==1)
            {
                val_year=year_check(d);
                if(val_year==1)
                return 2;
            }
            return 0;
        }
    }
    else if(val_year==1)
    {
        val_mon=mon_check(b);
        val_date=date_check(b);
        if(val_mon==1)
        {
            val_date=date_check(c);
            if(val_date==1)
            {
                return 1;
            }
            else
            {
                val_date==date_check(d);
                if(val_date==1)
                {
                    return 2;
                }
                return 0;
            }
        }
        else if(val_date==1)
        {
            val_mon=mon_check(c);
            if(val_mon==1)
            {
                return 1;
            }
            else
            {
                val_mon=mon_check(d);
                if(val_mon==1)
                {
                    return 2;
                }
                return 0;
            }
        }
        else
        {
            val_mon=mon_check(c);
            val_date=date_check(c);
            if(val_mon==1)
            {
                val_date=date_check(d);
                if(val_date==1)
                {
                    return 2;
                }
                return 0;
            }
            else if(val_date==1)
            {
                val_mon=mon_check(d);
                if(val_mon==1)
                {
                    return 2;
                }
                return 0;
            }
            return 0;
        }
    }
    return 0;
}
int date_check(char a[])
{
    if(isdigit(a[0])==1)
    {
        if((a[1]=='t'&&a[2]=='h'&&a[3]=='\0')||(a[1]=='s'&&a[2]=='t'&&a[3]=='\0')||(a[1]=='r'&&a[2]=='d'&&a[3]=='\0')||(a[1]=='n'&&a[2]=='d'&&a[3]=='\0')||(a[1]=='\0'))
        {
            return 1;
        }
        else if(isdigit(a[1])==1)
        {
            if((a[2]=='t'&&a[3]=='h')||(a[2]=='s'&&a[3]=='t')||(a[2]=='r'&&a[3]=='d')||(a[2]=='n'&&a[3]=='d')||(a[2]=='\0'))
            {
                return 1;
            }
        }
        return 0;
    }
    else
    {
        if(strcmp(a,"first")==0||strcmp(a,"second")==0||strcmp(a,"third")==0||strcmp(a,"fourth")==0)
        return 1;
        else if(strcmp(a,"fifth")==0||strcmp(a,"sixth")==0||strcmp(a,"seventh")==0||strcmp(a,"eighth")==0)
        return 1;
        else if(strcmp(a,"ninth")==0||strcmp(a,"tenth")==0||strcmp(a,"eleventh")==0||strcmp(a,"twelfth")==0)
        return 1;
        else if(strcmp(a,"thirteenth")==0||strcmp(a,"fourteenth")==0||strcmp(a,"fifteenth")==0||strcmp(a,"sixteenth")==0||strcmp(a,"seventeenth")==0)
        return 1;
        else if(strcmp(a,"eighteenth")==0||strcmp(a,"nineteenth")==0||strcmp(a,"twenty-first")==0||strcmp(a,"twenty-second")==0)
        return 1;
        else if(strcmp(a,"twenty-third")==0||strcmp(a,"twenty-fourth")==0||strcmp(a,"twenty-fifth")==0||strcmp(a,"twenty-sixth")==0)
        return 1;
        else if(strcmp(a,"twenty-seventh")==0||strcmp(a,"twenty-eighth")==0||strcmp(a,"twenty-ninth")==0||strcmp(a,"thirtieth")==0||strcmp(a,"thirty-first")==0)
        return 1;
        else
        return 0;
    }
    return 0;
}
int mon_check(char a[])
{
    if(strcmp(a,"jan")==0||strcmp(a,"january")==0)
    return 1;
    else if(strcmp(a,"feb")==0||strcmp(a,"february")==0)
    return 1;
    else if(strcmp(a,"mar")==0||strcmp(a,"march")==0)
    return 1;
    else if(strcmp(a,"apr")==0||strcmp(a,"april")==0)
    return 1;
    else if(strcmp(a,"may")==0)
    return 1;
    else if(strcmp(a,"jun")==0||strcmp(a,"june")==0)
    return 1;
    else if(strcmp(a,"jul")==0||strcmp(a,"july")==0)
    return 1;
    else if(strcmp(a,"aug")==0||strcmp(a,"august")==0)
    return 1;
    else if(strcmp(a,"sep")==0||strcmp(a,"september")==0||strcmp(a,"sept")==0)
    return 1;
    else if(strcmp(a,"oct")==0||strcmp(a,"october")==0)
    return 1;
    else if(strcmp(a,"nov")==0||strcmp(a,"november")==0)
    return 1;
    else if(strcmp(a,"dec")==0||strcmp(a,"december")==0)
    return 1;
    else
    return 0;
}
int year_check(char a[])
{
    int l=strlen(a);
    if(l==4)
    {
        if(isdigit(a[0])==1&&isdigit(a[1])==1&&isdigit(a[2])==1&&isdigit(a[3])==1)
        {
            if(a[0]=='0'&&a[1]=='0'&&a[2]=='0'&&a[3]=='0')
            return 0;
            return 1;
        }
        else
        return 0;
    }
    else
    return 0;
}


