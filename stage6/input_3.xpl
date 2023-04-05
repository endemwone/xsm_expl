type
  node
  {
    int d;
    int s;
    int t;
  }
  List
  {
    int a;
    int b;
    node g;
    List next;
  }
endtype
decl
  node y,z;
  List head;
  int insert(int a,int b,node g);
  node gcd(int m,int n);
enddecl
int insert(int a,int b,node g)
{
  decl
    List p,q;
    node temp;
  enddecl
  begin
      p=alloc();
      p.a=a;
      p.b=b;
      temp=alloc();
      temp.d=g.d;
      temp.s=g.s;
      temp.t=g.t;
      p.g=temp;
      if (head== null) then
          p.next=null;
          head=p;
      else
          p.next=head;
          head=p;
      endif;
  return 0;
  end
}

node gcd(int m,int n){

  decl
    int q,r,temp;
  enddecl

  begin
    if(n==0) then
        y.d = m;
        y.s = 1;
        y.t = 0;
    else
        q = m/n;
        r = m-q*n;
        z = gcd(n,r);
        temp = z.s;
        y.s = z.t;
        y.t = temp - (q*z.t);
    endif;
    temp=insert(m,n,y);
    return y;
  end
}

int main()
{
  decl
    int a,b,c;
    node res;
  enddecl

  begin
    initialize();
    y = alloc();
    read(a);
    read(b);
    head=null;
    res = gcd(a,b);
    write(res.d);
    write(res.s);
    write(res.t);
    write("stack");
    while(head!= null) do
        write(head.a);
        write(head.b);
        write(head.g.d);
        write(head.g.s);
        write(head.g.t);
        head=head.next;
        write("next");
    endwhile;
    return 0;
  end
}
