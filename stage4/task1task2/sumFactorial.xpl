begin
    decl
        int n,i,j,sum,k;
    enddecl
    
    read(n);
    sum=0;
    k=1;
    while(k<=n)do
        if(n<=1) then
            sum=sum+1;
        else
            i=1;
            j=1;
            while(i<=k)do
                j=j*i;
                i=i+1;
            endwhile;
            sum=sum+j;
        endif;
        k=k+1;
    endwhile;
    write(sum);
end