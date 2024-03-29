decl
  int a,b,n,arr[10];
  int swap(int index, int newindex);
  int readarr(int temp);
  int writearr(int temp);
  int partition (int low, int high);
  int quicksort(int low, int mid, int high);
enddecl

int swap(int index ,int newindex)
{
  decl
    int s;
  enddecl

  begin
    s = arr[index];
    arr[index] = arr[newindex];
    arr[newindex] = s;

    return s;
  end
}

int readarr(int temp)
{
  decl
    int count;
  enddecl

  begin
    count=0;

    while(count < temp) do
      read(arr[count]);
      count=count+1;
    endwhile;

    return count;
  end
}

int writearr(int temp)
{
  decl
    int count;
  enddecl

  begin
    count = 0;

    while(count < temp) do
      write(arr[count]);
      count = count+1;
    endwhile;

    return count;
  end
}

int partition (int low, int high)
{
  decl
    int pivot,t;
    int i,j;
  enddecl

  begin
    pivot = arr[high];
    j = low;
    i = low;

    while(i < high) do
      if (arr[i] <= pivot) then
      t = swap (i , j);
      j = j+1;
      endif;
      i=i+1;
    endwhile;

    i = swap (j , high);

    return j;
  end
}



int quicksort (int low , int mid, int high)
{
  decl
    int pp,temp,t;
  enddecl

  begin

    if (low < high) then
      pp = partition (low , high);
      temp = quicksort(pp+1,quicksort(low ,0, pp-1),high);
    endif;

    return 1;
  end
}

int main()
{
  decl
    int junk;
    int temp;
  enddecl

  begin

    read(n);
    junk = readarr(n);
    temp = quicksort(0,0,n-1);
    junk = writearr(n);

    return junk;
  end
}
