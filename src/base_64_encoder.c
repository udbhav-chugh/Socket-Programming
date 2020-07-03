#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char * encode (char * inputString)
{
    int len = strlen(inputString);
    char set64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

    char *ans = (char *) malloc(2000 * sizeof(char)); 

    int numBits = 0, pad = (3-len%3)%3;
    int val = 0;
    int resIndex = 0;

    for (int i = 0; i < len; i += 3) 
    {
    	val=0;
    	numBits=0;
    	for(int j=i;j<=i+2 && j<len ; j++)
    	{
    		val = val << 8;
    		val = val | inputString[j];
    		numBits+=8;
    	}

    	if(numBits==8)
    	{
    		val = val << 4;
    		numBits=12;
    	}
    	else if(numBits==16)
    	{
    		val = val << 2;
    		numBits=18;
    	}

    	for(int j=6;j<=numBits;j+=6)
    	{
    		int temp=val >> (numBits-j);
    		int index=temp%64;
    		char ch=set64[index];
    		ans[resIndex++]=ch;
    	}
    	
    }
    for(int j=0;j<pad;j++)
	{
		ans[resIndex++]='=';
	}
    ans[resIndex]='\0';

    return ans;
}
