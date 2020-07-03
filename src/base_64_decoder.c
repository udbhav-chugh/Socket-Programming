#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char * decode (char * inputString)
{
	int len = strlen(inputString); 

    char *ans = (char *) malloc(2000 * sizeof(char)); 

    int numBits = 0;
    int val = 0;
    int resIndex = 0;
    int count=0;

    for (int i = 0; i < len; i += 4) 
    {
    	val=0;
    	numBits=24;
    	count=0;
    	for(int j=i;j<=i+3 && j<len ; j++)
    	{
    		
    		int temp_index;
			if(inputString[j]>='A' && inputString[j]<='Z')
				temp_index = inputString[j]-'A';
			else if(inputString[j]>='a' && inputString[j]<='z')
				temp_index = inputString[j]-'a'+26;
			else if(inputString[j]>='0' && inputString[j]<='9')
				temp_index = inputString[j]-'0'+52;
			else if(inputString[j]=='+')
				temp_index=62;
			else if(inputString[j]=='/')
				temp_index=63;
    		else if (inputString[j]=='=')
    		{
    			count++;
    			continue;
    		}
    		val = val << 6;
			val = val | temp_index;
    	}

    	if(count==2)
    	{
    		val = val >> 4;
    		numBits=8;
    	}
    	else if(count==1)
    	{
    		val = val >> 2;
    		numBits=16;
    	}
   

    	for(int j=8;j<=numBits;j+=8)
    	{
    		int temp=val >> (numBits-j);
    		int index=temp%256;
    		ans[resIndex++]=(char)index;
    	}
    	
    }
    ans[resIndex]='\0';
    return ans;
}

