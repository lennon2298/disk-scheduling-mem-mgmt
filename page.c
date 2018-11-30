#include<stdio.h>
fifo()
{
int i,j,n,a[50],frame[10],no,k,avail,count=0;
            printf("FIFO\n");
            printf("\n ENTER THE NUMBER OF PAGES:\n");
scanf("%d",&n);
            printf("\n ENTER THE PAGE NUMBER :\n");
            for(i=1;i<=n;i++)
            scanf("%d",&a[i]);
            printf("\n ENTER THE NUMBER OF FRAMES :");
            scanf("%d",&no);
for(i=0;i<no;i++)
            frame[i]= -1;
                        j=0;
                        printf("\tref string\t page frames\n");
for(i=1;i<=n;i++)
                        {
                                    printf("%d\t\t",a[i]);
                                    avail=0;
                                    for(k=0;k<no;k++)
if(frame[k]==a[i])
                                                avail=1;
                                    if (avail==0)
                                    {
                                                frame[j]=a[i];
                                                j=(j+1)%no;
                                                count++;
                                                for(k=0;k<no;k++)
                                                printf("%d\t",frame[k]);
}
                                    printf("\n");
}
                        printf("Page Fault Is %d\n",count);
                        return 0;
}


lru()
{
int q[20],p[50],c=0,c1,d,f,i,j,k=0,n,r,t,b[20],c2[20];
printf("LRU\n");
printf("\n ENTER THE NUMBER OF PAGES:\n");
scanf("%d",&n);
printf("\n ENTER THE PAGE NUMBER :\n");
for(i=0;i<n;i++)
            scanf("%d",&p[i]);
printf("\n ENTER THE NUMBER OF FRAMES :");
scanf("%d",&f);
q[k]=p[k];
printf("\n\t%d\n",q[k]);
c++;
k++;
for(i=1;i<n;i++)
            {
                        c1=0;
                        for(j=0;j<f;j++)
                        {
                                    if(p[i]!=q[j])
                                    c1++;
                        }
                        if(c1==f)
                        {
                                    c++;
                                    if(k<f)
                                    {
                                                q[k]=p[i];
                                                k++;
                                                for(j=0;j<k;j++)
                                                printf("\t%d",q[j]);
                                                printf("\n");
                                    }
                                    else
                                    {
                                                for(r=0;r<f;r++)
                                                {
                                                            c2[r]=0;
                                                            for(j=i-1;j<n;j--)
                                                            {
                                                            if(q[r]!=p[j])
                                                            c2[r]++;
                                                            else
                                                            break;
                                                }
                                    }
                                    for(r=0;r<f;r++)
                                     b[r]=c2[r];
                                    for(r=0;r<f;r++)
                                    {
                                                for(j=r;j<f;j++)
                                                {
                                                            if(b[r]<b[j])
                                                            {
                                                                        t=b[r];
                                                                        b[r]=b[j];
                                                                        b[j]=t;
                                                            }
                                                }
                                    }
                                    for(r=0;r<f;r++)
                                    {
                                                if(c2[r]==b[0])
                                                q[r]=p[i];
                                                printf("\t%d",q[r]);
                                    }
                                    printf("\n");
                        }
            }
}
printf("\npage faults is %d\n",c);
}


optimal()
{
    int fr[5],i,j,k,t[5],p=1,flag=0,page[25],size,nf,t1,u[5];
    printf("OPTIMAL\n");
    printf("\n ENTER THE NUMBER OF FRAMES :");
    scanf("%d",&nf);
    printf("\n ENTER THE NUMBER OF PAGES:\n");
    scanf("%d",&size);

    printf("\n ENTER THE PAGE NUMBER :\n");
    for(i=1; i<=size; i++)
        scanf("%d",&page[i]);

    for(i=1; i<=nf; i++)
        fr[i]=-1;
    for(i=1; i<=size; i++)
    {
        if(full(fr,nf)==1)
           break;
        else
        {
            flag=0;
            for(j=1; j<=nf; j++)
            {
               if(page[i]==fr[j])
                {
                    flag=1;
                    printf("          \t%d:\t",page[i]);
                    break;
                }
            }
            if(flag==0)
            {
                fr[p]=page[i];
                printf("          \t%d:\t",page[i]);
                p++;
            }

            for(j=1; j<=nf; j++)
                printf(" %d  ",fr[j]);
            printf("\n");
        }
    }
    p=0;
    for(; i<=size; i++)
    {
        flag=0;
        for(j=1; j<=nf; j++)
        {
            if(page[i]==fr[j])
            {
                flag=1;
                break;
            }
        }
       if(flag==0)
        {
            p++;
            for(j=1; j<=nf; j++)
            {
                for(k=i+1; k<=size; k++)
                {
                    if(fr[j]==page[k])
                    {
                        u[j]=k;
                        break;
                    }
                    else
                        u[j]=21;
                }
            }
            for(j=1; j<=nf; j++)
                t[j]=u[j];
            for(j=1; j<=nf; j++)
            {
                for(k=j+1; k<=nf; k++)
                {
                    if(t[j]<t[k])
                    {
                        t1=t[j];
                        t[j]=t[k];
                        t[k]=t1;
                    }
                }
            }
            for(j=1; j<=nf; j++)
            {
                if(t[1]==u[j])
                {
                    fr[j]=page[i];
                    u[j]=i;
                }
            }
            printf("page fault\t");
        }
        else
            printf("          \t");
        printf("%d:\t",page[i]);
        for(j=1; j<=nf; j++)
            printf(" %d  ",fr[j]);
        printf("\n");
    }
    printf("\ntotal page faults:  %d",p+3);
}
int full(int a[],int n)
{
    int k;
    for(k=1; k<=n; k++)
    {
        if(a[k]==-1)
            return 0;
    }
    return 1;
}

void main()
{
    fifo();
    lru();
    optimal();

}
