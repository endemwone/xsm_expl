begin
    decl
        int n,arr[50],i,j,dup;
    enddecl
    
    read(n);
    i=0;
    while(i<n) do
        read(arr[i]);
        i = i+1;
    endwhile;
    i=0;
    while(i<n) do
        j=i;
        while(j<n) do
            if(arr[i]>arr[j]) then
                dup = arr[i];
                arr[i] = arr[j];
                arr[j] = dup;
            endif;
            j = j + 1;
        endwhile;
        i = i+1;
    endwhile;
    i=0;
    while(i<n) do
        write(arr[i]);
        i = i+1;
    endwhile;
end